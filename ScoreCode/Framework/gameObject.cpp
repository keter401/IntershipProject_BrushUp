#include "Framework\gameObject.h"

bool DWGameObject::Destroy()
{
	if (bDestory)
	{
		Uninit();
		delete this;
		return true;
	}
	else
	{
		return false;
	}
}

void DWGameObject::RegistPendingComponents()
{
	//追加待ちのコンポーネントを登録
	for (auto component : PendingComponentsList)
	{
		ComponentsList.push_back(component);
	}

	//コンポーネントをすべて初期化
	for (auto component : PendingComponentsList)
	{
		component->Init();
	}

	PendingComponentsList.clear();
}