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

        virtual void Initialize() override;
        virtual void Update() override;
        virtual void FixedUpdate() override;
        virtual void Render() override;


        virtual void OnCollision();

    private:
        void createDebugGeometry(std::shared_ptr<Geometries> geometries);
        void createDebugBox(math::Vector3 halfSize);
        void createDebugCapsule(float radius, float halfHeight);

    public:
        PxEventCallback* mCallback;
        Physical* mPhysical;
    };
}
