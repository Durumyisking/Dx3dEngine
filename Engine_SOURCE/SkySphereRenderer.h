#pragma once
#include "BaseRenderer.h"
#include "Mesh.h"
#include "Material.h"

class SkySphereRenderer : public BaseRenderer
{
public:
	SkySphereRenderer();
	virtual ~SkySphereRenderer();

	virtual void Initialize() final;
	virtual void Update() final;
	virtual void FixedUpdate() final;
	virtual void Render() final;

	void SetSkyBoyTextureByKey(const std::wstring& texureName);

private:
	SkyCB mProjConstantBuffer;
};
