#include "Framework\Components\StateMachine\stateMachine.h"
#include "Framework\State\state.h"
#include <stdexcept>

void DWStateMachine::Init()
{

}

void DWStateMachine::Uninit()
{

}

void DWStateMachine::Update()
{
	if (CurrentState)
	{
		CurrentState->Update();
	}
}

void DWStateMachine::ChangeState(DWState* state)
{
	if (state == nullptr) return;

	if (CurrentState)
	{
		CurrentState->Exit();
	}

	CurrentState = state;
	CurrentState->Enter();
}