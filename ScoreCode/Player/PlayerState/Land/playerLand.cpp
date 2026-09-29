#include "Player\player.h"
#include "Player\PlayerState\Land\playerLand.h"

void DWPlayerLand::Init()
{

}

void DWPlayerLand::Enter()
{
	if (Player == nullptr) return;

	Player->AmmoReload();

	DWVector2 vel = Player->GetMoveSpeed();
	vel.y = 0.0f;
	Player->SetMoveSpeed(vel);

	Player->SetCurrentState(DWPlayer::EPlayerState::Idle);
}

void DWPlayerLand::Update()
{

}

void DWPlayerLand::Exit()
{

}