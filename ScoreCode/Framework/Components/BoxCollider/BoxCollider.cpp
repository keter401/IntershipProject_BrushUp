#include "BoxCollider.h"
#include "Scene\Scenes\scene.h"
#include "Enemy\enemy.h"
#include "Framework\Manager\ColliderManager\colliderManager.h"
#include "Camera\camera.h"

DWVector2 DebugPos;

void DWBoxCollider2D::Init()
{  
    DWGameObject* owner = GetOwner();
    if (owner == nullptr) return;

	float const scaleAdjust = 1.0f;

    BoundingBoxPosition = owner->GetPosition();
    PrevBoundingBoxPosition = BoundingBoxPosition;
    BoundingBoxScale = owner->GetScale() * scaleAdjust;

    DebugLineColor = GetColor(0, 0, 255);
}

void DWBoxCollider2D::Uninit()
{

}

void DWBoxCollider2D::Update()
{
    DWGameObject* owner = GetOwner();
    if (!owner) return;
    PrevBoundingBoxPosition = BoundingBoxPosition;
    BoundingBoxPosition = owner->GetPosition();
	DebugPos = BoundingBoxPosition;

}

void DWBoxCollider2D::DataUpdate()
{
    DWGameObject* owner = GetOwner();
    if (!owner) return;

    PrevBoundingBoxPosition = BoundingBoxPosition;
    BoundingBoxPosition = owner->GetPosition();

    DWVector2 offset(0.0f, 0.0f);
	DWScene* scene = owner->GetScene();
    if (scene != nullptr)
    {
        if (auto* cam = scene->GetGameObject<DWCamera>())
        {
            offset = cam->GetOffset();
        }
    }

    topLeftX = static_cast<int>((BoundingBoxPosition.x - BoundingBoxScale.x * 0.5f) - offset.x);
    topLeftY = static_cast<int>((BoundingBoxPosition.y - BoundingBoxScale.y * 0.5f) - offset.y);
    bottomRightX = static_cast<int>((BoundingBoxPosition.x + BoundingBoxScale.x * 0.5f) - offset.x);
    bottomRightY = static_cast<int>((BoundingBoxPosition.y + BoundingBoxScale.y * 0.5f) - offset.y);
}

void DWBoxCollider2D::Draw()
{
#ifdef _DEBUG
    DrawLine(topLeftX, topLeftY, bottomRightX, topLeftY, DebugLineColor);
    DrawLine(topLeftX, topLeftY, topLeftX, bottomRightY, DebugLineColor);
    DrawLine(bottomRightX, topLeftY, bottomRightX, bottomRightY, DebugLineColor);
    DrawLine(topLeftX, bottomRightY, bottomRightX, bottomRightY, DebugLineColor);

    if(GetOwner()->GetTag() == DWGameObject::ETag::PLAYER)
    {
        TCHAR* text;
        TCHAR buffer[64];
        _stprintf_s(buffer, _T("Player Collider\nx: %.1f\ny: %.1f"), BoundingBoxPosition.x, BoundingBoxPosition.y);
        text = buffer;
        DrawString(10, 50, text, DebugLineColor);
	}
#endif
}

bool DWBoxCollider2D::IsCollidingWith(const DWBoxCollider2D* other) const  
{
    if (!other) return false;

    const DWVector2 aPos = BoundingBoxPosition;
    const DWVector2 aSize = BoundingBoxScale;
    const DWVector2 bPos = other->GetBoundingBoxPosition();
    const DWVector2 bSize = other->GetBoundingBoxScale();

    const DWVector2 aHalf = aSize * 0.5f;
    const DWVector2 bHalf = bSize * 0.5f;

    const DWVector2 aMin{ aPos.x - aHalf.x, aPos.y - aHalf.y };
    const DWVector2 aMax{ aPos.x + aHalf.x, aPos.y + aHalf.y };
    const DWVector2 bMin{ bPos.x - bHalf.x, bPos.y - bHalf.y };
    const DWVector2 bMax{ bPos.x + bHalf.x, bPos.y + bHalf.y };

    const bool overlapX = (aMin.x <= bMax.x) && (aMax.x >= bMin.x);
    const bool overlapY = (aMin.y <= bMax.y) && (aMax.y >= bMin.y);
    return overlapX && overlapY;
}

bool DWBoxCollider2D::IsStomped(const DWBoxCollider2D* other) const
{
    if (!other) return false;

    const DWVector2 aPos = BoundingBoxPosition;
    const DWVector2 aPrev = PrevBoundingBoxPosition;
    const DWVector2 aSize = BoundingBoxScale;

    const DWVector2 bPos = other->GetBoundingBoxPosition();

    const DWVector2 bPrev = other->PrevBoundingBoxPosition;
    const DWVector2 bSize = other->GetBoundingBoxScale();

    const float aHalfW = aSize.x * 0.5f, aHalfH = aSize.y * 0.5f;
    const float bHalfW = bSize.x * 0.5f, bHalfH = bSize.y * 0.5f;

    const float aTop = aPos.y - aHalfH, aBottom = aPos.y + aHalfH;
    const float aLeft = aPos.x - aHalfW, aRight = aPos.x + aHalfW;

    const float bTop = bPos.y - bHalfH, bBottom = bPos.y + bHalfH;
    const float bLeft = bPos.x - bHalfW, bRight = bPos.x + bHalfW;

    const float aTopPrev = aPrev.y - aHalfH;
    const float bBottomPrev = bPrev.y + bHalfH;

    const float overlapX = std::min(aRight, bRight) - std::max(aLeft, bLeft);
    const float minRequiredOverlapX = std::min(aSize.x, bSize.x) * 0.3f;
    if (overlapX < minRequiredOverlapX) return false;

    const float penX = std::min(aRight - bLeft, bRight - aLeft);
    const float penY = std::min(aBottom - bTop, bBottom - aTop);

    if (penX < penY)
    {
        return false;
    }

    const float eps = std::min(aSize.y, bSize.y) * 0.15f;

    if (!(bBottomPrev <= aTopPrev + eps && bBottom >= aTop - eps)) 
    {
        return false;
    }

    return true;
}

bool DWBoxCollider2D::DidStomp(const DWBoxCollider2D* other) const
{
    if (other == nullptr) return false;

    const DWVector2 aPos = BoundingBoxPosition;
    const DWVector2 aPrev = PrevBoundingBoxPosition;
    const DWVector2 aSize = BoundingBoxScale;

    const DWVector2 bPos = other->GetBoundingBoxPosition();
    const DWVector2 bPrev = other->PrevBoundingBoxPosition;
    const DWVector2 bSize = other->GetBoundingBoxScale();

    const float aHalfW = aSize.x * 0.5f, aHalfH = aSize.y * 0.5f;
    const float bHalfW = bSize.x * 0.5f, bHalfH = bSize.y * 0.5f;

    const float aTop = aPos.y - aHalfH, aBottom = aPos.y + aHalfH;
    const float aLeft = aPos.x - aHalfW, aRight = aPos.x + aHalfW;

    const float bTop = bPos.y - bHalfH, bBottom = bPos.y + bHalfH;
    const float bLeft = bPos.x - bHalfW, bRight = bPos.x + bHalfW;

    const float aBottomPrev = aPrev.y + aHalfH;
    const float bTopPrev = bPrev.y - bHalfH;

    const float overlapX = std::min(aRight, bRight) - std::max(aLeft, bLeft);
    const float minRequiredOverlapX = std::min(aSize.x, bSize.x) * 0.3f;
    if (overlapX < minRequiredOverlapX) return false;

    const float penX = std::min(aRight - bLeft, bRight - aLeft);
    const float penY = std::min(aBottom - bTop, bBottom - aTop);

    if (penX < penY)
    {
        return false;
    }

    const float eps = std::min(aSize.y, bSize.y) * 0.15f;

    if (!(aBottomPrev <= bTopPrev + eps && aBottom >= bTop - eps))
    {
        return false;
    }

    return true;
}

void DWBoxCollider2D::SetDebugLineColor(bool isColliding)
{
    if (isColliding) 
    {
        DebugLineColor = GetColor(255, 0, 0);  
    } 
    else 
    {
        DebugLineColor = GetColor(0, 0, 255);  
	}
}