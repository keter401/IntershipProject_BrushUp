#pragma once
#include "Framework\InGameCharacter\InGameCharacter.h"
#include "Framework\State\state.h"

class DWPlayer : public DWInGameCharacter
{
public:
	// 着地 (Land) と射撃 (Shot) は 1 フレームで完結する「アクション」なので
	// ステートにせず OnLanded() / TryShoot() で処理する
	enum EPlayerState
	{
		Idle = 0,
		Move = 1,
		Jump = 2,
		Fall = 3,

		StateCount,
	};

private:
	static constexpr DWVector2 Size = ObjectSize;
	static constexpr DWVector2 Rot = DWVector2{ 0.0f, 0.0f };
	static constexpr int AmmoMax = 10;
	static constexpr float Velocity = 5.0f;
	static constexpr float PlayerMaxHealth = 5.0f;
	static constexpr float JumpPower = 15.0f;
	static constexpr float DefaultGravity = 0.4f;
	static constexpr float TerminalVelocity = 15.0f;
	static constexpr float ShotReboundPower = 0.5f;
	static constexpr float TreadReboundPower = 10.0f;
	static constexpr float DamagedReboundPower = 15.0f;
	static constexpr int InvincibleFrame = 90;
	static constexpr int ShotCoolDownFrame = 5;

	// DxLib の GetColor は実行時にしか呼べないので、色だけはインスタンスごとに持つ
	const unsigned int DefaultColor = GetColor(255, 255, 255);

	int AmmoCount = AmmoMax;
	bool bIsFaceRight = true;

	int ShotCoolDownCounter = 0;

	unsigned int PlayerBodyColor = DefaultColor;

	// ステートはプレイヤーが所有し、Uninit で delete する
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

	// 弾があり、クールダウンが明けていれば撃つ。撃ったら true
	bool TryShoot();
	// ブロックの上に乗った瞬間の処理 (弾補充・落下速度リセット・Idle へ)
	void OnLanded();

	void Init() override;
	void Uninit() override;
	void Update() override;
	void DataUpdate() override;
	void Draw() override;

	DWVector2 GetSize() const { return Size; }
	int GetAmmoCount() const { return AmmoCount; }
	float GetVelocity() const { return Velocity; }
	float GetJumpPower() const { return JumpPower; }
	float GetGravity() const { return DefaultGravity; }
	int GetShootCoolDownCounter() const { return ShotCoolDownCounter; }
	int GetShootCoolDownFrame() const { return ShotCoolDownFrame; }

	bool GetIsFaceRight() const { return bIsFaceRight; }
	void SetIsFaceRight(const bool isFaceRight) { bIsFaceRight = isFaceRight; }

	void SetCurrentState(const EPlayerState State);

	EPlayerState GetCurrentState() const { return CurrentState; }
	EPlayerState GetPreviousState() const { return PreviousState; }

	void OnCollisionEnter2D(DWGameObject* other) override;
	void OnCollisionStay2D(DWGameObject* other) override;
	void OnCollisionExit2D(DWGameObject* other) override;

	void PushBack(const DWGameObject* other);

private:
	// ブロック押し戻しと敵接触ダメージは接触中ずっと必要なので Enter / Stay 共通
	void HandleContact(DWGameObject* other);
};
