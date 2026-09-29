#include "Player\player.h"
#include "Player\PlayerState\Fall\playerFall.h"

void DWPlayerFall::Init()
{

}

void DWPlayerFall::Enter()
{

}

void DWPlayerFall::Update()
{
	if (Player == nullptr) return;

	DWInput* input = Player->GetInput();
	if (input == nullptr) return;

	if (input->GetActionBottom() && Player->GetAmmoCount() > 0)
	{
		if (Player->GetShootCoolDownCounter() >= Player->GetShootCoolDownFrame())
			Player->SetCurrentState(DWPlayer::EPlayerState::Shot);
		return;
	}

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
}

void DWPlayerFall::Exit()
{

}