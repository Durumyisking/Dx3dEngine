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

		void SetMesh(Mesh* _Mesh) { mMesh = _Mesh; }
		void SetMeshByKey(std::wstring _Key);
		void SetMaterial(Material* _Material);
		void SetMaterialByKey(std::wstring _Key);
		void SetAnimMaterial(Material* _Material, Vector2 _SpriteSize);


		Mesh* GetMesh() const { return mMesh; }
		Material* GetMaterial() const { return mMaterial; }

		void ChangeSize() { mbIsChanged = true; }
		void ChangeColor(Vector4 _color);
		void MulColor(Vector4 _color);
		void AddColor(Vector4 _color);

		void LOD();
		void LODOn() { mbUseLOD = true; }
		void LODOff() { mbUseLOD = false; }

	private:
		void adjustTexture();


	private:
		Mesh* mMesh;
		Material* mMaterial;

		float mWidthRatio;
		float mHeightRatio;

		bool mbIsChanged;
		bool mbIsAnim;
		bool mbUseLOD;

		Vector2 mSpriteSize;


	};

}

