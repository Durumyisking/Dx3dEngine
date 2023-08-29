#include "Component.h"
#include "GameObj.h"



Component::Component(eComponentType type)
    : mType(type)
    , mOwner(nullptr)
{

}

Component::~Component()
{

}


math::Vector3 Component::GetOwnerPos() const
{
    return mOwner->GetPos();
}

math::Vector3 Component::GetOwnerWorldPos() const
{
    return mOwner->GetWorldPos();
}

