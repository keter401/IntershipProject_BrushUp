#include "Scene\sceneManager.h"
#include "Scene\Scenes\GameScene\gameScene.h"
#include "Player\player.h"
#include "Enemy\FloatingEnemy\floatingEnemy.h"
#include "Enemy\CrawlingEnemy\crawlingEnemy.h"
#include "Framework\Manager\ColliderManager\colliderManager.h"
#include "Framework\Manager\BulletManager\bulletManager.h"
#include "Framework\Manager\manager.h"
#include "Field\field.h"
#include "Block\block.h"
#include "Camera\camera.h"
#include "UI\UI.h"
#include "Audio\audio.h"

void DWGameScene::Init()
{
	AddGameObject<DWUI>(ELAYER::UI, this);
    DWAudio* audio = AddManager<DWAudio>(this);

    DWPlayer* player = AddGameObject<DWPlayer>(ELAYER::FIELD, this);
    if (player != nullptr)
    {
        player->SetInput(Input);
        AddGameObject<DWCamera>(ELAYER::UI, this)->SetTarget(player);
		if (audio != nullptr) player->SetAudio(audio);
    }
    AddManager<DWManager>(this);
    AddManager<DWColliderManager>(this);
    AddManager<DWField>(this);

}