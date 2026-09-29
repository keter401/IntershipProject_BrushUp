#include <stdexcept>
#include <sstream>
#include "Player\player.h"
#include "Player\PlayerState\Idle\playerIdle.h"
#include "Player\PlayerState\Fall\playerFall.h"
#include "Player\PlayerState\Jump\playerJump.h"
#include "Player\PlayerState\Move\playerMove.h"
#include "Framework\Components\StateMachine\stateMachine.h"
#include "Scene\Scenes\scene.h"
#include "Framework\Manager\ColliderManager\colliderManager.h"
#include "Framework\Manager\BulletManager\bulletManager.h"
#include "Enemy\enemy.h"
#include "Audio\audio.h"

DWPlayer::DWPlayer()
{
	Rotation = Rot;
	Scale = Size;
	ResetHealth(PlayerMaxHealth);
	Tag = DWGameObject::ETag::PLAYER;
	bReusableObject = true;
}

void DWPlayer::Init()
{
	AddComponent<DWBoxCollider2D> (this);
	DWStateMachine* stateMachine = AddComponent<DWStateMachine> (this);
	
	if (stateMachine != nullptr)
	{
		PlayerStatesList.assign(EPlayerState::StateCount, nullptr);
		PlayerStatesList[EPlayerState::Idle] = new DWPlayerIdle(this, stateMachine);
		PlayerStatesList[EPlayerState::Move] = new DWPlayerMove(this, stateMachine);
		PlayerStatesList[EPlayerState::Jump] = new DWPlayerJump(this, stateMachine);
		PlayerStatesList[EPlayerState::Fall] = new DWPlayerFall(this, stateMachine);
		CurrentState = EPlayerState::Fall;
		stateMachine->ChangeState(PlayerStatesList[CurrentState]);
	}

	ShotCoolDownCounter = 0;
}

void DWPlayer::Uninit()
{
	for (DWState* state : PlayerStatesList)
	{
		delete state;
	}
	PlayerStatesList.clear();
}

void DWPlayer::Update()
{
	if (bIsInvincible)
	{
		InvincibleCounter++;

		const int step = InvincibleColorChangeRate > 0 ? InvincibleColorChangeRate : 1;

		if (bInvincibleFadeUp == false)
		{
			InvincibleColorCode -= step;
			if (InvincibleColorCode <= 0)
			{
				InvincibleColorCode = 0;
				bInvincibleFadeUp = true;
			}
		}
		else
		{
			InvincibleColorCode += step;
			if (InvincibleColorCode >= 255)
			{
				InvincibleColorCode = 255;
				bInvincibleFadeUp = false;
			}
		}

		PlayerBodyColor = GetColor(255, InvincibleColorCode, InvincibleColorCode);

		if (InvincibleCounter >= InvincibleFrame)
		{
			bIsInvincible = false;
			InvincibleCounter = 0;
			PlayerBodyColor = DefaultColor;
			InvincibleColorCode = 255;
		}
	}

	if(MoveSpeed.y >= TerminalVelocity)
	{
		MoveSpeed.y = TerminalVelocity;
	}

	PlayerMove();

	for(auto& component : ComponentsList)
	{
		component->Update();
	}

	if(CurrentState == EPlayerState::Jump || CurrentState == EPlayerState::Fall)
	{
		if (Input->GetActionButton())
		{
			ShotCoolDownCounter++;
		}
		else
		{
			ShotCoolDownCounter = 0;
		}
	}
}

void DWPlayer::DataUpdate()
{
	if(bIsDataModified == true)
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

void DWPlayer::Draw()
{
	const DWScreenRect rect = GetScreenRect();
	DrawBox(rect.left, rect.top, rect.right, rect.bottom, PlayerBodyColor, true);

#ifdef _DEBUG
    for(auto& component : ComponentsList)
    {
        component->Draw();
    }

    unsigned int textColor;
    textColor = GetColor(255, 255, 255);

	auto StateToString = [](EPlayerState state) -> const wchar_t*
		{
			switch (state)
			{
			case EPlayerState::Idle: return L"Idle";
			case EPlayerState::Move: return L"Move";
			case EPlayerState::Jump: return L"Jump";
			case EPlayerState::Fall: return L"Fall";
			default: return L"Unknown";
			}
		};

	std::wstring s = L"Current State : ";
	s += StateToString(CurrentState);
	std::string currentStateNarrowText(s.begin(), s.end());

    const TCHAR* currentStateText = nullptr;
	currentStateText = currentStateNarrowText.c_str();

	s.clear();
	s = L"Previous State : ";
	s += StateToString(PreviousState);
	std::string previousStateNarrowText(s.begin(), s.end());

    const TCHAR* previousStateText = nullptr;
	previousStateText = previousStateNarrowText.c_str();

	// UI (HP: y=10, Ammo: y=30) と重ならない位置に出す
    DrawString(10, 70, previousStateText, textColor);
    DrawString(10, 90, currentStateText, textColor);
	
	TCHAR* text;
	TCHAR buffer[64];
	_stprintf_s(buffer, _T("%.1f\n%.1f"), Position.x, Position.y);
	text = buffer;
	const unsigned int color = GetColor(0, 0, 0);
	DrawString(static_cast<int> (Position.x - Scale.x * 0.45f), static_cast<int> (Position.y - Scale.y * 0.4f), text, color);

	text[0] = '\0';
	buffer[0] = '\0';
	_stprintf_s(buffer, _T("Ammo : %d"), AmmoCount);
	text = buffer;
	DrawString(10, 130, text, textColor);

#endif
}

void DWPlayer::SetCurrentState(const EPlayerState State)
{
	PreviousState = CurrentState;
	CurrentState = State;
	
	DWStateMachine* stateMachine = GetComponent<DWStateMachine>();
	stateMachine->ChangeState(PlayerStatesList[CurrentState]);
}

void DWPlayer::PlayerMove()
{
	Position += MoveSpeed;
}

void DWPlayer::PlayerJump()
{
	MoveSpeed.y = -JumpPower;
}

void DWPlayer::PlayerShot()
{
	DWBulletManager* bulletMgr = Scene->GetManager<DWBulletManager>();
	if (bulletMgr == nullptr) return;
	if (AmmoCount <= 0) return;

	ShotCoolDownCounter = 0;

	bulletMgr->Spawn(Position);
	AmmoCount -= 1;
}

bool DWPlayer::TryShoot()
{
	if (AmmoCount <= 0) return false;
	if (ShotCoolDownCounter < ShotCoolDownFrame) return false;

	PlayerShot();
	ShotRebound();

	if (Audio != nullptr)
	{
		Audio->PlayAudio(DWAudio::ESoundType::PlayerShoot);
	}
	return true;
}

void DWPlayer::OnLanded()
{
	AmmoReload();
	MoveSpeed.y = 0.0f;
	SetCurrentState(EPlayerState::Idle);
}

void DWPlayer::GravityForce()
{
	MoveSpeed.y += DefaultGravity;
}

void DWPlayer::ShotRebound()
{
	MoveSpeed.y -= ShotReboundPower;
}

void DWPlayer::TreadRebound()
{
	MoveSpeed.y -= TreadReboundPower;
}

void DWPlayer::AmmoReload()
{
	AmmoCount = AmmoMax;
}

void DWPlayer::TakeDamaged(float damage)
{
	if (damage < 0.0f) return;
	if (bIsInvincible) return;

	CurrentHealth -= damage;
	bIsInvincible = true;

	if (CurrentHealth < 0.0f)
	{
		CurrentHealth = 0.0f;
	}

	if(Audio != nullptr)
	{
		Audio->PlayAudio(DWAudio::ESoundType::PlayerDamaged);
	}
}

void DWPlayer::DamagedByOther(float damage, const DWGameObject* other)
{
	if (damage < 0.0f) return;
	if (other == nullptr) return;
	if (bIsInvincible) return;

	// 相手から離れる方向へノックバックしてから、通常のダメージ処理
	TreadRebound();

	if (other->GetPosition().x > Position.x)	MoveSpeed.x -= DamagedReboundPower;
	else										MoveSpeed.x += DamagedReboundPower;

	TakeDamaged(damage);
}

void DWPlayer::OnCollisionEnter2D(DWGameObject* other)
{
	HandleContact(other);
}

void DWPlayer::OnCollisionStay2D(DWGameObject* other)
{
	HandleContact(other);
}

void DWPlayer::HandleContact(DWGameObject* other)
{
	if (other == nullptr) return;

	switch (other->GetTag())
	{
		case DWGameObject::ETag::BLOCK:
		{
	 		PushBack(other);
			break;
		}
		case DWGameObject::ETag::ENEMY:
		{
			DWEnemy* enemy = static_cast<DWEnemy*>(other);

			if (enemy->CanStomp())
			{
				DWBoxCollider2D* boxCollider = GetComponent<DWBoxCollider2D>();
				if (boxCollider == nullptr) return;
				DWBoxCollider2D* enemyBoxCollider = enemy->GetComponent<DWBoxCollider2D>();
				if (enemyBoxCollider == nullptr) return;

				if (enemyBoxCollider->IsStomped(boxCollider))
				{
					TreadRebound();
					
					float stompDamage = 3.0f;
					enemy->TakeDamaged(stompDamage);
				}
				else
				{
					float damage = 1.0f;
					DamagedByOther(damage, other);
				}
			}
			else
			{
				float damage = 1.0f;
				DamagedByOther(damage, other);
			}
			break;
		}
		default:
			break;
	}
}

void DWPlayer::OnCollisionExit2D(DWGameObject* other)
{
	if (other == nullptr) return;

	switch (other->GetTag())
	{
	case DWGameObject::ETag::BLOCK:
	{
		if (Scene) 
		{
			auto* colliderManager = Scene->GetManager<DWColliderManager>();
			if(colliderManager == nullptr) return;
			const bool stillOnBlock = (colliderManager && colliderManager->IsCollidingWithTagThisFrame(this, DWGameObject::ETag::BLOCK));
			if (!stillOnBlock) 
			{
				if (GetCurrentState() != EPlayerState::Jump) 
				{
					SetCurrentState(EPlayerState::Fall);
				}
			}
		}
		break;
	}
	case DWGameObject::ETag::ENEMY:
	{
		PlayerBodyColor = GetColor(255, 255, 255);

		break;
	}
	default:
		break;
	}
}

void DWPlayer::PushBack(const DWGameObject* other)
{
	switch (ResolvePenetration(other))
	{
	case EPushBackSide::Left:
	case EPushBackSide::Right:
		MoveSpeed.x = 0.0f;
		break;

	case EPushBackSide::Bottom:
		// 頭をぶつけた
		MoveSpeed.y = 0.0f;
		break;

	case EPushBackSide::Top:
		// 上から乗った (上昇中に側面をかすめただけなら着地扱いにしない)
		if (MoveSpeed.y >= 0.0f)
		{
			OnLanded();
		}
		break;

	default:
		break;
	}
}
