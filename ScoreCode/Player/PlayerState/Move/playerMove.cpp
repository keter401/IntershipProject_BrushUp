#include "Player\player.h"
#include "Player\PlayerState\Move\playerMove.h"
#include "Audio\audio.h"

void DWPlayerMove::Init()
{

}

void DWPlayerMove::Enter()
{

}

void DWPlayerMove::Update()
{
	if (Player == nullptr) return;

	DWInput* input = Player->GetInput();
	if (input == nullptr) return;

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

	if (!input->GetLeftBottom() && !input->GetRightBottom())
	{
		Player->SetCurrentState(DWPlayer::EPlayerState::Idle);
		return;
	}
	if (input->GetActionBottom())
	{
		Player->SetCurrentState(DWPlayer::EPlayerState::Jump);
		DWAudio* audio = Player->GetAudio();
		if(audio != nullptr)
		{
			audio->PlayAudio(DWAudio::ESoundType::PlayerJump);
		}
		return;
	}
}

void DWPlayerMove::Exit()
{

}