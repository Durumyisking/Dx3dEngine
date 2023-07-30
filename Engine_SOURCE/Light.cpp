#include "Light.h"
#include "Transform.h"
#include "GameObj.h"
#include "Renderer.h"
#include "ResourceMgr.h"
#include "Material.h"

Light::Light()
	: Component(eComponentType::Light)
	, mVolumeMesh(nullptr)
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
		tr->SetScale(Vector3(mAttribute.radius * 2.f, mAttribute.radius * 2.f, mAttribute.radius * 2.f));
	}

	Vector3 position = tr->GetPosition();
	mAttribute.position = Vector4(position.x, position.y, position.z, 1.0f);
	mAttribute.direction = Vector4(tr->Forward().x, tr->Forward().y, tr->Forward().z, 0.0f);

	renderer::PushLightAttribute(mAttribute);
}

void Light::Render()
{
	Material* material = GETSINGLE(ResourceMgr)->Find<Material>(L"LightDirMaterial");

	if (material == nullptr)
	{
		return;
	}

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
		mVolumeMesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"Circlemesh");
	}
	else if (mAttribute.type == eLightType::Spot)
	{
		//
	}
}
