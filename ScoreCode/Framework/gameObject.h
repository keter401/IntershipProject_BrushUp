#pragma once

#include "main.h"
#include "Framework\Components\component.h"

class DWComponent;
class DWScene;
class DWAudio;

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

	const DWVector2 ObjectSize = DWVector2{ 50.0f, 50.0f };

protected:
	bool bDestory = false;

	DWScene* Scene = nullptr;

	DWVector2 Position;
	DWVector2 Rotation;
	DWVector2 Scale;

	ETag Tag = ETag::NONE;

	std::list<DWComponent*> PendingComponentsList;
	std::list<DWComponent*> ComponentsList;

	DWInput* Input = nullptr;

	DWAudio* Audio = nullptr;

	bool bReuseableObject = false;
public:
	DWGameObject() {}

	virtual void Init() {}
	virtual void Uninit() {}
	virtual void Update() {}
	virtual void DataUpdate() {}
	virtual void Draw() {}

	void SetInput(DWInput* input) { Input = input; }
	DWInput* GetInput() const { return Input; }

	void SetAudio(DWAudio* audio) { Audio = audio; }
	DWAudio* GetAudio() const { return Audio; }

	void SetDestory() { bDestory = true; }
	bool GetDestoryFlag() const { return bDestory; }
	bool Destroy();
	bool IsReuseableObject() const { return bReuseableObject; }

	// コンポーネントを追加(予約)
	template<typename T>
	T* AddComponent(DWGameObject* owner)
	{
		T* newComponent = new T(owner);
		PendingComponentsList.push_back(newComponent);
		return newComponent;
	}
	//予約コンポーネントをアタッチ
	void RegistPendingComponents();
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
	virtual void OnCollisionEnter2D(const DWGameObject* other) {};
	// 衝突継続中、2フレーム目以降に毎フレーム呼ばれる
	virtual void OnCollisionStay2D(const DWGameObject* other) {};
	// 衝突が終了したフレームに1回だけ呼ばれる
	virtual void OnCollisionExit2D(const DWGameObject* other) {};

	DWScene* GetScene() const { return Scene; }
	DWVector2 GetPosition() const { return Position; }
	DWVector2 GetRotation() const { return Rotation; }
	DWVector2 GetScale() const { return Scale; }
	const ETag GetTag() const { return Tag; }

	void SetScene(DWScene* scene) { Scene = scene; }
	void SetPosition(const DWVector2& position) { Position = position; }
	void SetRotation(const DWVector2& rotation) { Rotation = rotation; }
	void SetScale(const DWVector2& scale) { Scale = scale; }
	void SetTag(const ETag tag) { Tag = tag; }
};
