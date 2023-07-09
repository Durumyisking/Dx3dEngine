#pragma once
#include "Entity.h"

struct MassProperties
{

        // 정적마찰 (움직이기 전의 마찰:막 움직이기 시작할때 마찰계수                // 물체 충돌 전 후 속도 비율
    MassProperties(float StaticFriction = 0.5f, float DynamicFriction = 0.2f, float Restitution = 0.603f)
        : mStaticFriction(StaticFriction)
        , mDynamicFriction(DynamicFriction)
        , mRestitution(Restitution)
    { }

    float mStaticFriction;
    float mDynamicFriction;
    float mRestitution;
};

class PhysicalProperties : public dru::Entity
{
public:
    PhysicalProperties(const MassProperties& massProperties);
    ~PhysicalProperties();

    void ApplyToShape(PxShape* shape);
    void SetMaterialFlag(PxMaterialFlags flag) { mMaterial->setFlags(flag); }

public:
    PxMaterial* GetMaterial() { return mMaterial; }

    float GetStaticFriction() { return mStaticFriction; }
    float GetDynamicFriction() { return mDynamicFriction; }
    float GetRestitution() { return mRestitution; }

    void SetStaticFriction(float fFriction);
    void SetDynamicFriction(float fFriction);
    void SetRestitution(float fRestitution);

private:
    float mStaticFriction;
    float mDynamicFriction;
    float mRestitution;

private:
    PxMaterial* mMaterial;
};