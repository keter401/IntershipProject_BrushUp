#pragma once
#include "Framework\InGameCharacter\InGameCharacter.h"
#include "Framework\State\state.h"

class DWEnemy : public DWInGameCharacter
{
protected:
	unsigned int EnemyBodyColor = GetColor(0, 0, 0);

	bool bCanStomp = false;

public:
	DWEnemy();

	virtual void EnemyMove() = 0;

	virtual void Init() = 0;
	virtual void Uninit() = 0;
	virtual void Update() = 0;
	virtual void Draw() = 0;

	virtual void EnemyDamaged() = 0;

	bool CanStomp() { return bCanStomp; }
};