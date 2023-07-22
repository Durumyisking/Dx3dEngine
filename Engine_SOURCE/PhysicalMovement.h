#pragma once

#include "Component.h"

using namespace math;



class PhysicalMovement :
    public Component
{
public:
    PhysicalMovement();
    virtual ~PhysicalMovement();

    virtual void Initialize();
    virtual void Update();
    virtual void FixedUpdate();
    virtual void Render();

public:
    void Move(const Vector3& velocity);

private:
};
