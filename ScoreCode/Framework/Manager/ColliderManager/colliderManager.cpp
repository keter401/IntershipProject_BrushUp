#include "Framework\Manager\ColliderManager\colliderManager.h"
#include "Scene\Scenes\scene.h"
#include "Bullet\bullet.h"
#include <unordered_set> 

void DWColliderManager::Init()
{
    Tag = DWManager::EManagerTag::ColliderManager;
    CollideObjectList.clear();
    PrevCollideObjectList.clear();
}

void DWColliderManager::Uninit()
{
    CollideObjectList.clear();
    PrevCollideObjectList.clear();
}

void DWColliderManager::Update()
{
    DWScene* scene = GetCurrentScene();
    if (scene == nullptr) return;

    std::vector<DWGameObject*> all = scene->GetGameObjects<DWGameObject>();
    std::unordered_set<DWGameObject*> alive(all.begin(), all.end());

    std::vector<DWGameObject*> targets;
    targets.reserve(all.size());
    for (auto* go : all)
    {
        if (go == nullptr) continue;
        if (go->GetComponent<DWBoxCollider2D>())
            targets.push_back(go);
    }

    const int list_Max = static_cast<int>(targets.size());
    std::vector<DWGameObject*> newPairs;
    newPairs.reserve(CollideObjectList.size());

    for (int i = 0; i < list_Max - 1; ++i)
    {
        DWGameObject* objA = targets[i];
        DWBoxCollider2D* ca = objA->GetComponent<DWBoxCollider2D>();

        if (objA->GetTag() == DWGameObject::ETag::BULLET)
        {
            DWBullet* bullet = dynamic_cast<DWBullet*>(objA);
            if (!bullet->IsActive())  continue;
        }

        for (int j = i + 1; j < list_Max; ++j)
        {
            DWGameObject* objB = targets[j];
            if (objA->GetTag() == objB->GetTag()) continue;

            DWBoxCollider2D* cb = objB->GetComponent<DWBoxCollider2D>();
            if (ca == nullptr || cb == nullptr) continue;
			if (!ca->IsActive() || !cb->IsActive()) continue;

            if (objB->GetTag() == DWGameObject::ETag::BULLET)
            {
                DWBullet* bullet = dynamic_cast<DWBullet*>(objB);
                if (!bullet->IsActive())  continue;
            }

            if (objA->GetTag() == DWGameObject::ETag::PLAYER && objB->GetTag() == DWGameObject::ETag::BULLET ||
                objA->GetTag() == DWGameObject::ETag::BULLET && objB->GetTag() == DWGameObject::ETag::PLAYER)
                continue;


            if (ObjectsOverlap(ca, cb))
            {
                DWGameObject* x = (objA < objB) ? objA : objB;
                DWGameObject* y = (objA < objB) ? objB : objA;

                newPairs.push_back(x);
                newPairs.push_back(y);
            }
        }
    }

    const auto oldPairs = CollideObjectList;
    CollideObjectList = std::move(newPairs);

    for (size_t i = 0; i + 1 < CollideObjectList.size(); i += 2)
    {
        DWGameObject* a = CollideObjectList[i];
        DWGameObject* b = CollideObjectList[i + 1];

        if (a == nullptr || b == nullptr) continue;
        if (a->GetDestoryFlag() || b->GetDestoryFlag()) continue;
        if (a->GetTag() == b->GetTag()) continue;

        if (a) a->OnCollisionEnter2D(b);
        if (b) b->OnCollisionEnter2D(a);
    }

    for (size_t i = 0; i + 1 < oldPairs.size(); i += 2)
    { 
        DWGameObject* a = oldPairs[i];
        DWGameObject* b = oldPairs[i + 1];

        if (a == nullptr|| b == nullptr ) continue;
        const bool destroyed = (a->GetDestoryFlag() || b->GetDestoryFlag());
        const bool stillColliding = PairExistsIn(CollideObjectList, a, b);

        if (!stillColliding || destroyed)
        {
            if (a != nullptr && !a->GetDestoryFlag()) a->OnCollisionExit2D(b);
            if (b != nullptr && !b->GetDestoryFlag()) b->OnCollisionExit2D(a);
        }
    }

}

void DWColliderManager::ClearList()
{
    CollideObjectList.clear();
    PrevCollideObjectList.clear();
}

std::vector<DWGameObject*> DWColliderManager::GetCollidingObjects(DWGameObject* obj) const
{
    std::vector<DWGameObject*> out;
    if (obj == nullptr) return out;

    for (size_t i = 0; i + 1 < CollideObjectList.size(); i += 2)
    {
        DWGameObject* a = CollideObjectList[i];
        DWGameObject* b = CollideObjectList[i + 1];
        if (a == obj) out.push_back(b);
        else if (b == obj) out.push_back(a);
    }
    return out;
}

int DWColliderManager::FindPairIndex(DWGameObject* objA, DWGameObject* objB) const
{

    for (size_t i = 0; i + 1 < CollideObjectList.size(); i += 2)
    {
        if (CollideObjectList[i] == objA && CollideObjectList[i + 1] == objB)
            return static_cast<int>(i);
    }
    return -1;
}

void DWColliderManager::PushPairNormalized(DWGameObject* x, DWGameObject* y)
{
    CollideObjectList.push_back(x);
    CollideObjectList.push_back(y);
}

bool DWColliderManager::ObjectsOverlap(const DWBoxCollider2D* c1, const DWBoxCollider2D* c2)
{
    if (c1 == nullptr || c2 == nullptr) return false;

    const DWVector2 pos1 = c1->GetBoundingBoxPosition();
    const DWVector2 scale1 = c1->GetBoundingBoxScale();
    const DWVector2 pos2 = c2->GetBoundingBoxPosition();
    const DWVector2 scale2 = c2->GetBoundingBoxScale();

    const float halfW1 = scale1.x * 0.5f, halfH1 = scale1.y * 0.5f;
    const float halfW2 = scale2.x * 0.5f, halfH2 = scale2.y * 0.5f;

    const bool overlapX = std::fabs(pos1.x - pos2.x) <= (halfW1 + halfW2);
    const bool overlapY = std::fabs(pos1.y - pos2.y) <= (halfH1 + halfH2);
    return overlapX && overlapY;
}

bool DWColliderManager::PairExistsIn(
    const std::vector<DWGameObject*>& flatList,
    DWGameObject* a, DWGameObject* b) const
{
    DWGameObject* x = (a < b) ? a : b;
    DWGameObject* y = (a < b) ? b : a;

    for (size_t i = 0; i + 1 < flatList.size(); i += 2)
    {
        if (flatList[i] == x && flatList[i + 1] == y) return true;
    }
    return false;
}

bool DWColliderManager::IsCollidingWithTagThisFrame(DWGameObject* obj, DWGameObject::ETag tag) const
{
    if (obj == nullptr) return false;

    for (size_t i = 0; i + 1 < CollideObjectList.size(); i += 2) 
    {
        DWGameObject* a = CollideObjectList[i];
        DWGameObject* b = CollideObjectList[i + 1];
        if (a == obj && b && b->GetTag() == tag) return true;
        if (b == obj && a && a->GetTag() == tag) return true;
    }
    return false;
}