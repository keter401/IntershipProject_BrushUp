#include "Enemy\FloatingEnemy\floatingEnemy.h"
#include "Framework\Components\BoxCollider\BoxCollider.h"
#include "Scene\Scenes\scene.h"
#include "Framework\gameObject.h"
#include "Camera\camera.h"
#include "Audio\audio.h"

void DWFloatingEnemy::Init()
{
	bDestory = false;
	Scale = Size;
	Rotation = Rot;
	SetHealth(Health);
	Tag = DWGameObject::ETag::ENEMY;

	AddComponent<DWBoxCollider2D>(this);
	RegistPendingComponents();

	EnemyBodyColor = GetColor(0, 255, 255);

	bCanStomp = true;

	Audio = Scene->GetManager<DWAudio>();
}

void DWFloatingEnemy::Uninit()
{

}

void DWFloatingEnemy::Update()
{
	DWVector2 offset(0.0f, 0.0f);
	if (Scene != nullptr)
	{
		if (auto* cam = Scene->GetGameObject<DWCamera>())
		{
			offset = cam->GetOffset();

			float screenLeft = offset.x;
			float screenRight = offset.x + SCREEN_WIDTH;
			float screenTop = offset.y;
			float screenBottom = offset.y + SCREEN_HEIGHT;

			if (Position.x + Scale.x * 0.5f > screenLeft &&
				Position.x - Scale.x * 0.5f < screenRight &&
				Position.y + Scale.y * 0.5f > screenTop &&
				Position.y - Scale.y * 0.5f < screenBottom)
			{
				bCanMove = true;
			}
		}
	}

	if (bCanMove == false)	return;

	EnemyMove();

	for (auto& component : ComponentsList)
	{
		component->Update();
	}

}

void DWFloatingEnemy::DataUpdate()
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

void DWFloatingEnemy::Draw()
{
	DWVector2 offset(0.0f, 0.0f);
	if (Scene != nullptr)
	{
		if (auto* cam = Scene->GetGameObject<DWCamera>())
		{
			offset = cam->GetOffset();
		}
	}

	topLeftX = static_cast<int>((Position.x - Scale.x * 0.5f) - offset.x);
	topLeftY = static_cast<int>((Position.y - Scale.y * 0.5f) - offset.y);
	bottomRightX = static_cast<int>((Position.x + Scale.x * 0.5f) - offset.x);
	bottomRightY = static_cast<int>((Position.y + Scale.y * 0.5f) - offset.y);

	DrawBox(topLeftX, topLeftY, bottomRightX, bottomRightY, EnemyBodyColor, true);

#ifdef _DEBUG
	const TCHAR* text;
	text = _T("•‚—V“G");
	const unsigned int color = GetColor(255, 255, 255);

	DrawString(topLeftX, topLeftY, text, color);

	for (auto& component : ComponentsList)
	{
		component->Draw();
	}
#endif
}

void DWFloatingEnemy::EnemyMove()
{
	DWScene* scene = GetScene();
	if (scene == nullptr) return;

	DWGameObject* player = scene->GetGameObjectByTag(DWGameObject::ETag::PLAYER);
	if (player == nullptr) return;

	DWVector2 playerPos = player->GetPosition();
	DWVector2 myPos = GetPosition();

	DWVector2 direction = playerPos - myPos;
	direction.normalize();

	myPos += direction * Velocity;

	SetPosition(myPos);
}

void DWFloatingEnemy::TakeDamaged(const float damage)
{
	CurrentHealth -= damage;

	if(CurrentHealth <= 0.0f)
	{
		CurrentHealth = 0.0f;
		bDestory = true;

		if (Audio != nullptr)
		{
			Audio->PlayAudio(DWAudio::ESoundType::EnemyDead);
		}
	}
}

void DWFloatingEnemy::OnCollisionEnter2D(const DWGameObject* other)
{
	if (other == nullptr) return;

	float damage;
	switch (other->GetTag())
	{
	case DWGameObject::ETag::BLOCK:
		PushBack(other);
		break;
	case DWGameObject::ETag::PLAYER:
		break;
	case DWGameObject::ETag::BULLET:
		damage = 1.0f;
		TakeDamaged(damage);
		break;
	default:
		break;
	}
}

void DWFloatingEnemy::OnCollisionExit2D(const DWGameObject* other)
{

}


void DWFloatingEnemy::PushBack(const DWGameObject* other)
{
	if (other == nullptr) return;

	DWVector2 objPos = other->GetPosition();
	const DWVector2 objHalfScale = other->GetScale() * 0.5f;
	const DWVector2 halfSize = Scale * 0.5f;

	const float distanceX = objPos.x - Position.x;
	const float distanceY = objPos.y - Position.y;

	const float px = (halfSize.x + objHalfScale.x) - std::abs(distanceX);
	const float py = (halfSize.y + objHalfScale.y) - std::abs(distanceY);

	if (px <= 0.0f || py <= 0.0f) return;

	if (px < py)
	{
		if (distanceX < 0.0f)
		{
			Position.x = other->GetPosition().x + (objHalfScale.x + halfSize.x);
		}
		else
		{
			Position.x = other->GetPosition().x - (objHalfScale.x + halfSize.x);
		}
		MoveSpeed.x = 0.0f;
	}
	else
	{
		if (distanceY < 0.0f)
		{
			Position.y = other->GetPosition().y + (objHalfScale.y + halfSize.y);
		}
		else
		{
			Position.y = other->GetPosition().y - (objHalfScale.y + halfSize.y);
		}
		MoveSpeed.y = 0.0f;
	}
}