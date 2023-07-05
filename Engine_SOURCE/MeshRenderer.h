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

		virtual void Initialize() override ;
		virtual void update() override;
		virtual void fixedUpdate() override;
		virtual void render() override;

	};
}