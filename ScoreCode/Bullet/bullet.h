#pragma once

#include "Framework\gameObject.h"

class DWBulletManager;

class DWBullet : public DWGameObject {
private:
    static constexpr DWVector2 Size = ObjectSize * 0.5f;
    static constexpr DWVector2 Rot = DWVector2{ 0.0f, 0.0f };
    static constexpr DWVector2 Velocity = DWVector2{ 0.0f, 15.0f };
    static constexpr int MaxLife = 120;   // ÉtÉåÅ[ÉÄêî

    unsigned int BulletColor;

    bool bActive = false;
    DWBulletManager* Pool = nullptr;
    DWVector2 Vel = Velocity;
    int Life = 0;

	int Number = 0;

public:
    DWBullet();

    void Init() override;
    void Uninit() override;
    void Update() override;
    void DataUpdate() override;
    void Draw() override;

    void OnCollisionEnter2D(DWGameObject* other) override;
    void OnCollisionExit2D(DWGameObject* other) override;

    void Activate(DWBulletManager* pool, const DWVector2& pos);
    void Deactivate();

	void SetNumber(int num) { Number = num; }

    bool IsActive() const { return bActive; }
};