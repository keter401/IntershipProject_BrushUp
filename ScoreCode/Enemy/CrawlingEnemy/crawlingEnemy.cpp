#include "Enemy\CrawlingEnemy\crawlingEnemy.h"
#include "Framework\Components\BoxCollider\BoxCollider.h"
#include "Scene\Scenes\scene.h"
#include "Framework\gameObject.h"
#include "Camera\camera.h"
#include "Player\player.h"
#include "Audio\audio.h"

void DWCrawlingEnemy::Init()
{
	bDestory = false;
	Scale = Size;
	Rotation = Rot;
	SetHealth(Health);
	Tag = DWGameObject::ETag::ENEMY;

	AddComponent<DWBoxCollider2D>(this);
	RegistPendingComponents();

	EnemyBodyColor = GetColor(255, 0, 255);

	bCanStomp = false;

	Audio = Scene->GetManager<DWAudio>();
}

void DWCrawlingEnemy::Uninit()
{

}

void DWCrawlingEnemy::Update()
{
	EnemyMove();

	for (auto& component : ComponentsList)
	{
		component->Update();
	}
}

void DWCrawlingEnemy::DataUpdate()
{
	if (bIsDataModified == true)
	{
		Position = ModifyPos;
		MoveSpeed = ModifySpeed;
		bIsDataModified = false;
	}

	for (auto& component : ComponentsList)
	{
		component->DataUpdate();
	}

}

void DWCrawlingEnemy::Draw()
{
	DWVector2 offset(0.0f, 0.0f);
	if (Scene != nullptr)
	{
		if (auto* cam = Scene->GetGameObject<DWCamera>())
		{
			offset = cam->GetOffset();
		}
	}

	int topLeftX = static_cast<int>((Position.x - Scale.x * 0.5f) - offset.x);
	int topLeftY = static_cast<int>((Position.y - Scale.y * 0.5f) - offset.y);
	int bottomRightX = static_cast<int>((Position.x + Scale.x * 0.5f) - offset.x);
	int bottomRightY = static_cast<int>((Position.y + Scale.y * 0.5f) - offset.y);

	DrawBox(topLeftX, topLeftY, bottomRightX, bottomRightY, EnemyBodyColor, true);

#ifdef _DEBUG
	const TCHAR* text;
	text = _T("’n–Ê“G");
	const unsigned int color = GetColor(255, 255, 255);

	DrawString(topLeftX, topLeftY, text, color);

	for (auto& component : ComponentsList)
	{
		component->Draw();
	}
#endif
}

void DWCrawlingEnemy::EnemyMove()
{

}

void DWCrawlingEnemy::TakeDamaged(const float damage)
{
	CurrentHealth -= damage;

	if (CurrentHealth <= 0.0f)
	{
		CurrentHealth = 0.0f;
		bDestory = true;

		if(Audio != nullptr)
		{
			Audio->PlayAudio(DWAudio::ESoundType::EnemyDead);
		}
	}
}

void DWCrawlingEnemy::OnCollisionEnter2D(const DWGameObject* other)
{
	if (other == nullptr) return;

	DWGameObject::ETag otherObjTag = other->GetTag();

	float damage = 1.0f;

	switch (otherObjTag)
	{
	case DWGameObject::BULLET:
	{
		TakeDamaged(damage);
		break;
	}
	default:
		break;
	}
}

void DWCrawlingEnemy::OnCollisionExit2D(const DWGameObject* other)
{

}