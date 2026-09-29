#pragma once

#include "Framework\gameObject.h"

class DWBulletManager;

class DWBullet : public DWGameObject {
private:
    const DWVector2 Size = ObjectSize * 0.5f;
    const DWVector2 Rot = DWVector2{ 0.0f, 0.0f };
    const DWVector2 Velocity = DWVector2{ 0.0f, 15.0f };
    const float MaxLife = 120.0f;

    unsigned int BulletColor;

    bool bActive = false;
    DWBulletManager* Pool = nullptr;
    DWVector2 Vel = Velocity;
    float Life = 0.0f;

	int Number = 0;

public:
    DWBullet();

    void Init() override;
    void Uninit() override;
    void Update() override;
    void DataUpdate() override;
    void Draw() override;

    void OnCollisionEnter2D(const DWGameObject* other) override;
    void OnCollisionExit2D(const DWGameObject* other) override;

    void Activate(DWBulletManager* pool, const DWVector2& pos);
    void Deactivate();

	void SetNumber(int num) { Number = num; }

    bool IsActive() const { return bActive; }
};