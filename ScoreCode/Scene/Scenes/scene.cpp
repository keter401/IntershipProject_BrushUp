#include "Scene\Scenes\scene.h"
#include "Scene\sceneManager.h"
#include "Scene\Scenes\TitleScene\titleScene.h"
#include "Scene\Scenes\GameScene\gameScene.h"
#include "Scene\Scenes\ResultScene\resultScene.h"
#include "Player\player.h"

void DWScene::Init()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		GameObjectList[i].clear();
	}

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		if (!GameObjectList[i].empty())
		{
			for (auto gameObject : GameObjectList[i])
			{
				if (gameObject != nullptr)
				{
					gameObject->Init();
					gameObject->SetInput(Input);
				}
			}
		}
	}
}

void DWScene::Uninit()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		for (auto gameObject : GameObjectList[i])
		{
			if (gameObject != nullptr)
			{
				gameObject->Uninit();
				delete gameObject;
			}
		}
		GameObjectList[i].clear();
	}
}

void DWScene::Update()
{
	for (int i = 0; i < MAX_LAYER; ++i)
	{
		for (auto gameObject : GameObjectList[i])
		{
			if (gameObject != nullptr) gameObject->Update();
		}
	}

	if (!ManagerList.empty())
	{
		for (auto manager : ManagerList)
		{
			if (manager != nullptr)
			{
				manager->Update();
			}
		}
	}
	else
	{
		int i = 0;
	}

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		for (auto gameObject : GameObjectList[i])
		{
			if (gameObject != nullptr)
			{
				gameObject->DataUpdate();
			}
		}
	}

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		GameObjectList[i].remove_if([](DWGameObject* gameObject)
		{
			return gameObject->Destroy();
		});
	}

	if(Input->GetKeyTrigger(KEY_INPUT_RETURN) || Input->GetPadTrigger(PAD_INPUT_START))
	{
		if (CurrentSceneName == DWScene::TITLESCENE) SceneManager->ChangeScene<DWGameScene> ();
		else if (CurrentSceneName == DWScene::GAMESCENE) SceneManager->ChangeScene<DWResultScene> ();
		else if (CurrentSceneName == DWScene::RESULTSCENE) SceneManager->ChangeScene<DWTitleScene> ();
	}

	if (CurrentSceneName == DWScene::GAMESCENE)
	{
		if (!GameObjectList[ELAYER::FIELD].empty())
		{
			DWPlayer* player = nullptr;
			for (auto gameObject : GameObjectList[ELAYER::FIELD])
			{
				if (gameObject != nullptr)
				{
					if (gameObject->GetTag() == DWGameObject::ETag::PLAYER)
					{
						player = static_cast<DWPlayer*>(gameObject);
						break;
					}
				}
			}
			if (player != nullptr)
			{
				if (player->GetHealth() <= 0)
				{
					SceneManager->ChangeScene<DWResultScene>();
				}
			}
		}
	}
}

void DWScene::Draw()
{
	{// ï∂éöóÒÇÃï`âÊÅAå„Ç≈çÌèúÇ∑ÇÈó\íË
		unsigned int color;
		color = GetColor(255, 255, 255);

		const TCHAR* text = nullptr;

		switch (CurrentSceneName)
		{
		case DWScene::TITLESCENE:
			text = _T("TitleScene");
			break;
		case DWScene::GAMESCENE:
			text = _T(" ");
			break;
		case DWScene::RESULTSCENE:
			text = _T("ResultScene");
			break;
		default:
			text = _T("UnknownScene");
			break;
		}

		DrawString(640, 360, text, color);
	}

	for (int i = 0; i < MAX_LAYER; ++i)
	{
		for (auto gameObject : GameObjectList[i])
		{
			if (gameObject != nullptr)
			{
				gameObject->Draw();
			}
		}
	}
}

void DWScene::AddGameObject(DWGameObject* obj, int layer)
{
	if (obj == nullptr) return;

	if (layer < 0 || layer >= MAX_LAYER) return;

	GameObjectList[layer].push_back(obj);
	obj->SetScene(this);
	obj->RegistPendingComponents();
}

