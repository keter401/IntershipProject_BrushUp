#pragma once

#include "Framework\Manager\manager.h"
#include "Framework\Components\BoxCollider\BoxCollider.h"
#include "Framework\gameObject.h"
#include <vector>
#include <utility>
#include <unordered_set>

class DWColliderManager : public DWManager
{
public:
    // 衝突ペア。常に first < second (ポインタ値) に正規化して重複を防ぐ
    using Pair = std::pair<DWGameObject*, DWGameObject*>;

private:
    struct PairHash
    {
        size_t operator()(const Pair& p) const noexcept
        {
            const size_t a = std::hash<DWGameObject*>{}(p.first);
            const size_t b = std::hash<DWGameObject*>{}(p.second);
            return a ^ (b + 0x9e3779b9 + (a << 6) + (a >> 2));
        }
    };
    using PairSet = std::unordered_set<Pair, PairHash>;

    // GetComponent<DWBoxCollider2D>() の結果をフレーム内で使い回すためのペア
    struct Entry
    {
        DWGameObject* Object = nullptr;
        DWBoxCollider2D* Collider = nullptr;
    };

    // 静的オブジェクト (ブロック) を登録する空間ハッシュのセル幅 (px)
    static constexpr float CellSize = 64.0f;

    // 今フレーム成立している衝突ペア (発見順)
    std::vector<Pair> CurrentPairs;

    static Pair MakePair(DWGameObject* a, DWGameObject* b);
    // タグの組み合わせとして衝突判定の対象になるか
    static bool CanCollide(const DWGameObject* a, const DWGameObject* b);
    // 破棄フラグ / コライダー無効化で、このペアがもう成立しないか
    static bool IsPairEnded(DWGameObject* a, DWGameObject* b);
    // 生存している側にだけ Exit を通知
    static void NotifyExit(DWGameObject* a, DWGameObject* b);

    // 空間ハッシュ用
    static long long CellKey(int cx, int cy);
    static void CellRange(const DWBoxCollider2D* collider, int& minX, int& minY, int& maxX, int& maxY);

public:
    DWColliderManager(DWScene* scene) : DWManager(scene) {}

    void Init() override;
    void Uninit() override;
    void Update() override;

    void ClearList();

    std::vector<DWGameObject*> GetCollidingObjects(DWGameObject* obj) const;
    bool IsCollidingWithTagThisFrame(DWGameObject* obj, DWGameObject::ETag tag) const;
};
