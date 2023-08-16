#pragma once
#include "BaseRenderer.h"
#include "Mesh.h"
#include "Material.h"

class SkyboxRenderer : public BaseRenderer
{
public:
	SkyboxRenderer();
	virtual ~SkyboxRenderer();

	virtual void Initialize() final;
	virtual void Update() final;
	virtual void FixedUpdate() final;
	virtual void Render() final;

};
