#include "Scene\Scenes\scene.h"
#include "Scene\sceneManager.h"
#include "Camera\camera.h"

void DWScene::Uninit()
{
	// 1) 全 Manager を Uninit → 2) 全 GameObject を Uninit → 3) まとめて delete。
	// Uninit の中で他の Manager やオブジェクトを参照するもの (Field → ColliderManager など) があるので、
	// 「全部 Uninit してから delete」の 2 段階にしないと delete 済みの相手を触ってしまう
	for (auto manager : ManagerList)
	{
		if (manager != nullptr) manager->Uninit();
	}
	for (int i = 0; i < MaxLayer; ++i)
	{
		for (auto gameObject : GameObjectList[i])
		{
			if (gameObject != nullptr) gameObject->Uninit();
		}
	}

	for (auto manager : ManagerList)
	{
		delete manager;
	}
	ManagerList.clear();

	for (int i = 0; i < MaxLayer; ++i)
	{
		for (auto gameObject : GameObjectList[i])
		{
			delete gameObject;
		}
		GameObjectList[i].clear();
	}
	MainCamera = nullptr;
}

DWVector2 DWScene::GetCameraOffset() const
{
	if (MainCamera == nullptr) return DWVector2(0.0f, 0.0f);
	return MainCamera->GetOffset();
}

void DWScene::Update()
{
	for (int i = 0; i < MaxLayer; ++i)
	{
		for (auto gameObject : GameObjectList[i])
		{
			if (gameObject != nullptr) gameObject->Update();
		}
	}

	for (auto manager : ManagerList)
	{
		if (manager != nullptr)
		{
			manager->Update();
		}
	}

	for (int i = 0; i < MaxLayer; ++i)
	{
		for (auto gameObject : GameObjectList[i])
		{
			if (gameObject != nullptr)
			{
				gameObject->DataUpdate();
			}
		}
	}

	// 破棄フラグの立ったオブジェクトをフレーム末にまとめて delete
	for (int i = 0; i < MaxLayer; ++i)
	{
		GameObjectList[i].remove_if([](DWGameObject* gameObject)
		{
			return gameObject->Destroy();
		});
	}
}

void DWScene::Draw()
{
	for (int i = 0; i < MaxLayer; ++i)
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

void DWScene::AddGameObject(DWGameObject* obj, int layer, const DWVector2& pos)
{
	if (obj == nullptr) return;

	if (layer < 0 || layer >= MaxLayer)
	{
		delete obj;
		return;
	}

	obj->SetScene(this);
	obj->SetInput(Input);
	obj->SetPosition(pos);
	obj->Init();
	obj->RegisterPendingComponents();

	GameObjectList[layer].push_back(obj);
}
