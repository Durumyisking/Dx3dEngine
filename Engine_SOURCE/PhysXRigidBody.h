#pragma once
#include "Component.h"

namespace dru
{
    class PhysXRigidBody :
        public Component
    {
public:
    PhysXRigidBody();
    virtual ~PhysXRigidBody();

    virtual void Initialize();
    virtual void update();
    virtual void fixedUpdate();
    virtual void render();

private:
    eActorType  mActorType;
    };
}
