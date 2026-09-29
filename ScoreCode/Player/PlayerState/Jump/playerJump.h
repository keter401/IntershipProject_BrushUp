#pragma once
#include "Framework\State\state.h"

class DWPlayer;

class DWPlayerJump : public DWState
{
private:
	DWPlayer* Player = nullptr;

public:
	DWPlayerJump(DWGameObject* owner, DWStateMachine* machine) : DWState(owner, machine)
	{
		Player = static_cast<DWPlayer*> (owner);
	}

	void Init() override;
	void Enter() override;
	void Update() override;
	void Exit() override;

};