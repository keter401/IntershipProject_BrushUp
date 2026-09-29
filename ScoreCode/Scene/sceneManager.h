#pragma once
#include "main.h"

class DWSceneManager
{
private:
	class DWScene* CurrentScene = nullptr;
	class DWScene* NextScene = nullptr;

	DWInput* Input = nullptr;

	void ChangeSceneProcess();

public:
	void Init();
	void Uninit();
	void Update();
	void Draw();

	DWScene* GetScene() const { return CurrentScene; }

    template <typename T>
    void ChangeScene()
    {
        NextScene = new T(this, Input);
		ChangeSceneProcess();
    }
};