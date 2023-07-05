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

	void Light::update()
	{
	}

	void Light::fixedUpdate()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		math::Vector3 position = tr->GetWorldPosition();

		mAttribute.position = Vector4(position.x, position.y, position.z, 1.f);
		mAttribute.direction = Vector4(tr->Right().x, tr->Right().y, tr->Right().z, 1.f); // right를 정면으로 둔다.

		renderer::PushLightAttribute(mAttribute);
	}

	void Light::render()
	{
	}


}