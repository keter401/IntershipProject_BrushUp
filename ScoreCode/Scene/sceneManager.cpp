#include "Scene\sceneManager.h"
#include "Scene\Scenes\scene.h"
#include "Scene\Scenes\TitleScene\titleScene.h"

void DWSceneManager::Init()
{
	Input = new DWInput();
	Input->Init();

	ChangeScene<DWTitleScene>();
	ChangeSceneProcess();
}

void DWSceneManager::Uninit()
{
	if (CurrentScene)
	{
		CurrentScene->Uninit();
		delete CurrentScene;
		CurrentScene = nullptr;
	}

	if (NextScene)
	{
		// Init 前のシーンなので Uninit は不要
		delete NextScene;
		NextScene = nullptr;
	}

	if (Input)
	{
		Input->Uninit();
		delete Input;
		Input = nullptr;
	}
}

void DWSceneManager::Update()
{
	Input->Update();

	if (CurrentScene) CurrentScene->Update();

	ChangeSceneProcess();
}

void DWSceneManager::Draw()
{
	if (CurrentScene) CurrentScene->Draw();
}

void DWSceneManager::ChangeSceneProcess()
{
	if (NextScene == nullptr) return;

	if (CurrentScene)
	{
		CurrentScene->Uninit();
		delete CurrentScene;
	}

	CurrentScene = NextScene;
	NextScene = nullptr;

	CurrentScene->Init();
}
