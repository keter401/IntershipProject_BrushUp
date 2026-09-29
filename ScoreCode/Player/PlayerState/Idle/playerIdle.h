#pragma once
#include "Framework\State\state.h"
#include "Player\player.h"

class DWPlayer;

class DWPlayerIdle : public DWState
{
private:
	std::string StateName = "Idle";

	DWPlayer* Player = nullptr;

public:
	DWPlayerIdle(DWGameObject* owner, DWStateMachine* machine) : DWState(owner, machine)
	{
		Player = static_cast<DWPlayer*> (owner);
	}

	void Init() override;
	void Enter() override;
	void Update() override;
	void Exit() override;
};