#pragma once

#include "Framework\gameObject.h"

class DWStateMachine;

class DWState
{
protected:
	DWGameObject* StateOwner;
	DWStateMachine* StateMachine;

public:
	DWState(DWGameObject* owner, DWStateMachine* machine) : StateOwner(owner), StateMachine(machine) {}

	virtual void Init() = 0;
	virtual void Enter() = 0;
	virtual void Update() = 0;
	virtual void Exit() = 0;

	DWGameObject* GetStateOwner() { return StateOwner; }
	DWStateMachine* GetStateMachine() { return StateMachine; }
};
