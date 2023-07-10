#pragma once
#include "Component.h"
#include "Transform.h"

namespace dru
{
<<<<<<< Updated upstream
    class CCollider2D :
        public CComponent
=======
    class Collider2D : public Component
>>>>>>> Stashed changes
    {
    public:
        CCollider2D();
        ~CCollider2D();

        virtual void Initialize() final;
        virtual void update() final;
        virtual void fixedUpdate() final;
        virtual void render() final;

<<<<<<< Updated upstream
        virtual void OnCollisionEnter(CCollider2D* _oppo);
        virtual void OnCollision(CCollider2D* _oppo);
        virtual void OnCollisionExit(CCollider2D* _oppo);


        // 물리 필요 없는애 (충돌로 움직임 없는 이벤트만 발동시킬때)
        virtual void OnTriggerEnter(CCollider2D* _oppo);
        virtual void OnTrigger(CCollider2D* _oppo);
        virtual void OnTriggerExit(CCollider2D* _oppo);
=======
        virtual void OnCollisionEnter(Collider2D* oppo);
        virtual void OnCollision(Collider2D* oppo);
        virtual void OnCollisionExit(Collider2D* oppo);


        // 물리 필요 없는애 (충돌로 움직임 없는 이벤트만 발동시킬때)
        virtual void OnTriggerEnter(Collider2D* oppo);
        virtual void OnTrigger(Collider2D* oppo);
        virtual void OnTriggerExit(Collider2D* oppo);
>>>>>>> Stashed changes

        void SetType(eColliderType type) { mType = type; }
        eColliderType GetType() const { return mType; }

        void SetCenter(Vector2 center) { mCenter = center; }
        Vector2 GetCenter() const { return mCenter; }

        void SetScale(Vector2 scale) { mScale = scale; }
        Vector2 GetScale() const { return mScale; }

        void SetRadius(float radius) { mRadius = radius; }
        float GetRadius() const 
        {
            if (eColliderType::Circle == mType)
                return mRadius;
            else
                return 0.f;
        }

        Vector3 GetColliderPos() const { return mPosition; }

        bool IsTrigger() const { return mbTrigger; }
        void SetTrigger() { mbTrigger = true; }

        void SetState(eCollisionState state) { mState = state; }
        eCollisionState GetState() { return mState; }

        UINT32 GetColliderID() const { return mColliderID; }

        static UINT32 colliderID;

        void On() { mbOn = true; }
        void Off() { mbOn = false; }

        void RenderingOn() { mbRenderOn = true; }
        void RenderingOff() { mbRenderOn = false; }


        bool IsOn() const { return mbOn; }
        bool IsRenderingOn() const { return mbRenderOn; }

    private:

        eColliderType mType;
        eCollisionState mState;

        CTransform* mTransform;

        const UINT32 mColliderID;

        Vector2 mScale;
        Vector2 mCenter;
        float   mRadius;
        Vector3 mPosition;

        bool    mbTrigger;

        bool    mbOn;
        bool    mbRenderOn;

    };
}
