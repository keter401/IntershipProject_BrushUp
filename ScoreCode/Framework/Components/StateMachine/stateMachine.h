#pragma once
#include "main.h"
#include "Framework\Components\component.h"

class DWState;

class DWStateMachine : public DWComponent
{
private:
	DWState* CurrentState = nullptr;

public:
	DWStateMachine(DWGameObject* owner) : DWComponent(owner) {}

	void Init() override;
	void Uninit() override;
	void Update() override;

	void ChangeState(DWState* state);

	DWState* GetCurrentState() const { return CurrentState; }
};