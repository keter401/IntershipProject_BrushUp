#include "Player\player.h"
#include "Player\PlayerState\Shot\playerShot.h"
#include "Framework\Manager\BulletManager\bulletManager.h"
#include "Audio\audio.h"

void DWPlayerShot::Init()
{

}

void DWPlayerShot::Enter()
{
	if (Player == nullptr) return;
	DWInput* input = Player->GetInput();
	if (input == nullptr) return;
	
	if (Player->GetAmmoCount() > 0)
	{
		Player->PlayerShot();
		Player->ShotRebound();

		DWAudio* audio = Player->GetAudio();
		if (audio != nullptr)
		{
			audio->PlayAudio(DWAudio::ESoundType::PlayerShoot);
		}
	}

	DWPlayer::EPlayerState previousState = Player->GetPreviousState();
	Player->SetCurrentState(previousState);
}

void DWPlayerShot::Update()
{

}

void DWPlayerShot::Exit()
{

}