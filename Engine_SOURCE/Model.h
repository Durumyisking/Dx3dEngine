#pragma once
#include "EngineResource.h"
#include "StructedBuffer.h"
#include "ModelNode.h"
#include "Bone.h"


#include "..//External/assimp/include/assimp/Importer.hpp"
#include "..//External/assimp/include/assimp/cimport.h"
#include "..//External/assimp/include/assimp/postprocess.h"
#include "..//External/assimp/include/assimp/scene.h"

#pragma comment(lib, "..//External/assimp/lib/Debug/assimp-vc143-mtd.lib")
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
		Texture*		pTex;
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

	ModelNode* FindNode(const std::wstring& nodeName);
	Bone* FindBone(const std::wstring& nodeName);
	Bone* GetBone(UINT index) { return mBones[index]; }
	void BindBoneMatrix();
	void CreateTexture();
	void CreateMaterial();
	std::vector<Texture*> GetTexture(int index);
	Material* GetMaterial(UINT index) { return mMaterials[index]; }

	void AddMaterial(Material* mater) { mMaterials.emplace_back(mater); }
private:
	void recursiveProcessNode(aiNode* node, const aiScene* scene, ModelNode* rootNode);
	void recursiveProcessMesh(aiMesh* mesh, const aiScene* scene, const std::wstring& nodeName);
	std::vector<Model::TextureInfo> processMaterial(aiMaterial* mater, aiTextureType type, const std::wstring& typeName);
	std::vector<Model::TextureInfo> processMaterial(aiMaterial* mater, aiTextureType type);

	void recursiveProcessBoneMatrix(aiMatrix4x4 matrix, const std::wstring& nodeName);

	void release();
public:
	math::Matrix ConvertMatrix(aiMatrix4x4 aimat);

public:
	void Bind_Render();

	GETSET(const std::wstring&, mRootNodeName, RootNodeName)
	GETSET(const std::wstring&, mCurDirectoryPath, CurDirectoryPath)
	GETSET(GameObj*, mOwner, Owner)

private:
	Assimp::Importer mAssimpImporter;

	NodeMap mNodes;
	BoneMap mBoneMap;

	ModelVector<Bone*> mBones;
	ModelVector<Mesh*> mMeshes;
	ModelVector<Material*> mMaterials;

	std::vector<std::vector<TextureInfo>> mTextures;

	StructedBuffer* mStructure;
	GameObj* mOwner;

	std::wstring mRootNodeName;
	std::wstring mCurDirectoryPath;
};