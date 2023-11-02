#include "Light.h"
#include "Transform.h"
#include "GameObj.h"
#include "Renderer.h"
#include "ResourceMgr.h"
#include "Material.h"
#include "Mesh.h"
#include "SceneMgr.h"
#include "Scene.h"


Light::Light()
	: Component(eComponentType::Light)
	, mVolumeMesh(nullptr)
	, mMaterial(nullptr)
	, mIndex(0)
{
	mIndex = static_cast<UINT>(renderer::lights.size());
	renderer::lights.push_back(this);
}

Light::~Light()
{
}

void Light::Initialize()
{
}

void Light::Update()
{
}

void Light::FixedUpdate()
{
	Transform* tr = GetOwner()->GetComponent<Transform>();
	Vector3 position = tr->GetPosition();
	if (eLightType::Point == mAttribute.type)
	{
		tr->SetScale(Vector3(mAttribute.radius * 5.f, mAttribute.radius * 5.f, mAttribute.radius * 5.f));
	}
	else
	{
		// 카메라의 타겟을 얻는다
		// 그 타겟의 포지션으로 고정한다.

		GameObj* p = renderer::mainCamera->GetOwner();
		if (nullptr != p)
		{
			position = p->GetWorldPos();
			position.y += 12.5f;
		}
	}

	mAttribute.position = Vector4(position.x, position.y, position.z, 1.0f);
	mAttribute.direction = Vector4(tr->Forward().x, tr->Forward().y, tr->Forward().z, 0.0f);	

	mAttribute.view = 
		XMMatrixLookAtLH(
		mAttribute.position, mAttribute.position + mAttribute.direction, tr->Up());

	mAttribute.projection = XMMatrixPerspectiveFovLH(
		toRadian(120.f), 1600.f / 900.f, 1.f, 100.f);

	mAttribute.inverseProjection = mAttribute.projection.Invert();

	 //Vector4 eye(0.0f, 0.0f, 0.0f, 1.0f);
	 //Vector4 xLeft(-1.0f, -1.0f, 0.0f, 1.0f);
	 //Vector4 xRight(1.0f, 1.0f, 0.0f, 1.0f);
	 //eye = Vector4::Transform(eye, mAttribute.projection);
	 //xLeft = Vector4::Transform(xLeft, mAttribute.projection.Invert());
	 //xRight = Vector4::Transform(xRight, mAttribute.projection.Invert());
	 //xLeft /= xLeft.w;
	 //xRight /= xRight.w;
	 //std::cout << "LIGHT_FRUSTUM_WIDTH = " << xRight.x - xLeft.x << std::endl;

	renderer::PushLightAttribute(mAttribute);
}

void Light::Render()
{
}

void Light::PrevRender()
{
}

void Light::DeferredLightRender()
{
	if (mMaterial == nullptr)
		return;

	Transform* tr = GetOwner()->GetComponent<Transform>();
	tr->SetConstantBuffer();

	ConstantBuffer* cb = renderer::constantBuffers[static_cast<UINT>(eCBType::Light)];
	LightCB data = {};
	data.lightCount = static_cast<UINT>(renderer::lights.size());
	data.lightIndex = mIndex;

	cb->SetData(&data);
	cb->Bind(eShaderStage::VS);
	cb->Bind(eShaderStage::PS);


	mVolumeMesh->BindBuffer();
	mMaterial->Bind();
	mVolumeMesh->Render();
}

void Light::SetType(eLightType type)
{
	mAttribute.type = type;
	if (mAttribute.type == eLightType::Directional)
	{
		mVolumeMesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"Rectmesh");
		mMaterial = GETSINGLE(ResourceMgr)->Find<Material>(L"LightDirMaterial");
	}
	else if (mAttribute.type == eLightType::Point)
	{
		mVolumeMesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"Spheremesh");
		mMaterial = GETSINGLE(ResourceMgr)->Find<Material>(L"LightPointMaterial");
	}
	else if (mAttribute.type == eLightType::Spot)
	{
		//
	}
}
