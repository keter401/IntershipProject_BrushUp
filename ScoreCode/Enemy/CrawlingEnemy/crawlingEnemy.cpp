#include "Enemy\CrawlingEnemy\crawlingEnemy.h"
#include "Framework\Components\BoxCollider\BoxCollider.h"
#include "Scene\Scenes\scene.h"
#include "Framework\gameObject.h"
#include "Player\player.h"
#include "Audio\audio.h"

void DWCrawlingEnemy::Init()
{
	bDestroy = false;
	Scale = Size;
	Rotation = Rot;
	ResetHealth(InitialHealth);
	Tag = DWGameObject::ETag::ENEMY;

	AddComponent<DWBoxCollider2D>(this);

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
	const DWScreenRect rect = GetScreenRect();
	DrawBox(rect.left, rect.top, rect.right, rect.bottom, EnemyBodyColor, true);

#ifdef _DEBUG
	const TCHAR* text;
	text = _T("’n–Ê“G");
	const unsigned int color = GetColor(255, 255, 255);

	DrawString(rect.left, rect.top, text, color);

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
		bDestroy = true;

		if(Audio != nullptr)
		{
			Audio->PlayAudio(DWAudio::ESoundType::EnemyDead);
		}
	}
}

void DWCrawlingEnemy::OnCollisionEnter2D(DWGameObject* other)
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

void DWCrawlingEnemy::OnCollisionExit2D(DWGameObject* other)
{

}