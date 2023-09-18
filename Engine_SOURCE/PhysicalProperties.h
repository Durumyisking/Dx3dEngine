#pragma once
#include "Entity.h"

struct MassProperties
{
    // �������� (�����̱� ���� ����:�� �����̱� �����Ҷ� �������                // ��ü �浹 �� �� �ӵ� ����
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
    // ������ ��Ƽ������ static/dynamic friction, restituion(�ݹ߷� ex- ���� ƨ������ Ƣ����°�) �� ������ �����̴�.
    // �� ��Ƽ������ pxShape�� �����ų �� �ִ�.


};