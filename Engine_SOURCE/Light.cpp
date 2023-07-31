#include "Light.h"
#include "Transform.h"
#include "GameObj.h"
#include "Renderer.h"
#include "ResourceMgr.h"
#include "Material.h"

Light::Light()
	: Component(eComponentType::Light)
	, mVolumeMesh(nullptr)
	, mIndex(0)
{
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

	renderer::PushLightAttribute(mAttribute);
}

void Light::Render()
{
	Material* material = nullptr;

	if (mAttribute.type == eLightType::Directional)
	{
		material = GETSINGLE(ResourceMgr)->Find<Material>(L"LightDirMaterial");
	}
	else if (mAttribute.type == eLightType::Point)
	{
		material = GETSINGLE(ResourceMgr)->Find<Material>(L"LightPointMaterial");
	}


	if (material == nullptr)
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
	material->Bind();
	mVolumeMesh->Render();
}

void Light::SetType(eLightType type)
{
	mAttribute.type = type;
	if (mAttribute.type == eLightType::Directional)
	{
		mVolumeMesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"Rectmesh");
	}
	else if (mAttribute.type == eLightType::Point)
	{
		mVolumeMesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"Spheremesh");
	}
	else if (mAttribute.type == eLightType::Spot)
	{
		//
	}
}
