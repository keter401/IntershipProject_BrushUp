#include "UI.h"
#include "Player\player.h"
#include "Scene\Scenes\scene.h"

void DWUI::Init()
{
	bReuseableObject = true;
	StageNumber = 1;
}

void DWUI::Uninit()
{

}

void DWUI::Update()
{
	DWPlayer* player = Scene->GetGameObject<DWPlayer>();
	if (player == nullptr) return;

	PlayerHealth = static_cast<int>(player->GetHealth());
	PlayerAmmoRemain = player->GetAmmoCount();

    if (StageNumberFrame <= ShowStageNumberFrame)
    {
	    StageNumberFrame++;
        StageNumberAlpha = MaxAlpha - (StageNumberFrame * (MaxAlpha / ShowStageNumberFrame));
    }
    else 
    {
        StageNumberAlpha = 0;
	}
}

void DWUI::Draw()
{
	{//Player Health
        std::string text = "HealthPoint : ";
        for (int i = 0; i < PlayerHealth; i++)
        {
            text += "■";
        }

        unsigned int HealthUIColor;
        if (PlayerHealth > 3)
        {
            HealthUIColor = Green;
        }
        else if (PlayerHealth > 1 && PlayerHealth <= 3)
        {
            HealthUIColor = Yellow;
        }
        else
        {
            HealthUIColor = Red;
        }

        DrawString(10, 10, text.c_str(), HealthUIColor);
    }

    {//Player Ammo
        std::string text = "Ammo : ";
        for (int i = 0; i < PlayerAmmoRemain; i++)
        {
            text += "●";
        }

		DrawString(10, 30, text.c_str(), Yellow);
    }

    {//ステージ
        std::string text = "Stage ";
		text += std::to_string(StageNumber);

        SetDrawBlendMode(DX_BLENDMODE_ALPHA, StageNumberAlpha);
		DrawString(static_cast<int>(SCREEN_WIDTH * 0.5f), 10, text.c_str(), White);
        SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
    }
}

void DWUI::AddStageNumber()
{
    StageNumber++;
	StageNumberFrame = 0;
	StageNumberAlpha = MaxAlpha;
}
