#pragma once
#include "BaseRenderer.h"
#include "Mesh.h"
#include "Material.h"

namespace dru
{
	 
	class CSpriteRenderer : public CBaseRenderer
	{
	public:
		CSpriteRenderer();
		virtual ~CSpriteRenderer();

		virtual void Initialize() final;
		virtual void update() final;
		virtual void fixedUpdate() final;
		virtual void render() final;

	};

}