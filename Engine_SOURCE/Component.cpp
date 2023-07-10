#include "Component.h"
#include "GameObj.h"

namespace dru
{
    Component::Component(eComponentType _Type)
        : mType(_Type)
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

} 