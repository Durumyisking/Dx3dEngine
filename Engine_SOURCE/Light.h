#pragma once
#include "Component.h"
#include "Graphics.h"

class Mesh;
class Material;
class Light :
    public Component
{
public:
	Light();
	virtual ~Light();

	virtual void Initialize() final;
	virtual void Update() final;
	virtual void FixedUpdate() final;
	virtual void Render() final;
	virtual void PrevRender() final;

	void DeferredLightRender();

	void SetType(eLightType type);

	LightAttribute& GetAttribute() { return mAttribute; }

	void SetDiffuse(math::Vector4 diffuse) { mAttribute.diffuse = diffuse; }
	void SetSpecular(math::Vector4 spec) { mAttribute.specular = spec; }
	void SetAmbient(math::Vector4 ambient) { mAttribute.ambient = ambient; }

	void SetRadius(float radius) { mAttribute.radius = radius; }
	void SetFallOffStart(float fallOffStart) { mAttribute.fallOffStart= fallOffStart; }
	void SetFallOffEnd(float fallOffEnd) { mAttribute.fallOffEnd = fallOffEnd; }
	void SetSpotPower(float spotPower) { mAttribute.spotPower = spotPower; }

	math::Vector4 GetDiffuse() { return mAttribute.diffuse ; }
	math::Vector4 GetAmbient() { return mAttribute.ambient; }
	enums::eLightType GetType() { return mAttribute.type; }

	float GetRadius() const { return mAttribute.radius; }
	float GetFallOffStart(float fallOffStart) const { return  mAttribute.fallOffStart; }
	float GetFallOffEnd(float fallOffEnd) const { return mAttribute.fallOffEnd; }
	float GetSpotPower(float spotPower) const {	return mAttribute.spotPower; }

	void SetIndex(UINT idx) { mIndex = idx; }


private:
	LightAttribute mAttribute;
	Mesh* mVolumeMesh;
	Material* mMaterial;
	UINT mIndex;
};
