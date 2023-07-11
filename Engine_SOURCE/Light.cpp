#include "Light.h"
#include "Transform.h"
#include "GameObj.h"
#include "Renderer.h"

namespace dru
{
	Light::Light()
		: Component(eComponentType::Light)
	{

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
		math::Vector3 position = tr->GetWorldPosition();

		mAttribute.position = Vector4(position.x, position.y, position.z, 1.f);
		mAttribute.direction = Vector4(tr->Forward().x, tr->Forward().y, tr->Forward().z, 0.0f);

		renderer::PushLightAttribute(mAttribute);
	}

	void Light::Render()
	{
	}


}