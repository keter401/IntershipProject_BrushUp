#pragma once
#include "Enemy\enemy.h"

class DWCrawlingEnemy : public DWEnemy
{
private:
	const DWVector2 Size = ObjectSize;
	const DWVector2 Rot = DWVector2{ 0.0f, 0.0f };
	const float Velocity = 2.0f;
	const float Health = 3.0f;

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

	void OnCollisionEnter2D(const DWGameObject* other) override;
	void OnCollisionExit2D(const DWGameObject* other) override;
};