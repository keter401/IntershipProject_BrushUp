#pragma once
#include "main.h"

class DWScene;

class DWManager
{
public:
    enum EManagerTag
    {
		None = 0,
		ColliderManager = 1,
		BulletManager = 2,
    };

protected:
    DWScene* CurrentScene = nullptr;
    EManagerTag Tag = EManagerTag::None;

public:
	DWManager() = default;
	DWManager(DWScene* scene) : CurrentScene(scene) {}
	virtual ~DWManager() = default;

    virtual void Init();
    virtual void Uninit();
    virtual void Update();

    DWScene* GetCurrentScene() { return CurrentScene; }
    EManagerTag GetManagerTag() const { return Tag; }
};
