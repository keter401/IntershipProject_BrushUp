#pragma once
#include "Framework\gameObject.h"
#include "Framework\Manager\manager.h"

#define MAX_LAYER 3

class DWSceneManager;

class DWScene
{
public:
	enum ESceneName
	{
		TITLESCENE = 0,
		GAMESCENE = 1,
		RESULTSCENE = 2,
	};

	enum ELAYER
	{
		BACKGROUND = 0,
		FIELD = 1,
		UI = 2,
	};

protected:
	std::list<DWGameObject*> GameObjectList[MAX_LAYER];
	std::list<DWManager*> ManagerList;
	DWSceneManager* SceneManager;
		
	ESceneName CurrentSceneName;

	DWInput* Input;

public:
	DWScene(DWSceneManager* sceneManager, DWInput* input) : SceneManager(sceneManager), Input(input) 
	{
		CurrentSceneName = TITLESCENE;
	}

	virtual void Init();
	void Uninit();
	void Update();
	void Draw();
	
	DWSceneManager* GetSceneManager() { return SceneManager; }
	ESceneName GetSceneName() { return CurrentSceneName; }

	// Manager
	template <typename T>
	T* AddManager(DWScene* scene)
	{
		T* managerObject = new T(scene);
		managerObject->Init();
		ManagerList.push_back(managerObject);

		return managerObject;
	}
	template <typename T>
	T* GetManager()
	{
		for (auto obj : ManagerList)
		{
			T* find = dynamic_cast<T*>(obj);
			if (find != nullptr)
			{
				return find;
			}
		}
		return nullptr;
	}

	// GameObject
	template <typename T>
	T* AddGameObject(const int Layer, DWScene* scene, DWVector2 pos = DWVector2( 0.0f, 0.0f ))
	{
		T* gameObject = new T();
		gameObject->SetScene(scene);
		gameObject->Init();
		gameObject->SetPosition(pos);
		GameObjectList[Layer].push_back(gameObject);

		return gameObject;
	}
	void AddGameObject(DWGameObject* obj, int layer);
	template <typename T>
	T* GetGameObject()
	{
		for (int i = 0; i < MAX_LAYER; i++)
		{
			if (GameObjectList[i].empty()) continue;
			for (auto obj : GameObjectList[i])
			{
				T* find = dynamic_cast<T*>(obj);
				if (find != nullptr)
				{
					return find;
				}
			}
		}
		return nullptr;
	}
	template <typename T>
	std::vector<T*> GetGameObjects()
	{
		std::vector<T*> finds;
		for (int i = 0; i < MAX_LAYER; i++)
		{
			if (GameObjectList[i].empty()) continue;
			for (auto obj : GameObjectList[i])
			{
				T* find = dynamic_cast<T*>(obj);
				if (find != nullptr)
				{
					finds.push_back(find);
				}
			}
		}
		return finds;
	}
	DWGameObject* GetGameObjectByTag(const DWGameObject::ETag tag)
	{
		for (int i = 0; i < MAX_LAYER; i++)
		{
			if (GameObjectList[i].empty()) continue;
			for (auto obj : GameObjectList[i])
			{
				if (obj->GetTag() == tag)
				{
					return obj;
				}
			}
		}
		return nullptr;
	}
	std::vector<DWGameObject*> GetGameObjectsByTag(const DWGameObject::ETag tag)
	{
		std::vector<DWGameObject*> finds;
		for (int i = 0; i < MAX_LAYER; i++)
		{
			if (GameObjectList[i].empty()) continue;
			for (auto obj : GameObjectList[i])
			{
				if (obj->GetTag() == tag)
				{
					finds.push_back(obj);
				}
			}
		}
		return finds;
	}
};