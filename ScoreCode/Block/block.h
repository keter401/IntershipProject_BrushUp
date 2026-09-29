#pragma once

#include "Framework\gameObject.h"

class DWInGameCharacter;

class DWBlock : public DWGameObject
{
private:
    const DWVector2 Size = ObjectSize;
    const DWVector2 Rot = DWVector2{ 0.0f, 0.0f };
    const unsigned int UnbreakableBlockColor = GetColor(175, 175, 175);
    const unsigned int BreakableBlockColor = GetColor(50, 50, 50);

    bool bBreakable = false;
    unsigned int BlockColor = UnbreakableBlockColor;

	int BlockHitPoints = 1;

public:
	DWBlock();

	void Init() override;
	void Uninit() override;
	void Update() override;
	void DataUpdate() override;
	void Draw() override;

	void SetBreakable(bool breakable) { bBreakable = breakable; }
	bool IsBreakable() const { return bBreakable; }

	void OnCollisionEnter2D(const DWGameObject* other) override;
	void OnCollisionExit2D(const DWGameObject* other) override;
};