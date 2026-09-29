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
	virtual ~DWState() = default;

	// •K—v‚È‚à‚Ì‚¾‚¯ override ‚·‚ê‚Î‚æ‚¢
	virtual void Init() {}
	virtual void Enter() {}
	virtual void Update() {}
	virtual void Exit() {}

	DWGameObject* GetStateOwner() { return StateOwner; }
	DWStateMachine* GetStateMachine() { return StateMachine; }
};
