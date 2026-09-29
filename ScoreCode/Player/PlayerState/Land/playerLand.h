#pragma once
#include "Framework\State\state.h"

class DWPlayer;

class DWPlayerLand : public DWState
{
private:
	DWPlayer* Player = nullptr;

public:
	DWPlayerLand(DWGameObject* owner, DWStateMachine* machine) : DWState(owner, machine)
	{
		Player = static_cast<DWPlayer*> (owner);
	}

	void Init() override;
	void Enter() override;
	void Update() override;
	void Exit() override;
};