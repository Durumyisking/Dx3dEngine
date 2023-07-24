#pragma once
#include "EngineResource.h"
#include "StructedBuffer.h"


#include "..//External/assimp/include/assimp/Importer.hpp"
#include "..//External/assimp/include/assimp/cimport.h"
#include "..//External/assimp/include/assimp/postprocess.h"
#include "..//External/assimp/include/assimp/scene.h"

#pragma comment(lib, "..//External/assimp/lib/Debug/assimp-vc143-mtd.lib")
#define ASSIMP_LOAD_FLAGES (aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace |  aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder)
#define ASSIMP_D3D_FLAGES aiProcess_ConvertToLeftHanded



class Mesh;
namespace dru
{
	class Model : public Resource
	{
	public:
		struct ModelNode
		{
			std::wstring mName = L"";
			math::Matrix mTransformation = {};
			std::vector<Mesh*> mMeshes = {};

			std::wstring mRootName = L"";
		};

		struct BoneMat
		{
			math::Matrix mat;
		};

		typedef std::map<std::wstring, Model::ModelNode> NodeMap;
		typedef std::vector<Mesh::Bone> BoneVector;
		typedef std::vector<Mesh*> MeshVector;
	public:
		Model();
		virtual ~Model();

		virtual HRESULT Load(const std::wstring& path) override;

		const ModelNode* FindNode(const std::wstring& nodeName) const;
		math::Matrix RecursiveGetBoneMatirx(Mesh::Bone& bone);

	public:
		std::wstring ConvertToW_String(const char* str);
		std::string ConvertToString(const wchar_t* str);
		math::Matrix ConvertMatrixt(aiMatrix4x4 aimat);
	private:
		void recursiveProcessNode(aiNode* node, const aiScene* scene, std::wstring rootName);
		void recursiveProcessMesh(aiMesh* mesh, const aiScene* scene, const std::wstring& nodeName);
		void recursiveProcessMaterial();

		void recursiveProcessBoneMatrix(math::Matrix& matrix, const std::wstring& nodeName);

	public:
		void Bind();
		void Render();

		GETSET(const std::wstring&, mRootNodeName, RootNodeName)


	private:
		Assimp::Importer mAssimpImporter;
		std::wstring mRootNodeName;

		NodeMap mNodes;
		BoneVector mBones;
		MeshVector mMeshes;

		StructedBuffer* mStructure;
	};
}

