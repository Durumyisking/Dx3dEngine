#include "Transform.h"
#include "Camera.h"
#include "GameObj.h"
#include "Physical.h"

namespace dru
{
	Transform::Transform()
		: Component(eComponentType::Transform)
		, mParent(nullptr)
		, mRelativeForward(Vector3::Forward)
		, mRelativeRight(Vector3::Right)
		, mRelativeUp(Vector3::Up)
		, mWorldForward(Vector3::Forward)
		, mWorldRight(Vector3::Right)
		, mWorldUp(Vector3::Up)
		, mRelativePosition(Vector3::Zero)
		, mRelativeRotation(Vector3::Zero)
		, mRelativeScale(Vector3::One)
		, mInheritParentScale(true)
		, mWorldPosition(Vector3::Zero)
		, mWorldRotation(Vector3::Zero)
		, mWorldScale(Vector3::One)
		, mbIsScaleChanged(false)
		, mPxWorld(Matrix::Identity)
		, mPxTransform{}
	{
	}

	Transform::~Transform()
	{
	}

	void Transform::Initialize()
	{
		mPxTransform.p = convert::Vector3ToPxVec3(mRelativePosition);
		mPxTransform.q = PxQuat(PxIdentity);
	}

	void Transform::update()
	{
	}

	void Transform::fixedUpdate()
	{

		if (GetOwner()->GetComponent<Physical>())
		{
			Physical* physical = GetOwner()->GetComponent<Physical>();
			mPxTransform = physical->GetActor<PxRigidActor>()->getGlobalPose();

			Matrix matPxScale = Matrix::CreateScale(physical->GetGeometrySize());
			Matrix matPxRotation = Matrix::CreateFromQuaternion(convert::PxQuatToQuaternion(mPxTransform.q));
			Matrix matPxTranslation = Matrix::CreateTranslation(convert::PxVec3ToVector3(mPxTransform.p));
			mPxWorld = matPxScale * matPxRotation * matPxTranslation;

			//Vector3 vLocalTranslation = Vector3(
			//	mPxTransform.p.x + m_vGlobalOffset.x,
			//	mPxTransform.p.y + m_vGlobalOffset.y,
			//	mPxTransform.p.z);
			Vector3 vLocalTranslation = Vector3(
				mPxTransform.p.x ,
				mPxTransform.p.y ,
				mPxTransform.p.z);

			Matrix matTranslation = Matrix::CreateTranslation(vLocalTranslation);
			Matrix matScale = Matrix::CreateScale(mRelativeScale);

//			m_matOldWorld = mWorld;
			mWorld = matScale * matPxRotation * matTranslation;
		}
		else
		{	
			// 렌더링에 사용될 위치값을 업데이트.
			// 1. 월드 행렬 생성
			// - 크기 변환 행렬
			Matrix scale = Matrix::CreateScale(mRelativeScale);
			mWorldScale = mRelativeScale;

			// - 회전 변환 행렬
			Vector3 rot = mRelativeRotation * XM_PI / 180; // to radian
			Matrix rotation;
			rotation = Matrix::CreateRotationX(rot.x);
			rotation *= Matrix::CreateRotationY(rot.y);
			rotation *= Matrix::CreateRotationZ(rot.z);
			mWorldRotation = rot;


			// - 이동 변환 행렬
			Matrix position;
			position.Translation(mRelativePosition);
			mWorldPosition = mRelativePosition;

			mWorld = scale * rotation * position;
			mWorldForward = mRelativeForward = Vector3::TransformNormal(Vector3::Forward, rotation);
			mWorldRight = mRelativeRight = Vector3::TransformNormal(Vector3::Right, rotation);
			mWorldUp = mRelativeUp = Vector3::TransformNormal(Vector3::Up, rotation);

			if (mParent)
			{
				Matrix parentWorld = mParent->GetWorldMatrix();

				if (!mInheritParentScale)
				{
					Vector3 worldPos, worldScale;
					Quaternion worldRot;
					parentWorld.Decompose(worldScale, worldRot, worldPos);
					Matrix parentWorldNoScale = Matrix::CreateScale(1.f) * Matrix::CreateFromQuaternion(worldRot) * Matrix::CreateTranslation(worldPos);
					mWorld = mWorld * parentWorldNoScale;
				}
				else
				{
					mWorld *= parentWorld;
				}

				// - 월드 좌표, 크기, 회전 갱신
				Quaternion worldRot;
				mWorld.Decompose(mWorldScale, worldRot, mWorldPosition);
				Vector3 quatToRadRot = worldRot.ToEuler();
				mWorldRotation = quatToRadRot;

				mWorldForward = Vector3::TransformNormal(mRelativeForward, rotation);
				mWorldRight = Vector3::TransformNormal(mRelativeRight, rotation);
				mWorldUp = Vector3::TransformNormal(mRelativeUp, rotation);
				mWorldForward.Normalize();
				mWorldRight.Normalize();
				mWorldUp.Normalize();
			}
		}
	}

	void Transform::render()
	{
	}

	void Transform::SetConstantBuffer()
	{
		renderer::TransformCB trCb = {};
		trCb.world = mWorld;
		trCb.view = Camera::GetGpuViewMatrix();
		trCb.projection = Camera::GetGpuProjectionMatrix();

		ConstantBuffer* cb = renderer::constantBuffers[static_cast<UINT>(eCBType::Transform)];
		cb->SetData(&trCb);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::HS);
		cb->Bind(eShaderStage::DS);
		cb->Bind(eShaderStage::GS);
		cb->Bind(eShaderStage::PS);
		cb->Bind(eShaderStage::CS);
	}

	void Transform::SetParent(GameObj* parent)
	{
		mParent = parent->GetComponent<Transform>();
	}

	const Vector3& Transform::GetWorldPosition() 
	{
		if (GetOwner()->GetComponent<Physical>())
		{
			return GetPhysicalPosition();
		}
		else
		{
			return mWorldPosition;
		}

	}

	Vector3 Transform::GetPhysicalPosition()
	{
		assert(GetOwner()->GetComponent<Physical>());
		return convert::PxVec3ToVector3(GetOwner()->GetComponent<Physical>()->GetActor<PxRigidActor>()->getGlobalPose().p);
	}

	void Transform::SetPhysicalPosition(const Vector3& position)
	{
		assert(GetOwner()->GetComponent<Physical>());
		mPxTransform.p = convert::Vector3ToPxVec3(position);
		GetOwner()->GetComponent<Physical>()->GetActor<PxRigidActor>()->setGlobalPose(mPxTransform);
	}


}