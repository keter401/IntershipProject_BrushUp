#pragma once
#include "Framework\InGameCharacter\InGameCharacter.h"
#include "Framework\State\state.h"

class DWPlayer : public DWInGameCharacter
{
public:
	enum EPlayerState
	{
		Idle = 0,
		Move = 1,
		Jump = 2,
		Shot = 3,
		Fall = 4,
		Land = 5,
	};

private:

	const DWVector2 Size = ObjectSize;
	const DWVector2 Rot = DWVector2{ 0.0f, 0.0f };
	const int AmmoMax = 10;
	const float Velocity = 5.0f;
	const float MaxHealth = 5.0f;
	const float JumpPower = 15.0f;
	const float DefaultGravity = 0.4f;
	const unsigned int DefaultColor = GetColor(255, 255, 255);
	const float TerminalVelocity = 15.0f;
	const float ShotReboundPower = 0.5f;
	const float TreadReboundPower = 10.0f;
	const float DamagedReboundPower = 15.0f;
	const int InvincibleFrame = 90;

	DWVector2 MoveDirection;
	int AmmoCount;
	bool bIsFaceRight;

	int ShotCoolDownCounter = 0;
	int ShotCoolDownFrame = 0;

	unsigned int PlayerBodyColor;

	std::vector<DWState*> PlayerStatesList;

	EPlayerState CurrentState = EPlayerState::Idle;
	EPlayerState PreviousState = EPlayerState::Idle;

	int InvincibleCounter = 0;
	int InvincibleColorCode = 255;
	int InvincibleColorChangeRate = 30;

	bool bInvincibleFadeUp = false;

	bool bIsInvincible = false;

public:
	DWPlayer();

	void PlayerMove();
	void PlayerJump();
	void PlayerShot();
	void GravityForce();
	void ShotRebound();
	void TreadRebound();
	void AmmoReload();
	void TakeDamaged(float damage) override;
	void DamagedByOther(float damage, const DWGameObject* other);

	void Init() override;
	void Uninit() override;
	void Update() override;
	void DataUpdate() override;
	void Draw() override;

	DWVector2 GetSize() const { return Size; }
	DWVector2 GetRotation() const { return Rotation; }
	int GetAmmoCount() const { return AmmoCount; }
	float GetVelocity() const { return Velocity; }
	float GetMaxHealth() const { return MaxHealth; }
	float GetJumpPower() const { return JumpPower; }
	float GetGravity() const { return DefaultGravity; }
	int GetShootCoolDownCounter() const { return ShotCoolDownCounter; }
	int GetShootCoolDownFrame() const { return ShotCoolDownFrame; }

	bool GetIsFaceRight() const { return bIsFaceRight; }
	void SetIsFaceRight(const bool isFaceRight) { bIsFaceRight = isFaceRight; }

	void SetCurrentState(const EPlayerState State);

	EPlayerState GetCurrentState() const { return CurrentState; }
	EPlayerState GetPreviousState() const { return PreviousState; }

	void OnCollisionEnter2D(const DWGameObject* other) override;
	void OnCollisionStay2D(const DWGameObject* other) override;
	void OnCollisionExit2D(const DWGameObject* other) override;

	void PushBack(const DWGameObject* other);

private:
	// ブロック押し戻しと敵接触ダメージは接触中ずっと必要なので Enter / Stay 共通
	void HandleContact(const DWGameObject* other);
};
