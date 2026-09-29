#pragma once

#include "Scene\Scenes\scene.h"

class DWTitleScene : public DWScene
{
public:
	DWTitleScene(DWSceneManager* sceneManager, DWInput* input) : DWScene(sceneManager, input) 
	{ 
		CurrentSceneName = DWScene::ESceneName::TITLESCENE; 
	}

	void Init() override;
	void Update() override;
	void Draw() override;
};
