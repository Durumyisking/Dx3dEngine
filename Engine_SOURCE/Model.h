#pragma once
#include "EngineResource.h"
#include "StructedBuffer.h"
#include "ModelNode.h"
#include "Bone.h"


#include "..//External/assimp/include/assimp/Importer.hpp"
#include "..//External/assimp/include/assimp/cimport.h"
#include "..//External/assimp/include/assimp/postprocess.h"
#include "..//External/assimp/include/assimp/scene.h"

#ifdef _DEBUG
#pragma comment(lib, "..//External/assimp/lib/Debug/assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "..//External/assimp/lib/Release/assimp-vc143-mt.lib")
#endif


#define ASSIMP_LOAD_FLAGES (aiProcess_Triangulate  | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace  | aiProcess_FixInfacingNormals | aiProcess_LimitBoneWeights | aiProcess_ConvertToLeftHanded)
#define ASSIMP_TEST_FLAGES (aiProcess_RemoveRedundantMaterials | aiProcess_ImproveCacheLocality | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType  | aiProcess_LimitBoneWeights | aiProcess_CalcTangentSpace | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph |)
// aiProcess_RemoveRedundantMaterials | aiProcess_ImproveCacheLocality | aiProcess_ConvertToLeftHanded | aiProcess_Triangulate | aiProcess_JoinIdenticalVertices | aiProcess_SortByPType | aiProcess_PopulateArmatureData | aiProcess_LimitBoneWeights | aiProcess_CalcTangentSpace | aiProcess_OptimizeMeshes | aiProcess_OptimizeGraph
class Mesh;
class Texture;
class Material;
class GameObj;
class Model : public Resource
{
public:

	struct BoneMat
	{
		math::Matrix FinalTransformation;
	};

	struct TextureInfo
	{
		Texture* pTex;
		UINT			texID;
		aiTextureType	type;
		std::wstring	texName;
		std::wstring	texPath = L"";
	};

	template<typename T>
	using ModelVector = std::vector<T>;

	typedef std::map<std::wstring, ModelNode*> NodeMap;
	typedef std::map<std::wstring, Bone*> BoneMap;
public:
	Model();
	virtual ~Model();

	virtual HRESULT Load(const std::wstring& path) override;
	virtual HRESULT LoadFullpath(const std::wstring& path) override;


	ModelNode* FindNode(const std::wstring& nodeName);
	Bone* FindBone(const std::wstring& nodeName);
	Bone* GetBone(UINT index) { return mBones[index]; }
	void BindBoneMatrix();
	void CreateTexture();
	void CreateMaterial();
	std::vector<Texture*> GetTexture(int index);
	Mesh* GetMesh(UINT index) { return mMeshes[index]; }
	Material* GetMaterial(UINT index) { return mMaterials[index]; }

	Material* GetLastMaterial();

	void AddMaterial(Material* mater) { mMaterials.emplace_back(mater); }
	void MeshRenderSwtich(const std::wstring& name, bool renderSwitch = true);
	void AllMeshRenderSwtichOff();

private:
	void recursiveProcessNode(aiNode* node, const aiScene* scene, ModelNode* rootNode);
	void recursiveProcessMesh(aiMesh* mesh, const aiScene* scene, const std::wstring& nodeName);
	std::vector<Model::TextureInfo> processMaterial(aiMaterial* mater, aiTextureType type, const std::wstring& typeName);
	std::vector<Model::TextureInfo> processMaterial(aiMaterial* mater, aiTextureType type);

	void recursiveProcessBoneMatrix(aiMatrix4x4 matrix, const std::wstring& nodeName);

	void release();

public:
	math::Matrix ConvertMatrix(aiMatrix4x4 aimat);
	Material* GetVariableMaterials(UINT index);
	void SetVariableMaterials(UINT index, Material* mater);
	void SetVariableMaterialsByKey(UINT index, const std::wstring& key);
	void Bind_Render(bool bindMaterial = true);
	void Bind_RenderInstance(UINT instanceCount, bool bindMaterial = true);

	size_t GetMeshCount() const { return mMeshes.size(); }

	void ResizeVarialble(int size) { mVariableMaterials.resize(size); }

public:
	GETSET(const std::wstring&, mRootNodeName, RootNodeName)
	GETSET(const std::wstring&, mCurDirectoryPath, CurDirectoryPath)
	GETSET(Model*, mParentModel, ParentModel)
	GETSET(const std::wstring&, mParentTargetBone, ParentTargetBone)
	GETSET(const std::wstring&, mTargetBone, TargetBone)
	GETSET(math::Vector3, mOffsetRotation, OffsetRotation)
	GETSET(bool, mbUseInstance, UseInstance)
	const std::vector<Mesh*>& GetMeshes() { return mMeshes; }
	void SetWorldMatrix(const math::Matrix& worldMatrix) { mOwnerWorldMatrix = worldMatrix; }
	void SetFrameAnimationVector(const std::map<std::wstring, aiMatrix4x4>* animationVector);

private:
	Assimp::Importer mAssimpImporter;

	NodeMap mNodes;
	BoneMap mBoneMap;

	ModelVector<Bone*> mBones;
	ModelVector<Mesh*> mMeshes;
	ModelVector<Material*> mMaterials;
	ModelVector<Material*> mVariableMaterials;

	std::vector<std::vector<TextureInfo>> mTextures;

	const std::map<std::wstring, aiMatrix4x4>* mFrameAnimationVector;

	StructedBuffer* mStructure;

	std::wstring mRootNodeName;
	std::wstring mCurDirectoryPath;

	Model* mParentModel;
	std::wstring mParentTargetBone;
	std::wstring mTargetBone;

	math::Vector3 mOffsetRotation;

	bool mbUseInstance;

	math::Matrix mOwnerWorldMatrix;

};