#include "Player\player.h"
#include "Player\PlayerState\Jump\playerJump.h"

void DWPlayerJump::Init()
{

}

void DWPlayerJump::Enter()
{
	if (Player == nullptr) return;

	DWPlayer::EPlayerState previousState = Player->GetPreviousState();

	switch (previousState)
	{
	case DWPlayer::EPlayerState::Idle:
	case DWPlayer::EPlayerState::Move:
	case DWPlayer::EPlayerState::Land:
		Player->PlayerJump();
		break;
	default:
		break;
	}
}

void DWPlayerJump::Update()
{
	if (Player == nullptr) return;
	
	DWInput* input = Player->GetInput();
	if (input == nullptr) return;

	Player->GravityForce();

	DWVector2 vel = Player->GetMoveSpeed();

	if (input->GetRightBottom())
	{
		vel.x = Player->GetVelocity();
		Player->SetIsFaceRight(true);
	}
	else if (input->GetLeftBottom())
	{
		vel.x = -Player->GetVelocity();
		Player->SetIsFaceRight(false);
	}
	else if (!input->GetLeftBottom() && !input->GetRightBottom())
	{
		vel.x = 0.0f;
	}

	Player->SetMoveSpeed(vel);

	if (input->GetActionBottom() && Player->GetAmmoCount() > 0)
	{
		if (Player->GetShootCoolDownCounter() >= Player->GetShootCoolDownFrame())
		{
			Player->SetCurrentState(DWPlayer::EPlayerState::Shot);
		}
		return;
	}

	if (vel.y >= 0.0f)
	{
		Player->SetCurrentState(DWPlayer::EPlayerState::Fall);
		return;
	}
}

void DWPlayerJump::Exit()
{

}