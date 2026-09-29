#include "Scene\sceneManager.h"
#include "Scene\Scenes\TitleScene\titleScene.h"
#include "Scene\Scenes\GameScene\gameScene.h"

void DWTitleScene::Init()
{

}

void DWTitleScene::Update()
{
	DWScene::Update();

	if (Input->GetKeyTrigger(KEY_INPUT_RETURN) || Input->GetPadTrigger(PAD_INPUT_START))
	{
		SceneManager->ChangeScene<DWGameScene>();
	}
}

void DWTitleScene::Draw()
{
	DWScene::Draw();

	// 仮表示。タイトル画面を作るときに差し替える
	DrawString(640, 360, _T("TitleScene"), GetColor(255, 255, 255));
}
