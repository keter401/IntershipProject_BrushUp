#include "Enemy\FloatingEnemy\floatingEnemy.h"
#include "Framework\Components\BoxCollider\BoxCollider.h"
#include "Scene\Scenes\scene.h"
#include "Framework\gameObject.h"
#include "Audio\audio.h"

void DWFloatingEnemy::Init()
{
	bDestroy = false;
	Scale = Size;
	Rotation = Rot;
	ResetHealth(InitialHealth);
	Tag = DWGameObject::ETag::ENEMY;

	AddComponent<DWBoxCollider2D>(this);

	EnemyBodyColor = GetColor(0, 255, 255);

	bCanStomp = true;

	Audio = Scene->GetManager<DWAudio>();
}

void DWFloatingEnemy::Uninit()
{

}

void DWFloatingEnemy::Update()
{
	// ˆê“x‰æ–Ê“à‚É“ü‚Á‚½‚ç“®‚«o‚·
	if (!bCanMove && Scene != nullptr && Scene->GetMainCamera() != nullptr)
	{
		const DWVector2 offset = GetCameraOffset();

		const float screenLeft = offset.x;
		const float screenRight = offset.x + SCREEN_WIDTH;
		const float screenTop = offset.y;
		const float screenBottom = offset.y + SCREEN_HEIGHT;

		if (Position.x + Scale.x * 0.5f > screenLeft &&
			Position.x - Scale.x * 0.5f < screenRight &&
			Position.y + Scale.y * 0.5f > screenTop &&
			Position.y - Scale.y * 0.5f < screenBottom)
		{
			bCanMove = true;
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
	const DWScreenRect rect = GetScreenRect();
	DrawBox(rect.left, rect.top, rect.right, rect.bottom, EnemyBodyColor, true);

#ifdef _DEBUG
	const TCHAR* text;
	text = _T("•‚—V“G");
	const unsigned int color = GetColor(255, 255, 255);

	DrawString(rect.left, rect.top, text, color);

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
		bDestroy = true;

		if (Audio != nullptr)
		{
			Audio->PlayAudio(DWAudio::ESoundType::EnemyDead);
		}
	}
}

void DWFloatingEnemy::OnCollisionEnter2D(DWGameObject* other)
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

void DWFloatingEnemy::OnCollisionStay2D(DWGameObject* other)
{
	if (other == nullptr) return;

	// ‰Ÿ‚µ–ß‚µ‚¾‚¯‚ÍÚG’†‚¸‚Á‚Æ•K—vB’eƒ_ƒ[ƒW‚Í Enter ‚Ì1‰ñ‚¾‚¯
	if (other->GetTag() == DWGameObject::ETag::BLOCK)
	{
		PushBack(other);
	}
}

void DWFloatingEnemy::OnCollisionExit2D(DWGameObject* other)
{

}


void DWFloatingEnemy::PushBack(const DWGameObject* other)
{
	switch (ResolvePenetration(other))
	{
	case EPushBackSide::Left:
	case EPushBackSide::Right:
		MoveSpeed.x = 0.0f;
		break;

	case EPushBackSide::Top:
	case EPushBackSide::Bottom:
		MoveSpeed.y = 0.0f;
		break;

	default:
		break;
	}
}
