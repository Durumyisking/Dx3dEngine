#pragma once

#include "Component.h"

using namespace dru::math;

namespace dru
{
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
}