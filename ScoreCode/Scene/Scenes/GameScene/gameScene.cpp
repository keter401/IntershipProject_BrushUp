#include "Scene\sceneManager.h"
#include "Scene\Scenes\GameScene\gameScene.h"
#include "Scene\Scenes\ResultScene\resultScene.h"
#include "Player\player.h"
#include "Framework\Manager\ColliderManager\colliderManager.h"
#include "Framework\Manager\BulletManager\bulletManager.h"
#include "Field\field.h"
#include "Camera\camera.h"
#include "UI\UI.h"
#include "Audio\audio.h"

void DWGameScene::Init()
{
	// シーンを構成するオブジェクト/マネージャはここで全部組み立てる。
	// Manager の Update 順 = 追加順 (Audio → Collider → Bullet → Field)
	AddGameObject<DWUI>(ELAYER::UI);
	DWAudio* audio = AddManager<DWAudio>();

	Player = AddGameObject<DWPlayer>(ELAYER::FIELD);
	Player->SetAudio(audio);

	DWCamera* camera = AddGameObject<DWCamera>(ELAYER::UI);
	camera->SetTarget(Player);
	SetMainCamera(camera);

	AddManager<DWColliderManager>();
	AddManager<DWBulletManager>();
	AddManager<DWField>();
}

void DWGameScene::Update()
{
	DWScene::Update();

	if (Player != nullptr && Player->GetHealth() <= 0.0f)
	{
		SceneManager->ChangeScene<DWResultScene>();
		return;
	}

#ifdef _DEBUG
	// デバッグ用: Enter / Start で即リザルトへ
	if (Input->GetKeyTrigger(KEY_INPUT_RETURN) || Input->GetPadTrigger(PAD_INPUT_START))
	{
		SceneManager->ChangeScene<DWResultScene>();
	}
#endif
}
