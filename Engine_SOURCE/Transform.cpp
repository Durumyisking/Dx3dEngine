#include "Transform.h"
#include "Camera.h"
#include "GameObj.h"
#include "Physical.h"

#include "TimeMgr.h"

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
	, mOffsetScale(1.0f)
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
		mTickPerSceond += DT;
		if (mTickPerSceond > mDurationTime)
		{
			mTickPerSceond -= mDurationTime;
		}

		// 쿼터니언을 보간합니다
		if (mArriveQuternion != mCurQuternion)
		{
			mCurQuternion = math::Quaternion::Slerp(mCurQuternion, mArriveQuternion, mTickPerSceond / mDurationTime);
		}

		Physical* physical = GetPhysical();
		if (eActorType::Kinematic == physical->GetActorType())
		{
			physical->GetActor<PxRigidDynamic>()->getKinematicTarget(mPxTransform);
		}
		else
		{
			mPxTransform = physical->GetActor<PxRigidActor>()->getGlobalPose();
		}

		// 보간된 회전을 적용
		mPxTransform.q = PxQuat(mCurQuternion.x, mCurQuternion.y, mCurQuternion.z, mCurQuternion.w);

		Matrix matPxScale = Matrix::CreateScale(physical->GetGeometrySize());

		// 원래 코드
		Matrix matPxRotation = Matrix::CreateFromQuaternion(convert::PxQuatToQuaternion(mPxTransform.q));
		Matrix matPxTranslation = Matrix::CreateTranslation(convert::PxVec3ToVector3(mPxTransform.p));
		mRelativePosition = convert::PxVec3ToVector3(mPxTransform.p);
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
		Matrix matScale = Matrix::CreateScale(mRelativeScale * mOffsetScale);

//			m_matOldWorld = mWorld;
		mWorld = matScale * matPxRotation * matTranslation;

		// 기저세팅
		mWorldForward = mRelativeForward = Vector3::TransformNormal(Vector3::Forward, matPxRotation);
		mWorldRight = mRelativeRight = Vector3::TransformNormal(Vector3::Right, matPxRotation);
		mWorldUp = mRelativeUp = Vector3::TransformNormal(Vector3::Up, matPxRotation);

		mWorldForward.Normalize();
		mWorldRight.Normalize();
		mWorldUp .Normalize();
	}
	else
	{	
		// 렌더링에 사용될 위치값을 업데이트.
		// 1. 월드 행렬 생성
		// - 크기 변환 행렬
		Matrix scale = Matrix::CreateScale(mRelativeScale * mOffsetScale);
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
	trCb.inverseProjection = trCb.projection.Invert();
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

	auto mult = [&](const Vector4& v, const Matrix& m) -> Vector4
		{
			Vector4 result;
			result.x = (v.x * m._11) + (v.y * m._21) + (v.z * m._31) + (v.w * m._41);
			result.y = (v.x * m._12) + (v.y * m._22) + (v.z * m._32) + (v.w * m._42);
			result.z = (v.x * m._13) + (v.y * m._23) + (v.z * m._33) + (v.w * m._43);
			result.w = (v.x * m._14) + (v.y * m._24) + (v.z * m._34) + (v.w * m._44);

			return result;
		};

	if (GetOwner()->GetLayerType() == eLayerType::FX)
	{
		Vector4 pos(0.5f);
		pos.w = 1.0f;

		//pos = mult(pos, trCb.world);
		pos = mult(pos, trCb.view);
		pos = mult(pos, trCb.projection);

		Vector3 invView = pos.XYZ() / pos.w;
		pos = Vector4(invView, 1.0f);

		pos = mult(pos, trCb.inverseView);




		Vector3 ndcPos = pos.XYZ() / pos.w;

		int a = 0;

		Vector2 UV = ndcPos * Vector2(0.5f, -0.5f);
		UV += Vector2(0.5f, 0.5f);

		Vector4 invPro = mult(Vector4(1.0f, 1.0f, 1.0f, 1.0f), trCb.inverseProjection);
		//Vector4 invPro = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

		Vector2 rightTop = Vector2(invPro.x, invPro.y);
		float x = math::Interpolation<float>(0, 1, UV.x, -rightTop.x, rightTop.x);
		float y = math::Interpolation<float>(0, 1, UV.y, -rightTop.y, rightTop.y);

		Vector3 viewRay = Vector3(x, y, invPro.z);
		//viewRay *= depth;

		//Vector4 viewPos = mult(Vector4(viewRay, 1.0f), trCb.inverseView);
		//Vector4 worldPos = mult(Vector4(viewRay, 1.0f), trCb.inverseWorld);

		int t = 0;
	}
}

void Transform::SetParent(GameObj* parent)
{
	mParent = parent->GetComponent<Transform>();
}

const Vector3& Transform::GetWorldPosition() 
{
	if (GetOwner()->GetComponent<Physical>())
	{
		return convert::PxVec3ToVector3(mPxTransform.p);
		//return convert::PxVec3ToVector3(GetOwner()->GetComponent<Physical>()->GetActor<PxRigidActor>()->getGlobalPose().p);
	}
	else
	{
		return mWorldPosition;
	}

}

Vector3 Transform::GetPhysicalPosition()
{
	assert(GetPhysical());
	return convert::PxVec3ToVector3(mPxTransform.p);
	//return convert::PxVec3ToVector3(GetOwner()->GetComponent<Physical>()->GetActor<PxRigidActor>()->getGlobalPose().p);
}

Vector3 Transform::GetPhysicalRotation()
{
	assert(GetOwner()->GetComponent<Physical>());
	return convert::PxQuatToQuaternion(GetOwner()->GetComponent<Physical>()->GetActor<PxRigidActor>()->getGlobalPose().q).ToEuler() / XM_PI * 180;
}


void Transform::SetPhysicalPosition(const Vector3& position)
{
	assert(GetPhysical());
	mPxTransform.p = convert::Vector3ToPxVec3(position);
	//GetOwner()->GetComponent<Physical>()->GetActor<PxRigidActor>()->setGlobalPose(mPxTransform);

}

void Transform::SetPhysicalRotation(const Vector3& rotation_degrees)
{
	assert(GetOwner()->GetComponent<Physical>());

	mRelativeRotation = rotation_degrees;

	PxQuat rotationX(toRadian(mRelativeRotation.x), PxVec3(1.0f, 0.0f, 0.0f));
	PxQuat rotationY(toRadian(mRelativeRotation.y), PxVec3(0.0f, 1.0f, 0.0f));
	PxQuat rotationZ(toRadian(mRelativeRotation.z), PxVec3(0.0f, 0.0f, 1.0f));
	// 회전을 적용합니다.
	PxQuat finalRotation = rotationX * rotationY * rotationZ;
	mPxTransform.q = finalRotation;
	//GetOwner()->GetComponent<Physical>()->GetActor<PxRigidActor>()->setGlobalPose(mPxTransform);

	// 보간도착 지점에 쿼터니언
	mArriveQuternion = math::Quaternion(finalRotation.x, finalRotation.y, finalRotation.z, finalRotation.w);
	mTickPerSceond = 0.f;
}

void Transform::SetPhysicalRotation(const PxQuat& quat)
{
	assert(GetOwner()->GetComponent<Physical>());

	PxQuat finalRotation = quat;
	mPxTransform.q = finalRotation;

	// 보간도착 지점에 쿼터니언
	mArriveQuternion = math::Quaternion(finalRotation.x, finalRotation.y, finalRotation.z, finalRotation.w);
	mTickPerSceond = 0.f;
}

void Transform::AddPhysicalRotation(const Vector3& rotation_degrees)
{
	assert(GetOwner()->GetComponent<Physical>());

	mRelativeRotation += rotation_degrees;

	PxQuat rotationX(toRadian(mRelativeRotation.x), PxVec3(1.0f, 0.0f, 0.0f));
	PxQuat rotationY(toRadian(mRelativeRotation.y), PxVec3(0.0f, 1.0f, 0.0f));
	PxQuat rotationZ(toRadian(mRelativeRotation.z), PxVec3(0.0f, 0.0f, 1.0f));
	// 회전을 적용합니다.
	PxQuat finalRotation = rotationX * rotationY * rotationZ;
	mPxTransform.q = finalRotation;
	//GetOwner()->GetComponent<Physical>()->GetActor<PxRigidActor>()->setGlobalPose(mPxTransform);

	mArriveQuternion = math::Quaternion(finalRotation.x, finalRotation.y, finalRotation.z, finalRotation.w);
	mTickPerSceond = 0.f;
}

void Transform::AddPhysicalRotation_Radian(const Vector3& rotation_radian)
{
	assert(GetOwner()->GetComponent<Physical>());

	mRelativeRotation.x += toDegree(rotation_radian.x);
	mRelativeRotation.y += toDegree(rotation_radian.y);
	mRelativeRotation.z += toDegree(rotation_radian.z);

	PxQuat rotationX(toRadian(mRelativeRotation.x), PxVec3(1.0f, 0.0f, 0.0f));
	PxQuat rotationY(toRadian(mRelativeRotation.y), PxVec3(0.0f, 1.0f, 0.0f));
	PxQuat rotationZ(toRadian(mRelativeRotation.z), PxVec3(0.0f, 0.0f, 1.0f));

	// 회전을 적용합니다.
	PxQuat finalRotation = rotationX * rotationY * rotationZ;
	mPxTransform.q = finalRotation;
	//GetOwner()->GetComponent<Physical>()->GetActor<PxRigidActor>()->setGlobalPose(mPxTransform);

	mArriveQuternion = math::Quaternion(finalRotation.x, finalRotation.y, finalRotation.z, finalRotation.w);
	mTickPerSceond = 0.f;
}
