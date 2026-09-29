#pragma once
#include "main.h"

class DWScene;

class DWSceneManager
{
private:
	DWScene* CurrentScene = nullptr;
	DWScene* NextScene = nullptr;

	DWInput* Input = nullptr;

	void ChangeSceneProcess();

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	DWScene* GetScene() const { return CurrentScene; }

	// 遷移を予約する。実際の切替は Update() で CurrentScene->Update() が返ってから行う
	// (Scene::Update の途中で自分自身を delete しないため)
	template <typename T>
	void ChangeScene()
	{
		delete NextScene;   // 同一フレームに 2 回予約されたら後勝ち
		NextScene = new T(this, Input);
	}
};
