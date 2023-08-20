#pragma once
#include "BaseRenderer.h"
#include "Mesh.h"
#include "Material.h"
#include "CubeMap.h"



class MeshRenderer : public BaseRenderer
{
public:
	MeshRenderer();
	virtual ~MeshRenderer();

	virtual void Initialize() final ;
	virtual void Update() final;
	virtual void FixedUpdate() final;
	virtual void Render() final;

	TextureHDR t;
	
};