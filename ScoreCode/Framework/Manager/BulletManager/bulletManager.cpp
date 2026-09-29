#include "bulletManager.h"
#include "Bullet\bullet.h"
#include "Scene\Scenes\scene.h"
#include "Player\player.h"
#include <algorithm>

void DWBulletManager::Init()
{
    Storage.reserve(Capacity);
    Free.reserve(Capacity);
    Active.reserve(Capacity);

    if(CurrentScene == nullptr) return;

    for (size_t i = 0; i < Capacity; ++i)
    {
        DWBullet* bullet = new DWBullet();

        // Init はシーンが行う。弾の所有者はシーン (GameObjectList) で、このマネージャは貸し出し台帳だけ持つ
        CurrentScene->AddGameObject(bullet, DWScene::ELAYER::FIELD);
        bullet->Deactivate();
        bullet->SetNumber(static_cast<int>(i + 1));

        Storage.push_back(bullet);
        Free.push_back(bullet);
    }
}

void DWBulletManager::Uninit()
{
    // 弾本体はシーンが delete する。ここでは台帳を空にするだけ
    Storage.clear();
    Free.clear();
    Active.clear();
}

void DWBulletManager::Update()
{

}

void DWBulletManager::Spawn(DWVector2 pos)
{
    DWBullet* bullet = nullptr;

    if (!Free.empty())
    {
        bullet = Free.back();
        Free.pop_back();
    }
    else if (!Active.empty())
    {
        bullet = Active.front();
        Active.erase(Active.begin());
    }
    else
    {
        return;
    }

    if(bullet == nullptr) return;

    bullet->Activate(this, pos);
    Active.push_back(bullet);
}

void DWBulletManager::Despawn(DWBullet* bullet)
{
    auto theBullet = std::find(Active.begin(), Active.end(), bullet);

    if (theBullet != Active.end())
    {
        Active.erase(theBullet);
    }
    bullet->Deactivate();
    Free.push_back(bullet);
}
