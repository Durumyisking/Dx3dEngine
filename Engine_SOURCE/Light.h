#pragma once
#include "Component.h"
#include "Graphics.h"



class Light :
    public Component
{
public:
	Light();
	virtual ~Light();

	virtual void Initialize();
	virtual void Update();
	virtual void FixedUpdate();
	virtual void Render();

	LightAttribute& GetAttribute() { return mAttribute; }

	void SetDiffuse(math::Vector4 diffuse) { mAttribute.diffuse = diffuse; }
	void SetSpecular(math::Vector4 spec) { mAttribute.specular = spec; }
	void SetAmbient(math::Vector4 ambient) { mAttribute.ambient = ambient; }
	void SetType(enums::eLightType type) { mAttribute.type = type; }
	void SetRadius(float radius) { mAttribute.radius = radius; }
	void SetAngle(float angle) { mAttribute.angle = angle; }

	math::Vector4 GetDiffuse() { return mAttribute.diffuse ; }
	math::Vector4 GetAmbient() { return mAttribute.ambient; }
	enums::eLightType GetType() { return mAttribute.type; }
	float GetRadius() { return mAttribute.radius; }
	float GetAngle() { return mAttribute.angle; }



private:
	LightAttribute mAttribute;
};
