#pragma once
#include "Component.h"
#include "Mesh.h"
#include "Material.h"

namespace dru
{
	class CBaseRenderer : public CComponent
	{

	public:
<<<<<<< Updated upstream
		CBaseRenderer(eComponentType _Type);
		virtual ~CBaseRenderer();
=======
		BaseRenderer(eComponentType type);
		virtual ~BaseRenderer();
>>>>>>> Stashed changes

		virtual void Initialize() override;
		virtual void update() override;
		virtual void fixedUpdate() override;
		virtual void render() override;

<<<<<<< Updated upstream
		void SetMesh(std::shared_ptr <CMesh> _Mesh) { mMesh = _Mesh; }
		void SetMaterial(std::shared_ptr <CMaterial> _Material);
		void SetMaterialByKey(std::wstring _Key);
		void SetAnimMaterial(std::shared_ptr <CMaterial> _Material, Vector2 _SpriteSize);
=======
		void SetMesh(Mesh* mesh) { mMesh = mesh; }
		void SetMeshByKey(std::wstring key);
		void SetMaterial(Material* material);
		void SetMaterialByKey(std::wstring key);
		void SetAnimMaterial(Material* material, Vector2 spriteSize);
>>>>>>> Stashed changes


		std::shared_ptr<CMesh> GetMesh() { return mMesh; }
		std::shared_ptr<CMaterial> GetMaterial() { return mMaterial; }

		void ChangeSize() { mbIsChanged = true; }
		void ChangeColor(Vector4 color);
		void MulColor(Vector4 color);
		void AddColor(Vector4 color);

	private:
		void adjustTexture();


	private:
		std::shared_ptr <CMesh> mMesh;
		std::shared_ptr <CMaterial> mMaterial;

		float mWidthRatio;
		float mHeightRatio;

		bool mbIsChanged;
		bool mbIsAnim;

		Vector2 mSpriteSize;


	};

}

