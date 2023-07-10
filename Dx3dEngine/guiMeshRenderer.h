#pragma once
#include "guiComponent.h"

namespace gui
{
	class Material;
	class Mesh;
	class MeshRenderer : public Component
	{
	public:
		MeshRenderer();
		~MeshRenderer();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
	
		void SetMesh(std::string key);
		void SetMaterial(std::string key);

	private:
		dru::Material* mMaterial;
		dru::Mesh* mMesh;
	};
}