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
}

void DWPlayerFall::Exit()
{

}