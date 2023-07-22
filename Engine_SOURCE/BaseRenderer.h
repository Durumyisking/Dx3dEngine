#pragma once
#include "Component.h"



using namespace math;
class Mesh;
class Material;
class BaseRenderer : public Component
{

public:
	BaseRenderer(eComponentType type);
	virtual ~BaseRenderer();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	void SetMesh(Mesh* mesh) { mMesh = mesh; }
	void SetMeshByKey(std::wstring key);
	void SetMaterial(Material* material);
	void SetMaterialByKey(std::wstring key);
	void SetAnimMaterial(Material* material, Vector2 spriteSize);


	Mesh* GetMesh() const { return mMesh; }
	Material* GetMaterial() const { return mMaterial; }

	void ChangeColor(Vector4 color);
	void MulColor(Vector4 color);
	void AddColor(Vector4 color);

	void LOD();
	void LODOn() { mbUseLOD = true; }
	void LODOff() { mbUseLOD = false; }

private:
	Mesh* mMesh;
	Material* mMaterial;


	bool mbIsAnim;
	bool mbUseLOD;

	Vector2 mSpriteSize;


};
