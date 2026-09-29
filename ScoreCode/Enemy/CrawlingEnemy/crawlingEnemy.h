#pragma once
#include "Enemy\enemy.h"

class DWCrawlingEnemy : public DWEnemy
{
private:
	static constexpr DWVector2 Size = ObjectSize;
	static constexpr DWVector2 Rot = DWVector2{ 0.0f, 0.0f };
	static constexpr float Velocity = 2.0f;
	static constexpr float InitialHealth = 3.0f;

public:
	DWCrawlingEnemy() : DWEnemy() {}

	void EnemyMove() override;
	void TakeDamaged(const float damage) override;

	void Init() override;
	void Uninit() override;
	void Update() override;
	void DataUpdate() override;
	void Draw() override;

	void EnemyDamaged() override { EnemyBodyColor = GetColor(255, 0, 0); }

	void OnCollisionEnter2D(DWGameObject* other) override;
	void OnCollisionExit2D(DWGameObject* other) override;
};