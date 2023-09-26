#include "Camera.h"
#include "Transform.h"
#include "GameObj.h"
#include "Application.h"
#include "Renderer.h"
#include "Scene.h"
#include "Material.h"
#include "BaseRenderer.h"
#include "TimeMgr.h"
#include "Layer.h"
#include "ResourceMgr.h"
#include "InputMgr.h"

#include "Physical.h"
#include "Object.h"

#include "PhysXRayCast.h"

extern Application application;



Matrix Camera::View = Matrix::Identity;
Matrix Camera::InverseView = Matrix::Identity;
Matrix Camera::Projection = Matrix::Identity;
Matrix Camera::SkyFov = Matrix::Identity;

Camera::Camera()
	: Component(eComponentType::Camera)
	, mType(eProjectionType::Perspective)
	, mAspectRatio(1.f)
	, mNear(1.f)
	, mFar(1000.f)
	, mScale(1.f)
	, mView(Matrix::Identity)
	, mProjection(Matrix::Identity)
	, mTargetObj(nullptr)
	, mCamSpeed(1.f)
	, mCamDir(Vector3::Zero)
	, mFarDist(0.f)
	, mTime(0.3f)
	, mSmooth(false)
	, mRaycastHit{}
	, mRayMaxDist(5.f)
	, mRayMaxHit(1)
{
	EnableLayerMasks();
}
Camera::~Camera()
{
}
void Camera::Initialize()
{
	//RegisterCameraInRenderer();
}

void Camera::Update()
{
	if (mTargetObj) // 타겟 오브젝트를 쫓아감
	{

		Vector2 v2Start = Vector2(GetOwner()->GetPos().x, GetOwner()->GetPos().y);
		Vector2 v2Dest = Vector2(mTargetObj->GetPos().x, mTargetObj->GetPos().y);

		mFarDist = (v2Start - v2Dest).Length();

		if (mSmooth)
			mCamSpeed = mFarDist / mTime;

		if (mFarDist < 0.001f)
			mCamSpeed = 1.f;

		Vector3 Dir = mTargetObj->GetPos() - GetOwner()->GetPos();
		Dir.z = GetOwner()->GetPos().z;
		(Dir).Normalize(mCamDir);
	}
}

void Camera::FixedUpdate()
{
	CreateViewMatrix();
	CreateProjectionMatrix();

	RegisterCameraInRenderer();
}

void Camera::Render()
{
	View = mView;
	InverseView = View.Invert();
	Projection = mProjection;

	sortGameObjects();

	// shadow	
	Transform directionLighttr = *(renderer::lights[0]->GetOwner()->GetComponent<Transform>());
	directionLighttr.SetRotation(DecomposeRotMat(directionLighttr.GetWorldRotationMatrix()));

	ConstantBuffer* lightCB = renderer::constantBuffers[static_cast<UINT>(eCBType::LightMatrix)];

	LightMatrixCB data = {};
	data.lightView = CreateViewMatrix(&directionLighttr);
	data.lightProjection = CreateProjectionMatrix(eProjectionType::Perspective, static_cast<float>(application.GetWidth()), static_cast<float>(application.GetHeight()), 1.0f, 1000.0f);
	lightCB->SetData(&data);
	lightCB->Bind(eShaderStage::VS);	
	lightCB->Bind(eShaderStage::PS);

	renderTargets[static_cast<UINT>(eRenderTargetType::Shadow)]->OMSetRenderTarget();
	renderShadow();

	// Forward Render
	renderTargets[static_cast<UINT>(eRenderTargetType::Swapchain)]->OMSetRenderTarget();
	renderOpaque();
	renderCutout();
	renderTransparent();

	// Deferred Opaque Render 
	renderTargets[static_cast<UINT>(eRenderTargetType::Deferred)]->OMSetRenderTarget();
	renderDeferred();

	// Deferred light Render
	renderTargets[static_cast<UINT>(eRenderTargetType::Light)]->OMSetRenderTarget();

	for (Light* light : renderer::lights)
	{
		light->Render();
	}

	if (mLayerMask[static_cast<UINT>(eLayerType::UI)])
		return;

	//SwapChain
	renderTargets[static_cast<UINT>(eRenderTargetType::Swapchain)]->OMSetRenderTarget();

	// Deferred + SwapChain Merge
	Material* mergeMaterial = GETSINGLE(ResourceMgr)->Find<Material>(L"MergeMRT_Material");
	Mesh* rectMesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"Rectmesh");
	rectMesh->BindBuffer();
	mergeMaterial->Bind();
	rectMesh->Render();

}

void Camera::CreateViewMatrix()
{
	Transform* transform = GetOwner()->GetComponent<Transform>();

	// 이동정보
	Vector3 translation = transform->GetPosition();

	
	// 회전정보
	Vector3 up = transform->Up();
	Vector3 right = transform->Right();
	Vector3 foward = transform->Forward();
	
	//create view translation matrix
	mView = Matrix::Identity;
	mView *= Matrix::CreateTranslation(-translation);

	Matrix viewRotate;
	viewRotate._11 = right.x; 		viewRotate._12 = up.x;		 viewRotate._13 = foward.x;
	viewRotate._21 = right.y; 		viewRotate._22 = up.y;		 viewRotate._23 = foward.y;
	viewRotate._31 = right.z; 		viewRotate._32 = up.z;		 viewRotate._33 = foward.z;

	mView *= viewRotate;

}

Matrix Camera::CreateViewMatrix(Transform* tr)
{
	Matrix view = Matrix::Identity;
	//Vector3 up = Vector3(0.f, 1.f, 0.f);
	Vector3 up = tr->Up();
	Vector3 right = tr->Right();
	Vector3 forward = tr->Forward();

	Vector3 pos = forward * -50.f;

	view *= Matrix::CreateTranslation(-pos);

	view *= XMMatrixLookToLH(pos, forward, up);	

	
	return view;

	////새로 적용할 회전행렬
	//mView = XMMatrixLookAtLH(translation, foward, up);
}

void Camera::CreateProjectionMatrix()
{
	RECT winRect;
	GetClientRect(application.GetHwnd(), &winRect);

	float width = (winRect.right - winRect.left) * mScale;
	float height = (winRect.bottom - winRect.top) * mScale;
	mAspectRatio = width / height;

	SkyFov = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f , mAspectRatio, mNear, mFar);


	if (mType == eProjectionType::Perspective)
	{
		mProjection = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f, mAspectRatio, mNear, mFar);
	}	
	else // (mType == eProjectionType::Orthographic)
	{
		mProjection = Matrix::CreateOrthographic(width, height, mNear, mFar);
	}

}

Matrix Camera::CreateProjectionMatrix(eProjectionType type, float width, float height, float Near, float Far)
{
	Matrix proj = Matrix::Identity;

	float AspectRatio = width / height;
	if (mType == eProjectionType::Perspective)
	{
		proj = Matrix::CreatePerspectiveFieldOfView
		(
			XM_PI / 4.0f
			, AspectRatio
			, Near
			, Far
		);
	}
	else
	{
		proj = Matrix::CreateOrthographicLH(width /*/ 100.0f*/, height /*/ 100.0f*/, Near, Far);
	}

	return proj;
}

void Camera::RegisterCameraInRenderer()
{
	UINT type = static_cast<UINT>(GETSINGLE(SceneMgr)->GetActiveScene()->GetType());
	renderer::Cameras[type].push_back(this);
}

void Camera::TurnLayerMask(eLayerType layer, bool enable)
{
	mLayerMask.set(static_cast<UINT>(layer, enable));
}

void Camera::SetTarget(GameObj* target)
{
	mTargetObj = target;

	Vector3 Dir = mTargetObj->GetPos() - GetOwner()->GetPos();
	Dir.z = GetOwner()->GetPos().z;

	(Dir).Normalize(mCamDir);
}

bool Camera::Raycast(const Vector3& origin, const Vector3& dir, GameObj* gameObject, float maxDistance)
{


	if (this == renderer::mainCamera)
	{
		Physical* physical = gameObject->GetComponent<Physical>();
		if (!physical)
		{
			return false;
		}
		eGeometryType geometryType = physical->GetGeometryType();
		PxTransform pxTransform = gameObject->GetComponent<Transform>()->GetPxTransform();

		switch (geometryType)
		{
		case eGeometryType::Box:
		{
			PxBoxGeometry boxGeom = physical->GetGeometries()->boxGeom;
			bool bResult = PxGeometryQuery::raycast(
				convert::Vector3ToPxVec3(origin),
				convert::Vector3ToPxVec3(dir),
				boxGeom, pxTransform,
				maxDistance,
				PxHitFlag::ePOSITION | PxHitFlag::eDEFAULT,
				mRayMaxHit,
				&mRaycastHit);

			return bResult;
		}
		break;

		case eGeometryType::Capsule:
		{
			PxCapsuleGeometry capsuleGeom = physical->GetGeometries()->capsuleGeom;
			bool bResult = PxGeometryQuery::raycast(
				convert::Vector3ToPxVec3(origin),
				convert::Vector3ToPxVec3(dir),
				capsuleGeom, pxTransform,
				maxDistance,
				PxHitFlag::ePOSITION | PxHitFlag::eDEFAULT,
				mRayMaxHit,
				&mRaycastHit);

			return bResult;
		}
		break;

		case eGeometryType::Sphere:
		{
			PxSphereGeometry sphereGeom = physical->GetGeometries()->sphereGeom;

			bool bResult = PxGeometryQuery::raycast(
				convert::Vector3ToPxVec3(origin),
				convert::Vector3ToPxVec3(dir),
				sphereGeom, pxTransform,
				maxDistance,
				PxHitFlag::ePOSITION | PxHitFlag::eDEFAULT,
				mRayMaxHit,
				&mRaycastHit);

			return bResult;
		}
		break;
		}

		return false;
	}

	return false;
}


void Camera::sortGameObjects()
{
	mDeferredOpaqueGameObjects.clear();
	mOpaqueGameObjects.clear();
	mCutoutGameObjects.clear();
	mTransparentGameObjects.clear();
	mPostProcessGameObjects.clear();

	Scene* scene = GETSINGLE(SceneMgr)->GetActiveScene();
	for (size_t i = 0; i < static_cast<UINT>(eLayerType::End); i++)
	{
		if (mLayerMask[i])
		{
			Layer& layer = scene->GetLayer((eLayerType)i);

			GameObjects gameObjects = layer.GetGameObjects();

			if (0 == gameObjects.size())
				continue;

			for (GameObj* obj : gameObjects)
			{
				pushGameObjectToRenderingModes(obj);
			}
		}
	}

}

void Camera::renderShadow()
{
	for (GameObj* obj : mDeferredOpaqueGameObjects)
	{
		if (obj == nullptr)
			continue;

		obj->PrevRender();
	}

	for (GameObj* obj : mOpaqueGameObjects)
	{
		if (obj == nullptr)
			continue;

		obj->PrevRender();
	}
}

void Camera::renderDeferred()
{
	for (GameObj* obj : mDeferredOpaqueGameObjects)
	{
		if (renderPassCheck(obj))
		{
			obj->Render();
		}
	}
}

void Camera::renderOpaque()
{
	for (GameObj* obj : mOpaqueGameObjects)
	{
		if (renderPassCheck(obj))
		{
			obj->Render();
		}
	}
}

void Camera::renderCutout()
{
	for (GameObj* obj : mCutoutGameObjects)
	{
		if (renderPassCheck(obj))
		{
			obj->Render();
		}
	}
}

void Camera::renderTransparent()
{
	for (GameObj* obj : mTransparentGameObjects)
	{
		if (renderPassCheck(obj))
		{
			obj->Render();
		}
	}
}

void Camera::renderPostProcess()
{
	for (GameObj* obj : mPostProcessGameObjects)
	{
		if (renderPassCheck(obj))
		{
			renderer::CopyRenderTarget();
			obj->Render();
		}
	}
}


void Camera::pushGameObjectToRenderingModes(GameObj* obj)
{
	BaseRenderer* renderer = obj->GetComponent<BaseRenderer>();

	if ( eLayerType::CubeMap== obj->GetLayerType())
	{
		obj->Render();
	}

	if (nullptr == renderer)
		return;


	Material* material = renderer->GetMaterial();

	eRenderingMode mode = material->GetRenderingMode();

	switch (mode)
	{
	case eRenderingMode::DeferredOpaque:
	case eRenderingMode::DeferredMask:
		mDeferredOpaqueGameObjects.push_back(obj);
		break;
	case eRenderingMode::Opaque:
		mOpaqueGameObjects.push_back(obj);
		break;
	case eRenderingMode::Cutout:
		mCutoutGameObjects.push_back(obj);
		break;
	case eRenderingMode::Transparent:
		mTransparentGameObjects.push_back(obj);
		break;
	case eRenderingMode::PostProcess:
		mPostProcessGameObjects.push_back(obj);
		break;
	default:
		break;
	}
}

bool Camera::renderPassCheck(GameObj* obj)
{
	if (nullptr == obj)
	{
		return false;
	}
	if (obj->IsRenderingBlock())
	{
		return false;
	}
	if (nullptr != obj->GetComponent<Transform>()->GetParent())
	{
		if (obj->GetComponent<Transform>()->GetParent()->GetOwner()->IsRenderingBlock())
		{
			return false;
		}
	}

	return true;
}

void Camera::SetLayerMaskOn(eLayerType type)
{
	if (mLayerMask.test((UINT)type))
		return;

	mLayerMask.flip((UINT)type);
}

void Camera::SetLayerMaskOFF(eLayerType type)
{
	if (!mLayerMask.test((UINT)type))
		return;

	mLayerMask.flip((UINT)type);
}