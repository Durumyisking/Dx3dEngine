#include "Light.h"
#include "Transform.h"
#include "GameObj.h"
#include "Renderer.h"
#include "ResourceMgr.h"
#include "Material.h"
#include "Mesh.h"

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
	if (eLightType::Point == mAttribute.type)
	{
		tr->SetScale(Vector3(mAttribute.radius * 5.f, mAttribute.radius * 5.f, mAttribute.radius * 5.f));
	}

	Vector3 position = tr->GetPosition();
	mAttribute.position = Vector4(position.x, position.y, position.z, 1.0f);
	mAttribute.direction = Vector4(tr->Forward().x, tr->Forward().y, tr->Forward().z, 0.0f);

	//Matrix view = Matrix::Identity;
	////Vector3 up = Vector3(0.f, 1.f, 0.f);
	//Vector3 up = tr->Up();
	//Vector3 right = tr->Right();
	//Vector3 forward = tr->Forward();

	//Vector3 pos = forward * -50.f;

	//view *= Matrix::CreateTranslation(-pos);

	//view *= XMMatrixLookToLH(pos, forward, up);

	mAttribute.view = 
		XMMatrixLookAtLH(
		mAttribute.position, mAttribute.position + mAttribute.direction, tr->Up());

	mAttribute.projection = XMMatrixPerspectiveFovLH(
		XM_PI / 4.0f, 1600.f / 900.f, 1.f, 100.f);

	mAttribute.inverseProjection = mAttribute.projection.Invert();
	//= Matrix::CreatePerspectiveFieldOfView
	//(
	//	XM_PI / 4.0f
	//	, 1600.f / 900.f
	//	, 1.f
	//	, 100.f
	//);

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
