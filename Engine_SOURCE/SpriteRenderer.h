#pragma once
#include "BaseRenderer.h"
#include "Mesh.h"
#include "Material.h"

namespace dru
{
	 
	class SpriteRenderer : public BaseRenderer
	{
	public:
		SpriteRenderer();
		virtual ~SpriteRenderer();

		virtual void Initialize() final;
		virtual void update() final;
		virtual void fixedUpdate() final;
		virtual void render() final;

	};

}