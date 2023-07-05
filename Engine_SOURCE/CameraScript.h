#pragma once
#include "Script.h"
#include "Player.h"

namespace dru
{
    // 흔들림 효과를 위한 구조체
    struct ShakeParams {
        float magnitude;  // 흔들림 크기
        float duration;   // 흔들림 지속 시간
    };

    class CameraScript :
        public Script
    {
    public:
        CameraScript();
        virtual ~CameraScript();

        virtual void Initialize() override;
        virtual void update() override;
        virtual void fixedUpdate() override;
        virtual void render() override;

        virtual void OnCollisionEnter(Collider2D* _oppo);
        virtual void OnCollision(Collider2D* _oppo);
        virtual void OnCollisionExit(Collider2D* _oppo);

        void KeyBoardMove();
        void TargetMove();
        void MakeCamShake(float _duration, float _Magnitude);
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