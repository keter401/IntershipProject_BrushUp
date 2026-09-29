#pragma once

#include "Framework\gameObject.h"

class DWInGameCharacter : public DWGameObject
{
protected:
	float CurrentHealth = 0;
	float MaxHealth = 0;;
	DWVector2 MoveSpeed = DWVector2{ 0.0f, 0.0f };

	bool bIsDataModified = false;
	DWVector2 ModifyPos = DWVector2{ 0.0f, 0.0f };
	DWVector2 ModifySpeed = DWVector2{ 0.0f, 0.0f };

public:
	DWInGameCharacter() {}
		
	virtual void TakeDamaged(float damage) = 0;

	float GetHealth() const { return CurrentHealth; }
	float GetMaxHealth() const { return MaxHealth; }
	DWVector2 GetMoveSpeed() const { return MoveSpeed; }

	void SetMoveSpeed(DWVector2 speed) { MoveSpeed = speed; }
	void SetModifyPos(DWVector2 pos) { ModifyPos = pos; bIsDataModified = true; }
	void SetModifySpeed(DWVector2 speed) { ModifySpeed = speed; bIsDataModified = true; }
	void ResetHealth(float maxHealth) { MaxHealth = maxHealth; CurrentHealth = maxHealth; }
	void SetHealth(float health) { CurrentHealth = health; }
};