#pragma once

#include "Scene\Scenes\scene.h"

class DWResultScene : public DWScene
{
public:
	DWResultScene(DWSceneManager* sceneManager, DWInput* input) : DWScene(sceneManager, input) 
	{ 
		CurrentSceneName = DWScene::ESceneName::RESULTSCENE; 
	}

	void Init() override;
};