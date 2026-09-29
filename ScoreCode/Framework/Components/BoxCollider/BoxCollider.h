#pragma once
#include "main.h"
#include "Framework\Components\component.h"

class DWBoxCollider2D : public DWComponent
{
private:
	DWVector2 PrevBoundingBoxPosition;
	DWVector2 BoundingBoxPosition;
	DWVector2 BoundingBoxScale;

	unsigned int DebugLineColor = GetColor(0, 0, 0);

	bool bActive = true;

	int topLeftX = 0;
	int topLeftY = 0;
	int bottomRightX = 0;
	int bottomRightY = 0;
public:
	DWBoxCollider2D(DWGameObject* owner) : DWComponent(owner) {}

	void Init() override;
	void Uninit() override;
	void Update() override;
	void DataUpdate() override;
	void Draw() override;

	void SetBoundingBoxPosition(const DWVector2 position) { BoundingBoxPosition = position; }
	void SetBoundingBoxScale(const DWVector2 scale) { BoundingBoxScale = scale; }
	void SetActive(const bool active) { bActive = active; }

	DWVector2 GetBoundingBoxPosition() const { return BoundingBoxPosition; }
	DWVector2 GetBoundingBoxScale() const { return BoundingBoxScale; }

	bool IsCollidingWith(const DWBoxCollider2D* other) const;
	bool IsStomped(const DWBoxCollider2D* other) const;
	bool DidStomp(const DWBoxCollider2D* other) const;
	bool IsActive() const { return bActive; }

	void SetDebugLineColor(bool isColliding);
};