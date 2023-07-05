#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Material.h"

namespace dru
{
	class BaseRenderer : public Component
	{

	public:
		BaseRenderer(eComponentType _Type);
		virtual ~BaseRenderer();

		virtual void Initialize() override;
		virtual void update() override;
		virtual void fixedUpdate() override;
		virtual void render() override;

		void SetMesh(std::shared_ptr <Mesh> _Mesh) { mMesh = _Mesh; }
		void SetMeshByKey(std::wstring _Key);
		void SetMaterial(std::shared_ptr <Material> _Material);
		void SetMaterialByKey(std::wstring _Key);
		void SetAnimMaterial(std::shared_ptr <Material> _Material, Vector2 _SpriteSize);


		std::shared_ptr<Mesh> GetMesh() { return mMesh; }
		std::shared_ptr<Material> GetMaterial() { return mMaterial; }

		void ChangeSize() { mbIsChanged = true; }
		void ChangeColor(Vector4 _color);
		void MulColor(Vector4 _color);
		void AddColor(Vector4 _color);

	private:
		void adjustTexture();


	private:
		std::shared_ptr <Mesh> mMesh;
		std::shared_ptr <Material> mMaterial;

		float mWidthRatio;
		float mHeightRatio;

		bool mbIsChanged;
		bool mbIsAnim;

		Vector2 mSpriteSize;


	};

}

