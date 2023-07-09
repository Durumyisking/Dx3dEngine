#pragma once
#include "Component.h"
#include "Physical.h"
#include "PxEventCallback.h"

namespace dru
{
	class PhysXCollider :
        public Component
	{
    public:
        PhysXCollider();
        virtual ~PhysXCollider();

        virtual void Initialize();
        virtual void update();
        virtual void fixedUpdate();
        virtual void render();


        virtual void OnCollision();

    private:
        void CreateDebugGeometry(std::shared_ptr<Geometries> geometries);
        void CreateDebugBox(math::Vector3 halfSize);
        void CreateDebugCapsule(float radius, float halfHeight);

    public:
        PxEventCallback* mCallback;
        Physical* mPhysical;
    };
}
