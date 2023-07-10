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

extern dru::Application application;

namespace dru
{
	Matrix Camera::View = Matrix::Identity;
	Matrix Camera::Projection = Matrix::Identity;

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
	{
		EnableLayerMasks();
	}
	Camera::~Camera()
	{
	}
	void Camera::Initialize()
	{
		RegisterCameraInRenderer();
	}

	void Camera::update()
	{
		if (mTargetObj)
		{

			Vector2 v2Start = Vector2(GetOwner()->GetPos().x,  GetOwner()->GetPos().y);
			Vector2 v2Dest = Vector2(mTargetObj->GetPos().x,  mTargetObj->GetPos().y);

			mFarDist = (v2Start - v2Dest).Length();

			if(mSmooth)
				mCamSpeed = mFarDist / mTime;

			if (mFarDist < 0.001f)
				mCamSpeed = 1.f;

			Vector3 Dir = mTargetObj->GetPos() - GetOwner()->GetPos();
			Dir.z = GetOwner()->GetPos().z;
			(Dir).Normalize(mCamDir);
		}

	}

	void Camera::fixedUpdate()
	{
		CreateViewMatrix();
		CreateProjectionMatrix();

		RegisterCameraInRenderer();
	}

	void Camera::render()
	{
		View = mView;
		Projection = mProjection;

		sortGameObjects();

		renderOpaque();
		renderCutout();
		renderTransparent();
	}	

	void Camera::CreateViewMatrix()
	{
		Transform* transform = GetOwner()->GetComponent<Transform>();
		
		// 이동정보
		Vector3 translation = transform->GetPosition();

		// create view translation matrix
		mView = Matrix::Identity;
		mView *= Matrix::CreateTranslation(-translation);
		
		// 회전정보
		Vector3 up = transform->Up();
		Vector3 right = transform->Right();
		Vector3 foward = transform->Forward();

		Matrix viewRotate;
		viewRotate._11 = right.x; 		viewRotate._12 = up.x;		 viewRotate._13 = foward.x;
		viewRotate._21 = right.y; 		viewRotate._22 = up.y;		 viewRotate._23 = foward.y;
		viewRotate._31 = right.z; 		viewRotate._32 = up.z;		 viewRotate._33 = foward.z;

		mView *= viewRotate;

	}
		
	void Camera::CreateProjectionMatrix()
	{
		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);

		float width = (winRect.right - winRect.left) * mScale;
		float height = (winRect.bottom - winRect.top) * mScale;
		mAspectRatio = width / height;


		if (mType == eProjectionType::Perspective)
		{
			mProjection = Matrix::CreatePerspectiveFieldOfView(XM_PI / 4.f, mAspectRatio, mNear, mFar);
		}
		else // (mType == eProjectionType::Orthographic)
		{
			mProjection = Matrix::CreateOrthographic(width , height , mNear, mFar);
		}

	}

	void Camera::RegisterCameraInRenderer()
	{	
		UINT type = static_cast<UINT>(GETSINGLE(SceneMgr)->GetActiveScene()->GetType());
		renderer::	Cameras[type].push_back(this);
	}

	void Camera::TurnLayerMask(eLayerType _layer, bool _enable)
	{
		mLayerMask.set(static_cast<UINT>(_layer, _enable));
	}

	void Camera::SetTarget(GameObj* _Target)
	{
		mTargetObj = _Target;

		Vector3 Dir = mTargetObj->GetPos() - GetOwner()->GetPos();
		Dir.z = GetOwner()->GetPos().z;

		(Dir).Normalize(mCamDir);
	}

	CameraScript* Camera::GetCamScript()
	{
		return GetOwner()->GetScript<CameraScript>();
	}

	void Camera::sortGameObjects()
	{
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

	void Camera::renderOpaque()
	{
		for (GameObj* obj : mOpaqueGameObjects)
		{
			if (renderPassCheck(obj))
			{
				obj->render();
			}
		}
	}

	void Camera::renderCutout()
	{
		for (GameObj* obj : mCutoutGameObjects)
		{
			if (renderPassCheck(obj))
			{
				obj->render();
			}
		}
	}

	void Camera::renderTransparent()
	{
		for (GameObj* obj : mTransparentGameObjects)
		{
			if (renderPassCheck(obj))
			{
				obj->render();				
			}
		}
	}


	void Camera::pushGameObjectToRenderingModes(GameObj* obj)
	{
		BaseRenderer* renderer = obj->GetComponent<BaseRenderer>();

		if (nullptr == renderer)
			return;

		Material* material = renderer->GetMaterial();

		dru::graphics::eRenderingMode mode = material->GetRenderingMode();

		switch (mode)
		{
		case dru::graphics::eRenderingMode::Opaque:
			mOpaqueGameObjects.push_back(obj);
			break;
		case dru::graphics::eRenderingMode::Cutout:
			mCutoutGameObjects.push_back(obj);
			break;
		case dru::graphics::eRenderingMode::Transparent:
			mTransparentGameObjects.push_back(obj);
			break;
		case dru::graphics::eRenderingMode::PostProcess:
			mPostProcessGameObjects.push_back(obj);
			break;
		default:
			break;
		}
	}

	bool Camera::renderPassCheck(GameObj* _obj)
	{
		if (nullptr == _obj)
		{
			return false;
		}
		if (_obj->IsRenderingBlock())
		{
			return false;
		}
		if (nullptr != _obj->GetParent())
		{
			if (_obj->GetParent()->IsRenderingBlock())
			{
				return false;
			}
		}
		
		return true;
	}

}