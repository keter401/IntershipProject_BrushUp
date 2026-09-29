#include "Player\player.h"
#include "Player\PlayerState\Idle\playerIdle.h"
#include "Audio\audio.h"

void DWPlayerIdle::Init()
{

}

void DWPlayerIdle::Enter()
{

}

void DWPlayerIdle::Update()
{
	if (Player == nullptr) return;

	DWInput* input = Player->GetInput();
	if (input == nullptr) return;

	if (input->GetLeftButton() || input->GetRightButton())
	{
		Player->SetCurrentState(DWPlayer::EPlayerState::Move);
		return;
	}
	else
	{
		Player->SetMoveSpeed(DWVector2(0.0f, Player->GetMoveSpeed().y));
	}

	if (input->GetActionButton())
	{
		Player->SetCurrentState(DWPlayer::EPlayerState::Jump);
		DWAudio* audio = Player->GetAudio();
		if (audio != nullptr)
		{
			audio->PlayAudio(DWAudio::ESoundType::PlayerJump);
		}
		return;
	}
	
	// —Ž‰º”»’f‚Ìˆ—
	if (Player->GetMoveSpeed().y > 0.0f)
	{
		Player->SetCurrentState(DWPlayer::EPlayerState::Fall);
		return;
	}
}

void DWPlayerIdle::Exit()
{

}