#pragma once
#include "guiComponent.h"


class Material;
class Mesh;

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