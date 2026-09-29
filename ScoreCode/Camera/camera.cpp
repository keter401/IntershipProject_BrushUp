#include "Camera\camera.h"
#include "Scene\sceneManager.h"
#include "Scene\Scenes\scene.h"
#include "Framework\gameObject.h"

void DWCamera::Init()
{
    Offset = DWVector2(0.0f, 0.0f);
    bReuseableObject = true;
}

void DWCamera::Uninit()
{
    Target = nullptr;
}

void DWCamera::Update()
{
    if (Target == nullptr) return;

    const DWVector2 targetPos = Target->GetPosition();

	float screenOffsetX = -SCREEN_WIDTH * 0.15f;
	float halfScreenHeight = SCREEN_HEIGHT * 0.5f;

    Offset.x = screenOffsetX;
    Offset.y = targetPos.y - halfScreenHeight;
}
