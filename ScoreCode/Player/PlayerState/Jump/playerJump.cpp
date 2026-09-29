#include "Player\player.h"
#include "Player\PlayerState\Jump\playerJump.h"

void DWPlayerJump::Init()
{

}

void DWPlayerJump::Enter()
{
	if (Player == nullptr) return;

	// Jump へ入るのは Idle / Move からだけ (Shot ステートが無くなったので再入はない)
	Player->PlayerJump();
}

void DWPlayerJump::Update()
{
	if (Player == nullptr) return;
	
	DWInput* input = Player->GetInput();
	if (input == nullptr) return;

	Player->GravityForce();

	DWVector2 vel = Player->GetMoveSpeed();

	if (input->GetRightButton())
	{
		vel.x = Player->GetVelocity();
		Player->SetIsFaceRight(true);
	}
	else if (input->GetLeftButton())
	{
		vel.x = -Player->GetVelocity();
		Player->SetIsFaceRight(false);
	}
	else if (!input->GetLeftButton() && !input->GetRightButton())
	{
		vel.x = 0.0f;
	}

	Player->SetMoveSpeed(vel);

	// 射撃はステート遷移ではなくアクション。撃てなければ何も起きない
	if (input->GetActionButton())
	{
		Player->TryShoot();
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