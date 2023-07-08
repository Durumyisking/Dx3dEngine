#pragma once
#include "guiComponent.h"

namespace gui
{
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
		std::shared_ptr<class dru::graphics::Material> mMaterial;
		std::shared_ptr<class dru::Mesh> mMesh;
	};
}