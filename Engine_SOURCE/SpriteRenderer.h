#pragma once
#include "BaseRenderer.h"
#include "Mesh.h"
#include "Material.h"



	 
class SpriteRenderer : public BaseRenderer
{
public:
	SpriteRenderer();
	virtual ~SpriteRenderer();

	virtual void Initialize() final;
	virtual void Update() final;
	virtual void FixedUpdate() final;
	virtual void Render() final;

};

