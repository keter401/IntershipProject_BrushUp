#include "bullet.h"
#include "Framework\Manager\BulletManager\bulletManager.h"
#include "Framework\Manager\ColliderManager\colliderManager.h"
#include "Scene\Scenes\scene.h"

DWBullet::DWBullet()
{
	Scale = Size;
	Rotation = Rot;
	Tag = DWGameObject::ETag::BULLET;
	BulletColor = GetColor(100, 100, 255);
    bReusableObject = true;
}

void DWBullet::Init()
{
    AddComponent<DWBoxCollider2D>(this);
}

void DWBullet::Uninit()
{

}

void DWBullet::Update()
{
    if (!bActive) return;

    DWVector2 pos = GetPosition();
    pos += Vel;
    SetPosition(pos);

    Life--;
    if (Life <= 0)
    {
        if (Pool != nullptr)
        {
            Pool->Despawn(this);
        }
        return;
    }

    for (auto& component : ComponentsList)
    {
        component->Update();
    }

}

void DWBullet::DataUpdate()
{
    for (auto& component : ComponentsList)
    {
        component->DataUpdate();
    }
}

void DWBullet::Draw()
{
    if (!bActive) return;

    const DWVector2 offset = GetCameraOffset();

    int posX = static_cast<int>(Position.x - offset.x);
    int posY = static_cast<int>(Position.y - offset.y);
    int r = static_cast<int>(Scale.x / 2);

    DrawCircle(posX, posY, r, BulletColor, TRUE);

#ifdef _DEBUG
    
    TCHAR* text;
    TCHAR buffer[64];
    _stprintf_s(buffer, _T("%d"), Number);
    text = buffer;
    const unsigned int textColor = GetColor(255, 255, 255);
    DrawString(posX, posY, text, textColor);

#endif
}

void DWBullet::OnCollisionEnter2D(DWGameObject* other)
{
    if (!bActive) return;

	DWGameObject::ETag otherTag = other->GetTag();

    switch (otherTag)
    {
	case DWGameObject::ETag::ENEMY:
    case DWGameObject::ETag::BLOCK:
    {
        if (Pool != nullptr) Pool->Despawn(this);
        break;
    }
    default:
		break;
    }
}

void DWBullet::OnCollisionExit2D(DWGameObject* other)
{

}

void DWBullet::Activate(DWBulletManager* pool, const DWVector2& pos)
{
	if (pool == nullptr) return;

    Pool = pool;
    bActive = true;
    Life = MaxLife;
    Vel = Velocity;

	Position = pos;

	DWBoxCollider2D* boxCollider = GetComponent<DWBoxCollider2D>();
    if (boxCollider != nullptr)
    {
        boxCollider->SetActive(true);
        boxCollider->SetBoundingBoxPosition(pos);
		boxCollider->SetBoundingBoxScale(Scale);
    }
}

void DWBullet::Deactivate()
{
    bActive = false;
    Life = 0;

    DWBoxCollider2D* boxCollider = GetComponent<DWBoxCollider2D>();
    if (boxCollider != nullptr)
    {
        boxCollider->SetActive(false);
    }
}
