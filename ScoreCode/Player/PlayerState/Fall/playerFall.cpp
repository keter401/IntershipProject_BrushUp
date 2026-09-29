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

	// 重力と横移動を先に済ませる。
	// 以前はここでショット判定→return していたため、ボタン押しっぱなしの間
	// 重力も横移動も止まり、ShotRebound だけが効いて上昇してしまっていた。
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

	if (input->GetActionBottom() && Player->GetAmmoCount() > 0 &&
		Player->GetShootCoolDownCounter() >= Player->GetShootCoolDownFrame())
	{
		Player->SetCurrentState(DWPlayer::EPlayerState::Shot);
		return;
	}
}

void DWPlayerFall::Exit()
{

}