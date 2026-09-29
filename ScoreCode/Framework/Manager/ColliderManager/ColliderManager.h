#pragma once

#include "Framework\Manager\manager.h"
#include "Framework\Components\BoxCollider\BoxCollider.h"
#include "Framework\gameObject.h"
#include <vector>
#include <algorithm>
#include <utility>
#include <cmath>

class DWColliderManager : public DWManager
{
private:
    std::vector<DWGameObject*> CollideObjectList;
    std::vector<DWGameObject*> PrevCollideObjectList;

    int  FindPairIndex(DWGameObject* a, DWGameObject* b) const;
    void PushPairNormalized(DWGameObject* x, DWGameObject* y);
    bool ObjectsOverlap(const DWBoxCollider2D* c1, const DWBoxCollider2D* c2);

    bool PairExistsIn(
        const std::vector<DWGameObject*>& flatList,
        DWGameObject* a, DWGameObject* b) const;

    // 破棄フラグ / コライダー無効化で、このペアがもう成立しないか
    static bool IsPairEnded(DWGameObject* a, DWGameObject* b);
    // 生存している側にだけ Exit を通知
    static void NotifyExit(DWGameObject* a, DWGameObject* b);

public:
    DWColliderManager(DWScene* scene) : DWManager(scene) {}

    void Init() override;
    void Uninit() override;
    void Update() override;

    void ClearList();

    std::vector<DWGameObject*> GetCollidingObjects(DWGameObject* obj) const;

    const std::vector<DWGameObject*>& GetCollidePairsFlat() const { return CollideObjectList; }
    bool IsCollidingWithTagThisFrame(DWGameObject* obj, DWGameObject::ETag tag) const;
};