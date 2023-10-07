#include "Component.h"
#include "GameObj.h"
#include "Transform.h"
#include "Camera.h"
#include "Mesh.h"
#include "RigidBody.h"
#include "PhysXRigidBody.h"
#include "PhysicalMovement.h"
#include "Physical.h"
#include "PhysXCollider.h"
#include "Animator.h"
#include "BoneAnimator.h"
#include "SpriteRenderer.h"
#include "MeshRenderer.h"
#include "PostProcessRenderer.h"
#include "ParticleSystem.h"
#include "Light.h"



Component::Component(eComponentType type)
    : mType(type)
    , mOwner(nullptr)
    , mbSwitch(true)
{

}

Component::~Component()
{

}


math::Vector3 Component::GetOwnerPos() const
{
    assert(mOwner);
    return mOwner->GetPos();
}

math::Vector3 Component::GetOwnerWorldPos() const
{
    assert(mOwner);
    return mOwner->GetWorldPos();
}

Transform* Component::GetTransform() 
{
    assert(mOwner); 
    Transform* comp = GetOwner()->GetComponent<Transform>();
    assert(comp);
    return comp;
}

Camera* Component::GetCamera() 
{
    assert(mOwner);
    Camera* comp = GetOwner()->GetComponent<Camera>();
    assert(comp);
    return comp;
}


RigidBody* Component::GetRigidBody() 
{
    assert(mOwner);
    RigidBody* comp = GetOwner()->GetComponent<RigidBody>();
    assert(comp);
    return comp;
}

PhysXRigidBody* Component::GetPhysXRigidBody()
{
    assert(mOwner);
    PhysXRigidBody* comp = GetOwner()->GetComponent<PhysXRigidBody>();
    assert(comp);
    return comp;
}

PhysicalMovement* Component::GetMovement()
{
    assert(mOwner);
    PhysicalMovement* comp = GetOwner()->GetComponent<PhysicalMovement>();
    assert(comp);
    return comp;
}

Physical* Component::GetPhysical() 
{
    assert(mOwner);
    Physical* comp = GetOwner()->GetComponent<Physical>();
    assert(comp);
    return comp;
}

PhysXCollider* Component::GetPhysXCollider()
{
    assert(mOwner);
    PhysXCollider* comp = GetOwner()->GetComponent<PhysXCollider>();
    assert(comp);
    return comp;
}

Animator* Component::GetAnimator() 
{
    assert(mOwner);
    Animator* comp = GetOwner()->GetComponent<Animator>();
    assert(comp);
    return comp;
}

BoneAnimator* Component::GetBoneAnimator() 
{
    assert(mOwner);
    BoneAnimator* comp = GetOwner()->GetComponent<BoneAnimator>();
    assert(comp);
    return comp;
}

MeshRenderer* Component::GetMeshRenderer() 
{
    assert(mOwner);
    MeshRenderer* comp = GetOwner()->GetComponent<MeshRenderer>();
    assert(comp);
    return comp;
}

SpriteRenderer* Component::GetSpriteRenderer() 
{
    assert(mOwner);
    SpriteRenderer* comp = GetOwner()->GetComponent<SpriteRenderer>();
    assert(comp);
    return comp;
}


ParticleSystem* Component::GetParticle() 
{
    assert(mOwner);
    ParticleSystem* comp = GetOwner()->GetComponent<ParticleSystem>();
    assert(comp);
    return comp;
}

Light* Component::GetLight() 
{
    assert(mOwner);
    Light* comp = GetOwner()->GetComponent<Light>();
    assert(comp);
    return comp;
}
