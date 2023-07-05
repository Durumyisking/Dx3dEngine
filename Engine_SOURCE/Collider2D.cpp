#include "Collider2D.h"
#include "GameObj.h"
#include "Renderer.h"
#include "Script.h"

namespace dru
{
	UINT32 Collider2D::colliderID = 0;

	Collider2D::Collider2D()
		: Component(eComponentType::Collider)
		, mState(eCollisionState::End)
		, mType(eColliderType::End)
		, mTransform(nullptr)
		, mScale(Vector2::One)
		, mCenter(Vector2::Zero)
		, mbTrigger(false) // 이거 공부하자
		, mRadius(0.f)
		, mColliderID(colliderID++)
		, mbOn(true)
		, mbRenderOn(true)
	{

	}

	Collider2D::~Collider2D()
	{
	}

	void Collider2D::Initialize()
	{
		mTransform = GetOwner()->GetComponent<Transform>();
		//mbOn = true;
		//mbRenderOn = true;
	}

	void Collider2D::update()
	{
		if (!mbRenderOn)
		{
			mState = eCollisionState::CollisionOff;
		}
	}

	void Collider2D::fixedUpdate()
	{
		if (nullptr == mTransform)
			return;

		Vector3 scale = mTransform->GetScale();
		scale *= Vector3(mScale.x, mScale.y, 1.f);

		Vector3 rotation = mTransform->GetRotation();

		Vector3 position = mTransform->GetWorldPosition();
		mPosition = position;// +Vector3(mCenter.x, mCenter.y, 0.f);
		mPosition.z = 0.f;
		DebugMesh meshAttribute = {};
		meshAttribute.position = Vector3(mPosition.x, mPosition.y, mPosition.z);
		meshAttribute.radius = mRadius;
		meshAttribute.rotation = rotation;
		meshAttribute.scale = scale;
		meshAttribute.type = mType;
		meshAttribute.state = mState;
		

		renderer::debugMeshes.push_back(meshAttribute);
	}

	void Collider2D::render()
	{

	}

	void Collider2D::OnCollisionEnter(Collider2D* _oppo)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollisionEnter(_oppo);
		}
	}

	void Collider2D::OnCollision(Collider2D* _oppo)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollision(_oppo);
		}
	}

	void Collider2D::OnCollisionExit(Collider2D* _oppo)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnCollisionExit(_oppo);
		}
	}

	void Collider2D::OnTriggerEnter(Collider2D* _oppo)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTriggerEnter(_oppo);
		}
	}

	void Collider2D::OnTrigger(Collider2D* _oppo)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTrigger(_oppo);
		}
	}

	void Collider2D::OnTriggerExit(Collider2D* _oppo)
	{
		const std::vector<Script*>& scripts = GetOwner()->GetScripts();
		for (Script* script : scripts)
		{
			script->OnTriggerExit(_oppo);
		}
	}

}