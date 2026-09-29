#pragma once

#include "Scene\Scenes\scene.h"

class DWPlayer;

class DWGameScene : public DWScene
{
private:
	// Init で生成。ステージ再生成でも破棄されない (bReusableObject) のでポインタを保持してよい
	DWPlayer* Player = nullptr;

public:
	DWGameScene(DWSceneManager* sceneManager, DWInput* input) : DWScene(sceneManager, input)
	{ 
		CurrentSceneName = DWScene::ESceneName::GAMESCENE; 
	}

	void Init() override;
	void Update() override;

	DWPlayer* GetPlayer() const { return Player; }
};
