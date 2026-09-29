#include "Scene\sceneManager.h"
#include "Scene\Scenes\ResultScene\resultScene.h"
#include "Scene\Scenes\TitleScene\titleScene.h"

void DWResultScene::Init()
{

}

void DWResultScene::Update()
{
	DWScene::Update();

	if (Input->GetKeyTrigger(KEY_INPUT_RETURN) || Input->GetPadTrigger(PAD_INPUT_START))
	{
		SceneManager->ChangeScene<DWTitleScene>();
	}
}

void DWResultScene::Draw()
{
	DWScene::Draw();

	// 仮表示。リザルト画面を作るときに差し替える
	DrawString(640, 360, _T("ResultScene"), GetColor(255, 255, 255));
}
