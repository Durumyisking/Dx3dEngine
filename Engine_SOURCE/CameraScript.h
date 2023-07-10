#pragma once
#include "Script.h"

namespace dru
{
    // 흔들림 효과를 위한 구조체
    struct ShakeParams {
        float magnitude;  // 흔들림 크기
        float duration;   // 흔들림 지속 시간
    };

    class Camera;
    class CameraScript :
        public Script
    {
    public:
        CameraScript();
        virtual ~CameraScript();

        virtual void Initialize() final;
        virtual void update() final;
        virtual void fixedUpdate() final;
        virtual void render() final;

        virtual void OnCollisionEnter(Collider2D* oppo);
        virtual void OnCollision(Collider2D* oppo);
        virtual void OnCollisionExit(Collider2D* oppo);

        void KeyBoardMove();
        void TargetMove();
        void MakeCamShake(float duration, float magnitude);
        void Shake(const ShakeParams& params);
        void CancelShake();
        void ShakeMove();


    private:
        Camera* mCameraObject;
        Transform* mTransform;
        GameObj* mTarget;
        Vector3 mLookAt;

        float	mSpeed;
        float	mCamStep;

        bool mbShaking;
        ShakeParams mShakeParams;
        float mShakeTimer;


    };

}