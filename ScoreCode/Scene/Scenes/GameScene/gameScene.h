#pragma once

#include "Scene\Scenes\scene.h"

class DWGameScene : public DWScene
{
private:

public:
	DWGameScene(DWSceneManager* sceneManager, DWInput* input) : DWScene(sceneManager, input)
	{ 
		CurrentSceneName = DWScene::ESceneName::GAMESCENE; 
	}

	void Init() override;
};