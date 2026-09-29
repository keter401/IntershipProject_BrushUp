#include "Framework\gameObject.h"
#include "Scene\Scenes\scene.h"

DWGameObject::~DWGameObject()
{
	// Uninit ¨ delete ‚Ì‡‚ÅA“o˜^Ï‚ÝE“o˜^‘Ò‚¿‚Ì—¼•û‚ð‰ð•ú‚·‚é
	for (DWComponent* component : ComponentsList)
	{
		component->Uninit();
		delete component;
	}
	ComponentsList.clear();

	for (DWComponent* component : PendingComponentsList)
	{
		delete component;
	}
	PendingComponentsList.clear();
}

DWVector2 DWGameObject::GetCameraOffset() const
{
	if (Scene == nullptr) return DWVector2(0.0f, 0.0f);
	return Scene->GetCameraOffset();
}

DWScreenRect DWGameObject::GetScreenRect() const
{
	const DWVector2 offset = GetCameraOffset();
	const DWVector2 half = Scale * 0.5f;

	DWScreenRect rect;
	rect.left   = static_cast<int>((Position.x - half.x) - offset.x);
	rect.top    = static_cast<int>((Position.y - half.y) - offset.y);
	rect.right  = static_cast<int>((Position.x + half.x) - offset.x);
	rect.bottom = static_cast<int>((Position.y + half.y) - offset.y);
	return rect;
}

bool DWGameObject::Destroy()
{
	if (bDestroy)
	{
		Uninit();
		delete this;
		return true;
	}
	else
	{
		return false;
	}
}

void DWGameObject::RegisterPendingComponents()
{
	//’Ç‰Á‘Ò‚¿‚ÌƒRƒ“ƒ|[ƒlƒ“ƒg‚ð“o˜^
	for (auto component : PendingComponentsList)
	{
		ComponentsList.push_back(component);
	}

	//ƒRƒ“ƒ|[ƒlƒ“ƒg‚ð‚·‚×‚Ä‰Šú‰»
	for (auto component : PendingComponentsList)
	{
		component->Init();
	}

	PendingComponentsList.clear();
}
