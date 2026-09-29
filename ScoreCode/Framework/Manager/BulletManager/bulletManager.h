#pragma once

#include "Framework\Manager\manager.h"
#include <vector>
#include <functional>

class DWBullet;
class DWScene;

class DWBulletManager : public DWManager
{
private:
    size_t Capacity;
    std::vector<DWBullet*> Storage;
    std::vector<DWBullet*> Free;
    std::vector<DWBullet*> Active;

public:
    DWBulletManager(DWScene* scene, size_t capacity = 10)
        : DWManager(scene), Capacity(capacity) {}

    void Init() override;
    void Uninit() override;
    void Update() override;

    void Spawn(DWVector2 pos);

    void Despawn(DWBullet* bullet);

    size_t GetActiveCount() const { return Active.size(); }
    size_t GetFreeCount() const { return Free.size(); }
};