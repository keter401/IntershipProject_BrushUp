#pragma once

#include "Framework\gameObject.h"

class DWCamera : public DWGameObject
{
private:
    DWGameObject* Target = nullptr;
    DWVector2 Offset = DWVector2(0.0f, 0.0f);

public:
    DWCamera() {}

    void Init() override;
    void Uninit() override;
    void Update() override;
    void Draw() override {}

    void SetTarget(DWGameObject* target) { Target = target; }
    const DWVector2& GetOffset() const { return Offset; }
};
