#include "Framework\Manager\ColliderManager\colliderManager.h"
#include "Scene\Scenes\scene.h"
#include <unordered_map>
#include <algorithm>
#include <cmath>

void DWColliderManager::Init()
{
    Tag = DWManager::EManagerTag::ColliderManager;
    CurrentPairs.clear();
}

void DWColliderManager::Uninit()
{
    CurrentPairs.clear();
}

void DWColliderManager::Update()
{
    DWScene* scene = GetCurrentScene();
    if (scene == nullptr) return;

    const std::vector<DWGameObject*> all = scene->GetGameObjects<DWGameObject>();
    const std::unordered_set<DWGameObject*> alive(all.begin(), all.end());

    // 前フレームのペア。既に delete されたオブジェクトを含むペアはここで捨てる
    // (Destroy() されたポインタを触るとダングリングになるため)
    std::vector<Pair> previousPairs;
    previousPairs.reserve(CurrentPairs.size());
    for (const Pair& p : CurrentPairs)
    {
        if (alive.count(p.first) != 0 && alive.count(p.second) != 0)
        {
            previousPairs.push_back(p);
        }
    }
    const PairSet previousSet(previousPairs.begin(), previousPairs.end());

    // 有効なコライダーを 1 回だけ集め、動く物 (プレイヤー/敵/弾) と動かない物 (ブロック) に分ける。
    // 以前は総当たりの内側ループで毎回 GetComponent (= dynamic_cast) していた
    std::vector<Entry> dynamics;
    std::vector<Entry> statics;
    dynamics.reserve(32);
    statics.reserve(all.size());

    for (DWGameObject* go : all)
    {
        if (go == nullptr) continue;

        DWBoxCollider2D* collider = go->GetComponent<DWBoxCollider2D>();
        if (collider == nullptr || !collider->IsActive()) continue;

        if (go->GetTag() == DWGameObject::ETag::BLOCK)
        {
            statics.push_back({ go, collider });
        }
        else
        {
            dynamics.push_back({ go, collider });
        }
    }

    // ブロックを空間ハッシュへ登録。ブロックは動かないので O(n)
    std::unordered_map<long long, std::vector<int>> grid;
    grid.reserve(statics.size() * 2);
    for (int i = 0; i < static_cast<int>(statics.size()); ++i)
    {
        int minX, minY, maxX, maxY;
        CellRange(statics[i].Collider, minX, minY, maxX, maxY);
        for (int cy = minY; cy <= maxY; ++cy)
        {
            for (int cx = minX; cx <= maxX; ++cx)
            {
                grid[CellKey(cx, cy)].push_back(i);
            }
        }
    }

    std::vector<Pair> newPairs;
    newPairs.reserve(previousPairs.size() + 8);

    // 動く物 × 動く物: 数が少ない (プレイヤー1 + 敵 + 弾10) ので総当たりで十分
    for (size_t i = 0; i < dynamics.size(); ++i)
    {
        for (size_t j = i + 1; j < dynamics.size(); ++j)
        {
            const Entry& a = dynamics[i];
            const Entry& b = dynamics[j];
            if (!CanCollide(a.Object, b.Object)) continue;

            if (a.Collider->IsCollidingWith(b.Collider))
            {
                newPairs.push_back(MakePair(a.Object, b.Object));
            }
        }
    }

    // 動く物 × ブロック: 自分の AABB が触れるセルに登録されたブロックだけ調べる
    std::vector<int> candidates;
    for (const Entry& d : dynamics)
    {
        int minX, minY, maxX, maxY;
        CellRange(d.Collider, minX, minY, maxX, maxY);

        candidates.clear();
        for (int cy = minY; cy <= maxY; ++cy)
        {
            for (int cx = minX; cx <= maxX; ++cx)
            {
                auto it = grid.find(CellKey(cx, cy));
                if (it == grid.end()) continue;
                candidates.insert(candidates.end(), it->second.begin(), it->second.end());
            }
        }

        // 複数セルにまたがるブロックが重複するので除去 (ソートすることで生成順も安定する)
        std::sort(candidates.begin(), candidates.end());
        candidates.erase(std::unique(candidates.begin(), candidates.end()), candidates.end());

        for (int index : candidates)
        {
            const Entry& s = statics[index];
            if (!CanCollide(d.Object, s.Object)) continue;

            if (d.Collider->IsCollidingWith(s.Collider))
            {
                newPairs.push_back(MakePair(d.Object, s.Object));
            }
        }
    }

    CurrentPairs = std::move(newPairs);
    const PairSet currentSet(CurrentPairs.begin(), CurrentPairs.end());

    // --- Enter / Stay ---
    // 前フレームにも存在したペアは Stay、初めて成立したペアは Enter
    for (const Pair& p : CurrentPairs)
    {
        DWGameObject* a = p.first;
        DWGameObject* b = p.second;

        if (a->GetDestroyFlag() || b->GetDestroyFlag()) continue;

        if (previousSet.count(p) != 0)
        {
            a->OnCollisionStay2D(b);
            b->OnCollisionStay2D(a);
        }
        else
        {
            a->OnCollisionEnter2D(b);
            b->OnCollisionEnter2D(a);
        }
    }

    // --- Exit (1) 前フレームにあって今フレームに無いペア ---
    for (const Pair& p : previousPairs)
    {
        if (currentSet.count(p) == 0)
        {
            NotifyExit(p.first, p.second);
        }
    }

    // --- Exit (2) 今フレームのペアのうち、コールバック中に破棄/無効化されたもの ---
    // これらは次フレームに持ち越さない (delete 済みポインタを触らない /
    // プールから再利用された弾が「継続中」と誤判定されない)
    std::vector<Pair> survivors;
    survivors.reserve(CurrentPairs.size());
    for (const Pair& p : CurrentPairs)
    {
        if (IsPairEnded(p.first, p.second))
        {
            NotifyExit(p.first, p.second);
        }
        else
        {
            survivors.push_back(p);
        }
    }
    CurrentPairs = std::move(survivors);
}

DWColliderManager::Pair DWColliderManager::MakePair(DWGameObject* a, DWGameObject* b)
{
    return (a < b) ? Pair(a, b) : Pair(b, a);
}

bool DWColliderManager::CanCollide(const DWGameObject* a, const DWGameObject* b)
{
    if (a == nullptr || b == nullptr) return false;

    const DWGameObject::ETag tagA = a->GetTag();
    const DWGameObject::ETag tagB = b->GetTag();

    // 同じ種類同士 (ブロック×ブロック, 敵×敵 など) は判定しない
    if (tagA == tagB) return false;

    // プレイヤーの弾はプレイヤー自身に当たらない
    if ((tagA == DWGameObject::ETag::PLAYER && tagB == DWGameObject::ETag::BULLET) ||
        (tagA == DWGameObject::ETag::BULLET && tagB == DWGameObject::ETag::PLAYER))
    {
        return false;
    }

    return true;
}

bool DWColliderManager::IsPairEnded(DWGameObject* a, DWGameObject* b)
{
    if (a == nullptr || b == nullptr) return true;
    if (a->GetDestroyFlag() || b->GetDestroyFlag()) return true;

    DWBoxCollider2D* ca = a->GetComponent<DWBoxCollider2D>();
    DWBoxCollider2D* cb = b->GetComponent<DWBoxCollider2D>();
    if (ca == nullptr || cb == nullptr) return true;
    if (!ca->IsActive() || !cb->IsActive()) return true;

    return false;
}

void DWColliderManager::NotifyExit(DWGameObject* a, DWGameObject* b)
{
    if (a != nullptr && !a->GetDestroyFlag()) a->OnCollisionExit2D(b);
    if (b != nullptr && !b->GetDestroyFlag()) b->OnCollisionExit2D(a);
}

long long DWColliderManager::CellKey(int cx, int cy)
{
    // 負のセル座標も扱えるよう、上位 32bit に x、下位 32bit に y を詰める
    const unsigned long long ux = static_cast<unsigned int>(cx);
    const unsigned long long uy = static_cast<unsigned int>(cy);
    return static_cast<long long>((ux << 32) | uy);
}

void DWColliderManager::CellRange(const DWBoxCollider2D* collider, int& minX, int& minY, int& maxX, int& maxY)
{
    const DWVector2 pos = collider->GetBoundingBoxPosition();
    const DWVector2 half = collider->GetBoundingBoxScale() * 0.5f;

    minX = static_cast<int>(std::floor((pos.x - half.x) / CellSize));
    minY = static_cast<int>(std::floor((pos.y - half.y) / CellSize));
    maxX = static_cast<int>(std::floor((pos.x + half.x) / CellSize));
    maxY = static_cast<int>(std::floor((pos.y + half.y) / CellSize));
}

void DWColliderManager::ClearList()
{
    CurrentPairs.clear();
}

std::vector<DWGameObject*> DWColliderManager::GetCollidingObjects(DWGameObject* obj) const
{
    std::vector<DWGameObject*> out;
    if (obj == nullptr) return out;

    for (const Pair& p : CurrentPairs)
    {
        if (p.first == obj) out.push_back(p.second);
        else if (p.second == obj) out.push_back(p.first);
    }
    return out;
}

bool DWColliderManager::IsCollidingWithTagThisFrame(DWGameObject* obj, DWGameObject::ETag tag) const
{
    if (obj == nullptr) return false;

    for (const Pair& p : CurrentPairs)
    {
        if (p.first == obj && p.second != nullptr && p.second->GetTag() == tag) return true;
        if (p.second == obj && p.first != nullptr && p.first->GetTag() == tag) return true;
    }
    return false;
}
