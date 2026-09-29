 #include "Scene\sceneManager.h"
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
	Input->Uninit();

	if (CurrentScene)
	{
		CurrentScene->Uninit();
		delete CurrentScene;
	}

	if (NextScene)
	{
		NextScene->Uninit();
		delete NextScene;
	}
}

void DWSceneManager::Update()
{
	Input->Update();

	CurrentScene->Update();
}

void DWSceneManager::Draw()
{
	CurrentScene->Draw();
}

void DWSceneManager::ChangeSceneProcess()
{
	if (NextScene != nullptr)
	{
		if (CurrentScene)
		{
			CurrentScene->Uninit();
			delete CurrentScene;
		}

		CurrentScene = NextScene; 
		CurrentScene->Init();

		NextScene = nullptr;
		delete NextScene;
	}
}