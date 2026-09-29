#include "Framework\InGameCharacter\InGameCharacter.h"
#include "Framework\Components\BoxCollider\BoxCollider.h"
#include <cmath>

DWInGameCharacter::EPushBackSide DWInGameCharacter::ResolvePenetration(const DWGameObject* other)
{
	if (other == nullptr) return EPushBackSide::None;

	const DWVector2 objPos = other->GetPosition();
	const DWVector2 objHalf = other->GetScale() * 0.5f;
	const DWVector2 half = Scale * 0.5f;

	const float dx = objPos.x - Position.x;
	const float dy = objPos.y - Position.y;

	// 各軸の食い込み量。0 以下ならぴったり接しているだけで押し出し不要
	const float px = (half.x + objHalf.x) - std::abs(dx);
	const float py = (half.y + objHalf.y) - std::abs(dy);
	if (px <= 0.0f || py <= 0.0f) return EPushBackSide::None;

	EPushBackSide side = EPushBackSide::None;

	if (px < py)
	{
		if (dx < 0.0f)
		{
			Position.x = objPos.x + (objHalf.x + half.x);
			side = EPushBackSide::Right;
		}
		else
		{
			Position.x = objPos.x - (objHalf.x + half.x);
			side = EPushBackSide::Left;
		}
	}
	else
	{
		if (dy < 0.0f)
		{
			Position.y = objPos.y + (objHalf.y + half.y);
			side = EPushBackSide::Bottom;
		}
		else
		{
			Position.y = objPos.y - (objHalf.y + half.y);
			side = EPushBackSide::Top;
		}
	}

	// 同一フレーム内の後続の判定 (踏みつけなど) が新しい位置を見られるようコライダーも同期
	if (DWBoxCollider2D* collider = GetComponent<DWBoxCollider2D>())
	{
		collider->SetBoundingBoxPosition(Position);
	}

	return side;
}
