#pragma once
#include "Component.h"



using namespace math;
class Mesh;
class Material;
class Model;
class BaseRenderer : public Component
{

public:
	BaseRenderer(eComponentType type);
	virtual ~BaseRenderer();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void PrevRender() override {};
	virtual void Render() override;

	void SetMesh(Mesh* mesh) { mMesh = mesh; }
	void SetMeshByKey(std::wstring key);
	void SetMaterial(Material* material, UINT modelMeshSlot = 0);
	void SetMaterialByKey(std::wstring key, UINT modelMeshSlot = 0);
	void SetMaterial(std::wstring key);

	void ForceSetMaterial(Material* material);
	void ForceSetMaterialByKey(std::wstring key);

	void SetAnimMaterial(Material* material, Vector2 spriteSize);

	
	void SetModel(Model* model, Material* mater) { mModel = model, mMaterial = mater; }
	void SetModelByKey(std::wstring key);
	void SetModelByKey(std::wstring modelKey, std::wstring materialKey);
	GETSET(Model*, mModel, Model)


	Mesh* GetMesh() const { return mMesh; }
	Material* GetMaterial() const { return mMaterial; }
	const std::wstring& GetMeshKey() { return mMeshKey; }
	const std::wstring& GetMaterialKey() { return mMaterialKey; }
	void SetMeshKey(const std::wstring& key) { mMeshKey = key; }
	void SetMaterialKey(const std::wstring& key) { mMaterialKey = key; }

	void ChangeColor(Vector4 color);
	void MulColor(Vector4 color);
	void AddColor(Vector4 color);

	void LOD();
	void LODOn() { mbUseLOD = true; }
	void LODOff() { mbUseLOD = false; }

private:
	Mesh* mMesh;
	std::wstring mMeshKey;
	Material* mMaterial;
	std::wstring mMaterialKey;
	Model* mModel;

	bool mbIsAnim;
	bool mbUseLOD;

	Vector2 mSpriteSize;


};
