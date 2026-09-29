#pragma once
#include "Framework\gameObject.h"
#include "Framework\Manager\manager.h"

class DWSceneManager;
class DWCamera;

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

	static constexpr int MaxLayer = 3;

protected:
	// GameObject / Manager はシーンが所有し、Uninit() で delete する
	std::list<DWGameObject*> GameObjectList[MaxLayer];
	std::list<DWManager*> ManagerList;
	DWSceneManager* SceneManager;
		
	ESceneName CurrentSceneName;

	DWInput* Input;

	// 描画・当たり判定が参照するメインカメラ。毎フレーム GetGameObject<DWCamera>() で
	// 全オブジェクトを dynamic_cast で走査していたのをやめ、シーンが1つ持つ
	DWCamera* MainCamera = nullptr;

public:
	DWScene(DWSceneManager* sceneManager, DWInput* input) : SceneManager(sceneManager), Input(input) 
	{
		CurrentSceneName = TITLESCENE;
	}
	virtual ~DWScene() = default;

	// 派生シーンはオブジェクト構築 (Init)、遷移条件 (Update)、固有描画 (Draw) を override する。
	// Update / Draw を override したときは基底も呼ぶこと
	virtual void Init() {}
	virtual void Uninit();
	virtual void Update();
	virtual void Draw();
	
	DWSceneManager* GetSceneManager() { return SceneManager; }
	ESceneName GetSceneName() { return CurrentSceneName; }
	DWInput* GetInput() const { return Input; }

	// Camera
	void SetMainCamera(DWCamera* camera) { MainCamera = camera; }
	DWCamera* GetMainCamera() const { return MainCamera; }
	// メインカメラのスクロール量。カメラ未設定なら (0, 0)
	DWVector2 GetCameraOffset() const;

	// Manager
	template <typename T>
	T* AddManager()
	{
		T* managerObject = new T(this);
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
	// 生成 → シーン/入力/位置の設定 → Init → コンポーネント登録 をこの 1 経路に統一
	template <typename T>
	T* AddGameObject(const int layer, const DWVector2& pos = DWVector2(0.0f, 0.0f))
	{
		T* gameObject = new T();
		AddGameObject(gameObject, layer, pos);
		return gameObject;
	}
	void AddGameObject(DWGameObject* obj, int layer, const DWVector2& pos = DWVector2(0.0f, 0.0f));

	template <typename T>
	T* GetGameObject()
	{
		for (int i = 0; i < MaxLayer; i++)
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
		for (int i = 0; i < MaxLayer; i++)
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
		for (int i = 0; i < MaxLayer; i++)
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
		for (int i = 0; i < MaxLayer; i++)
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
