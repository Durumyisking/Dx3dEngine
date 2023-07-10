#pragma once
#include "BaseRenderer.h"
#include "Mesh.h"
#include "Material.h"

namespace dru
{
	class MeshRenderer : public BaseRenderer
	{
	public:
		MeshRenderer();
		virtual ~MeshRenderer();

		virtual void Initialize() final ;
		virtual void update() final;
		virtual void fixedUpdate() final;
		virtual void render() final;

	};
}