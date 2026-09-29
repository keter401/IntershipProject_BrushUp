#pragma once

class DWGameObject;

class DWComponent
{
private:
	DWGameObject* OwnerObject;
public:
	DWComponent(DWGameObject* owner) : OwnerObject(owner) {}

	DWGameObject* GetOwner() const { return OwnerObject; }

	virtual void Init() {};
	virtual void Uninit() {};
	virtual void Update() {};
	virtual void DataUpdate() {};
	virtual void Draw() {};
};