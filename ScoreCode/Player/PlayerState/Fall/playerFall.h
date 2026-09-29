#pragma once
#include "Framework\State\state.h"

class DWPlayer;

class DWPlayerFall : public DWState
{
private:
	DWPlayer* Player = nullptr;

public:
	DWPlayerFall(DWGameObject* owner, DWStateMachine* machine) : DWState(owner, machine)
	{
		Player = static_cast<DWPlayer*> (owner);
	}

	void Init() override;
	void Enter() override;
	void Update() override;
	void Exit() override;

};