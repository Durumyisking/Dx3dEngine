#pragma once
#include "guiComponent.h"
#include "Material.h"
#include "Mesh.h"


namespace gui
{
	class GUIMeshRenderer : public GUIComponent
	{
	public:
		GUIMeshRenderer();
		~GUIMeshRenderer();

		virtual void FixedUpdate() override;
		virtual void Update() override;
		virtual void LateUpdate() override;
	
		void SetMesh(std::string key);
		void SetMaterial(std::string key);

	private:
		Material*	mMaterial;
		Mesh*		mMesh;
	};
}