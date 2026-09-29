#pragma once
#include "Framework\gameObject.h"

class DWUI : public DWGameObject
{
private:
	const unsigned int Red = GetColor(255, 0, 0);
	const unsigned int Green = GetColor(0, 255, 0);
	const unsigned int Yellow = GetColor(255, 255, 0);
	const unsigned int White = GetColor(255, 255, 255);
	static constexpr int ShowStageNumberFrame = 180;
	static constexpr int MaxAlpha = 255;

	int PlayerHealth = 0;
	int PlayerAmmoRemain = 0;

	int StageNumber = 0;
	int StageNumberFrame = 0;
	int StageNumberAlpha = 255;

public:
	void Init() override;
	void Uninit() override;
	void Update() override;
	void DataUpdate() override {};
	void Draw() override;

	void AddStageNumber();
};
