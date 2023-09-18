#pragma once
#include "Entity.h"

struct MassProperties
{
    // 정적마찰 (움직이기 전의 마찰:막 움직이기 시작할때 마찰계수                // 물체 충돌 전 후 속도 비율
    MassProperties(float staticFriction = 0.5f, float dynamicFriction = 0.75f, float restitution = -0.0001f)
        : mStaticFriction(staticFriction)
        , mDynamicFriction(dynamicFriction)
        , mRestitution(restitution)
    { }

    float mStaticFriction;
    float mDynamicFriction;
    float mRestitution;
};

class PhysicalProperties : public Entity
{
public:
    PhysicalProperties(const MassProperties& massProperties);
    ~PhysicalProperties();

    void ApplyToShape(PxShape& shape);
    void SetMaterialFlag(PxMaterialFlags flag) { mMaterial->setFlags(flag); }

public:
    PxMaterial* GetMaterial()   const { return mMaterial; }

    float GetStaticFriction()   const { return mStaticFriction; }
    float GetDynamicFriction()  const { return mDynamicFriction; }
    float GetRestitution()      const { return mRestitution; }

    void SetStaticFriction(float friction);
    void SetDynamicFriction(float friction);
    void SetRestitution(float restitution);

private:
    float mStaticFriction;
    float mDynamicFriction;
    float mRestitution;

private:
    PxMaterial* mMaterial;
    // 피직스 머티리얼은 static/dynamic friction, restituion(반발력 ex- 벽에 튕겼을때 튀어나가는거) 을 가지는 정보이다.
    // 이 머티리얼을 pxShape에 적용시킬 수 있다.


};