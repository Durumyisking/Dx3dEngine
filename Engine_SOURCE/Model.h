#pragma once
#include "EngineResource.h"
#include "StructedBuffer.h"


#include "..//External/assimp/include/assimp/Importer.hpp"
#include "..//External/assimp/include/assimp/cimport.h"
#include "..//External/assimp/include/assimp/postprocess.h"
#include "..//External/assimp/include/assimp/scene.h"

#pragma comment(lib, "..//External/assimp/lib/Debug/assimp-vc143-mtd.lib")
#define ASSIMP_LOAD_FLAGES (aiProcess_Triangulate  | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace  | aiProcess_FixInfacingNormals)
#define ASSIMP_D3D_FLAGES aiProcess_ConvertToLeftHanded


class Mesh;
class Texture;
class Material;
class GameObj;
class Model : public Resource
{
public:
	struct ModelNode
	{
		std::wstring mName = L"";
		aiMatrix4x4 mTransformation = {};
		std::vector<Mesh*> mMeshes = {};

		std::wstring mRootName = L"";
		ModelNode* mRootNode = nullptr;
		std::vector<ModelNode*> mChilds;

		GETSET(aiMatrix4x4, mTransformation, Transformation)
	};

	struct BoneMat
	{
		math::Matrix FinalTransformation;
	};

	struct Bone
	{
		UINT mIndex;
		std::wstring mName = L"";

		// 바인드 포즈의 역행렬
		aiMatrix4x4 mOffsetMatrix = {};

		// GlobalInverse * NodeTransform * mOffsetMatrix
		aiMatrix4x4 mFinalMatrix = {};

		// 노드의 계층 정보
		aiMatrix4x4 mLocalMatrix = {};
	};

	struct TextureInfo
	{
		Texture*		pTex;
		UINT			texID;
		aiTextureType	type;
		std::wstring	texName;
		std::wstring	texPath;
	};

	typedef std::map<std::wstring, Model::ModelNode> NodeMap;
	typedef std::vector<Bone> BoneVector;
	typedef std::vector<Mesh*> MeshVector;
	typedef std::vector<TextureInfo> TextureVector;
	typedef std::map<std::wstring, Bone> BoneMap;
public:
	Model();
	virtual ~Model();

	virtual HRESULT Load(const std::wstring& path) override;

	ModelNode* FindNode(const std::wstring& nodeName);
	Bone* FindBone(const std::wstring& nodeName);
	Bone* GetBone(UINT index) { return &mBones[index]; }
	void RecursiveGetBoneMatirx();
	void CreateTexture();
	std::vector<Texture*> GetTexture(int index);

private:
	void recursiveProcessNode(aiNode* node, const aiScene* scene, ModelNode* rootNode);
	void recursiveProcessMesh(aiMesh* mesh, const aiScene* scene, const std::wstring& nodeName);
	std::vector<TextureInfo> processMaterial(aiMaterial* mater, aiTextureType type, const std::wstring& typeName);
	std::vector<TextureInfo> processMaterial(aiMaterial* mater, aiTextureType type);

	void recursiveProcessBoneMatrix(aiMatrix4x4 matrix, const std::wstring& nodeName);

public:
	std::wstring ConvertToW_String(const char* str);
	std::string ConvertToString(const wchar_t* str);
	math::Matrix ConvertMatrix(aiMatrix4x4 aimat);


public:
	void Bind_Render(Material* material);

	GETSET(const std::wstring&, mRootNodeName, RootNodeName)
	GETSET(const std::wstring&, mCurDirectoryPath, CurDirectoryPath)
	GETSET(GameObj*, mOwner, Owner)
	GETSET(Model*, mParentModel, ParentModel)

	void PushChild(Model* model)
	{
		mChildModel.emplace_back(model);
	};

private:
	Assimp::Importer mAssimpImporter;
	std::wstring mRootNodeName;

	NodeMap mNodes;
	BoneMap mBoneMap;

	BoneVector mBones;
	MeshVector mMeshes;
	std::vector<TextureVector> mTextures;

	StructedBuffer* mStructure;
	std::wstring mCurDirectoryPath;

	GameObj* mOwner;
	Model* mParentModel;

	// 현재 폴더의 모든 파일들을 순회하며 모델객체를 생성하기 때문에
	// 자식 노드의 정보가 없는 경우가 있음
	// 예외처리 추가해야함 FileMgr -> ModeLoad 함수
	// 따라서 현재 폴더의 dae 정보만 파일만을 넣어 사용해주세요 
	std::vector<Model*> mChildModel;
};

