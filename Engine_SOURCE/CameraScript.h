#pragma once
#include "Script.h"
#include "Player.h"

namespace dru
{
    // ��鸲 ȿ���� ���� ����ü
    struct ShakeParams {
        float magnitude;  // ��鸲 ũ��
        float duration;   // ��鸲 ���� �ð�
    };

    class CCameraScript :
        public CScript
    {
    public:
        CCameraScript();
        virtual ~CCameraScript();

        virtual void Initialize() final;
        virtual void update() final;
        virtual void fixedUpdate() final;
        virtual void render() final;

<<<<<<< Updated upstream
        virtual void OnCollisionEnter(CCollider2D* _oppo);
        virtual void OnCollision(CCollider2D* _oppo);
        virtual void OnCollisionExit(CCollider2D* _oppo);
=======
        virtual void OnCollisionEnter(Collider2D* oppo);
        virtual void OnCollision(Collider2D* oppo);
        virtual void OnCollisionExit(Collider2D* oppo);
>>>>>>> Stashed changes

        void KeyBoardMove();
        void TargetMove();
        void MakeCamShake(float duration, float magnitude);
        void Shake(const ShakeParams& params);
        void CancelShake();
        void ShakeMove();


    private:
        CCamera* mCameraObject;
        CTransform* mTransform;
        CGameObj* mTarget;
        Vector3 mLookAt;

        float	mSpeed;
        float	mCamStep;

        bool mbShaking;
        ShakeParams mShakeParams;
        float mShakeTimer;


    };

}