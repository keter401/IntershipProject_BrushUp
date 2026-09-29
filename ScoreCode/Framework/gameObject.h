#pragma once

#include "main.h"
#include "Framework\Components\component.h"

class DWComponent;
class DWScene;
class DWAudio;

// カメラオフセットを引いた描画用のスクリーン矩形
struct DWScreenRect
{
	int left = 0;
	int top = 0;
	int right = 0;
	int bottom = 0;
};

class DWGameObject
{
public:
	enum ETag
	{
		NONE,
		PLAYER,
		ENEMY,
		BULLET,
		BLOCK,
	};

	// 1 タイル分の基準サイズ
	static constexpr DWVector2 ObjectSize = DWVector2{ 50.0f, 50.0f };

protected:
	// true ならフレーム末に Scene が Destroy() で delete する
	bool bDestroy = false;

	DWScene* Scene = nullptr;

	DWVector2 Position;
	DWVector2 Rotation;
	DWVector2 Scale;

	ETag Tag = ETag::NONE;

	// コンポーネントはこのオブジェクトが所有し、デストラクタで delete する
	std::list<DWComponent*> PendingComponentsList;
	std::list<DWComponent*> ComponentsList;

	DWInput* Input = nullptr;

	DWAudio* Audio = nullptr;

	// true ならステージ再生成 (DWField::ClearFieldLayer) で破棄されない
	bool bReusableObject = false;

public:
	DWGameObject() {}
	virtual ~DWGameObject();

	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void DataUpdate() {}
	virtual void Draw() {}

	void SetInput(DWInput* input) { Input = input; }
	DWInput* GetInput() const { return Input; }

	void SetAudio(DWAudio* audio) { Audio = audio; }
	DWAudio* GetAudio() const { return Audio; }

	void SetDestroy() { bDestroy = true; }
	bool GetDestroyFlag() const { return bDestroy; }
	// bDestroy が立っていれば Uninit して自身を delete し true を返す。Scene の remove_if から呼ばれる
	bool Destroy();
	bool IsReusableObject() const { return bReusableObject; }

	// コンポーネントを追加(予約)
	template<typename T>
	T* AddComponent(DWGameObject* owner)
	{
		T* newComponent = new T(owner);
		PendingComponentsList.push_back(newComponent);
		return newComponent;
	}
	//予約コンポーネントをアタッチして Init する
	void RegisterPendingComponents();
	template<typename T>
	T* GetComponent()
	{
		for (DWComponent* component : ComponentsList) {
			T* castedComponent = dynamic_cast<T*>(component);
			if (castedComponent) {
				return castedComponent;
			}
		}
		return nullptr;
	}

	// 衝突開始フレームに1回だけ呼ばれる
	virtual void OnCollisionEnter2D(DWGameObject* other) {};
	// 衝突継続中、2フレーム目以降に毎フレーム呼ばれる
	virtual void OnCollisionStay2D(DWGameObject* other) {};
	// 衝突が終了したフレームに1回だけ呼ばれる
	virtual void OnCollisionExit2D(DWGameObject* other) {};

	DWScene* GetScene() const { return Scene; }

	// 所属シーンのカメラオフセット。シーン未設定なら (0, 0)
	DWVector2 GetCameraOffset() const;
	// Position を中心に Scale の大きさを持つ矩形を、カメラオフセットを引いてスクリーン座標にしたもの
	DWScreenRect GetScreenRect() const;

	DWVector2 GetPosition() const { return Position; }
	DWVector2 GetRotation() const { return Rotation; }
	DWVector2 GetScale() const { return Scale; }
	ETag GetTag() const { return Tag; }

	void SetScene(DWScene* scene) { Scene = scene; }
	void SetPosition(const DWVector2& position) { Position = position; }
	void SetRotation(const DWVector2& rotation) { Rotation = rotation; }
	void SetScale(const DWVector2& scale) { Scale = scale; }
	void SetTag(const ETag tag) { Tag = tag; }
};
