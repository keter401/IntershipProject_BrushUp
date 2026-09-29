#pragma once

#include "Framework\gameObject.h"

class DWInGameCharacter : public DWGameObject
{
public:
	// ResolvePenetration の結果: 自分が相手のどちら側へ押し出されたか
	enum class EPushBackSide
	{
		None,    // 重なっていなかった
		Left,    // 相手の左側へ (壁に右からぶつかった)
		Right,   // 相手の右側へ
		Top,     // 相手の上へ (着地)
		Bottom,  // 相手の下へ (頭をぶつけた)
	};

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

protected:
	// other (静止した矩形) との食い込みを、浅い軸の方向へ押し出して解消する。
	// Position とコライダー位置を更新し、どちら側へ押し出したかを返す。
	// 速度をどう扱うか (止める / 着地扱いにする) は呼び出し側の責務。
	EPushBackSide ResolvePenetration(const DWGameObject* other);
};