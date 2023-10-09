#pragma once
#include "Script.h"



// 흔들림 효과를 위한 구조체
struct ShakeParams
{
    float magnitude;  // 흔들림 크기
    float duration;   // 흔들림 지속 시간
};

class Camera;
class PlayerCameraScript :
    public Script
{
public:
    PlayerCameraScript();
    virtual ~PlayerCameraScript();

    virtual void Initialize() override;
    virtual void Update() override;
    virtual void FixedUpdate() override;
    virtual void Render() override;

    virtual void OnCollisionEnter(Collider2D* _oppo)override;
    virtual void OnCollision(Collider2D* _oppo) override;
    virtual void OnCollisionExit(Collider2D* _oppo)override;

    void KeyBoardMove();
    void TargetMove();
    void MakeCamShake(float _duration, float _Magnitude);
    void Shake(const ShakeParams& params);
    void CancelShake();
    void ShakeMove();

    void SetTargetObject(GameObj* target) { mTarget = target; }
private:
    Camera* mCameraObject;

    Transform* mTransform;
    GameObj* mTarget;
    Vector3 mLookAt;
    Vector3 mLookTo;

    float	mSpeed;
    float	mCamStep;

    bool mbShaking;
    ShakeParams mShakeParams;
    float mShakeTimer;


};

