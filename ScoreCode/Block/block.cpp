#include "Block\block.h"
#include "Framework\Components\StateMachine\stateMachine.h"
#include "Framework\Components\BoxCollider\BoxCollider.h"
#include "Framework\InGameCharacter\InGameCharacter.h"
#include "Framework\Manager\ColliderManager\colliderManager.h"
#include "Scene\Scenes\Scene.h"
#include "Player\player.h"
#include <cmath>

DWBlock::DWBlock()
{
	Tag = DWGameObject::ETag::BLOCK;
	Rotation = Rot;
	Scale = Size;
	
	bBreakable = false;
	BlockColor = UnbreakableBlockColor;
}

void DWBlock::Init()
{
	AddComponent<DWBoxCollider2D>(this);
}

void DWBlock::Uninit()
{

}

void DWBlock::Update()
{
	for (auto& component : ComponentsList)
	{
		component->Update();
	}

    if(bBreakable)
    {
        BlockColor = BreakableBlockColor;
	}
    else
    {
        BlockColor = UnbreakableBlockColor;
	}
}

void DWBlock::DataUpdate()
{
	for (auto& component : ComponentsList)
	{
		component->DataUpdate();
	}
}

void DWBlock::Draw()
{
    const DWScreenRect rect = GetScreenRect();
    DrawBox(rect.left, rect.top, rect.right, rect.bottom, BlockColor, true);

#ifdef _DEBUG
    TCHAR* text;

    if (!bBreakable)
    {
        TCHAR buffer[64];
        _stprintf_s(buffer, _T("%.1f\n%.1f"), Position.x, Position.y);
        text = buffer;
    }
    else
    {
        TCHAR buffer[64];
        _stprintf_s(buffer, _T("%d"), BlockHitPoints);
        text = buffer;
    }
    const unsigned int color = GetColor(255, 255, 255);

    DrawString(
        rect.left,
        rect.top + static_cast<int>(Scale.y * 0.1f),
        text, color
    );

    for (auto& component : ComponentsList)
    {
        component->Draw();
    }
#endif
}

void DWBlock::OnCollisionEnter2D(DWGameObject* other)
{
	if (other == nullptr) return;

	switch (other->GetTag())
	{
	case DWGameObject::ETag::BULLET:
	{
		if (bBreakable)
		{
			BlockHitPoints--;
			if (BlockHitPoints <= 0)
			{
				SetDestroy();
			}
		}
		break;
	}
	default:
		break;
	}
}

void DWBlock::OnCollisionExit2D(DWGameObject* other)
{

}