#include "Transform.h"
#include "Camera.h"
#include "GameObj.h"
#include "Physical.h"



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

void Transform::Update()
{
}

void Transform::FixedUpdate()
{

	if (GetOwner()->GetComponent<Physical>())
	{

		Physical* physical = GetOwner()->GetComponent<Physical>();
		mPxTransform = physical->GetActor<PxRigidActor>()->getGlobalPose();
		Matrix matPxScale = Matrix::CreateScale(physical->GetGeometrySize());

		// Test
		Vector3 rot = mRelativeRotation * XM_PI / 180; // to radian
		Matrix Testrotation;
		Testrotation = Matrix::CreateRotationX(rot.x);
		Testrotation *= Matrix::CreateRotationY(rot.y);
		Testrotation *= Matrix::CreateRotationZ(rot.z);


		// 원래 코드
		Matrix matPxRotation = Matrix::CreateFromQuaternion(convert::PxQuatToQuaternion(mPxTransform.q));
		Matrix matPxTranslation = Matrix::CreateTranslation(convert::PxVec3ToVector3(mPxTransform.p));
		mRelativePosition = convert::PxVec3ToVector3(mPxTransform.p);
		mPxWorld = matPxScale * Testrotation * matPxTranslation;

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
		mWorld = matScale * Testrotation * matTranslation;

		mWorldForward = mRelativeForward = Vector3::TransformNormal(Vector3::Forward, Testrotation);
		mWorldRight = mRelativeRight = Vector3::TransformNormal(Vector3::Right, Testrotation);
		mWorldUp = mRelativeUp = Vector3::TransformNormal(Vector3::Up, Testrotation);
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

void Transform::Render()
{
}

void Transform::SetConstantBuffer()
{
	renderer::TransformCB trCb = {};
	trCb.world = mWorld;
	trCb.inverseWorld = mWorld.Invert();

	trCb.worldIT = mWorld;
	//trCb.worldIT.Translation(Vector3::Zero);
	trCb.worldIT = trCb.worldIT.Invert().Transpose();


	trCb.view = Camera::GetGpuViewMatrix();
	trCb.inverseView = trCb.view.Invert();
	trCb.projection = Camera::GetGpuProjectionMatrix();
	trCb.fovForSkySphere= Camera::GetSkySphereFov();
	Vector3 p = renderer::mainCamera->GetOwnerWorldPos();
	trCb.cameraWorldPos = Vector4(p.x, p.y, p.z, 1.f);

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
		return convert::PxVec3ToVector3(GetOwner()->GetComponent<Physical>()->GetActor<PxRigidActor>()->getGlobalPose().p);
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

void Transform::SetPhysicalRotation(const Vector3& rotation_degrees)
{
	assert(GetOwner()->GetComponent<Physical>());

	PxQuat rotationX(PxPi * rotation_degrees.x / 180.0f, PxVec3(1.0f, 0.0f, 0.0f));
	PxQuat rotationY(PxPi * rotation_degrees.y / 180.0f, PxVec3(0.0f, 1.0f, 0.0f));
	PxQuat rotationZ(PxPi * rotation_degrees.z / 180.0f, PxVec3(0.0f, 0.0f, 1.0f));
	// 회전을 적용합니다.
	PxQuat finalRotation = rotationX * rotationY * rotationZ;
	mPxTransform.q = finalRotation;
	GetOwner()->GetComponent<Physical>()->GetActor<PxRigidActor>()->setGlobalPose(mPxTransform);
}