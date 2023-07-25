#include "Model.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "def.h"
#include "StructedBuffer.h"

namespace fs = std::filesystem;
namespace dru {


	dru::Model::Model()
		: Resource(eResourceType::Model)
	{
		mStructure = new StructedBuffer();
	}

	dru::Model::~Model()
	{
		if (mStructure)
		{
			mStructure->Clear();
			delete mStructure;
		}

		mStructure = nullptr;
	}

	HRESULT dru::Model::Load(const std::wstring& path)
	{
		std::string sPath = ConvertToString(path.c_str());
		const aiScene* aiscene = mAssimpImporter.ReadFile(sPath, ASSIMP_LOAD_FLAGES);

		if (aiscene == nullptr || aiscene->mRootNode == nullptr)
		{
			// 파일 로드 실패
			return E_FAIL;
		}

		std::wstring sceneName = ConvertToW_String(aiscene->mName.C_Str());
		mRootNodeName = ConvertToW_String(aiscene->mRootNode->mName.C_Str());
		recursiveProcessNode(aiscene->mRootNode, aiscene, sceneName);

		mStructure->Create(sizeof(BoneMat), mBones.size(), eSRVType::SRV, nullptr, true);

		return S_OK;
	}

	const Model::ModelNode* Model::FindNode(const std::wstring& nodeName) const
	{
		// TODO: 여기에 return 문을 삽입합니다.
		/*auto iter = mNodes.find(nodeName);
		if (iter == mNodes.end())
		{
			return nullptr;
		}*/

		for (auto& iter : mNodes)
		{
			if (iter.second.mName.find(nodeName) != std::wstring::npos)
			{
				return &iter.second;
			}
		}

		return nullptr;
	}

	math::Matrix Model::RecursiveGetBoneMatirx(Mesh::Bone& bone)
	{
		math::Matrix outMat = FindNode(L"Scene")->mTransformation;
		recursiveProcessBoneMatrix(outMat, bone.mBoneName);

		return outMat;
	}

	void Model::Bind()
	{

		BoneMat boneInfo = {};
		std::vector<BoneMat> boneMat;
		for (Mesh::Bone& bone : mBones)
		{
			boneInfo.offsetmat = bone.mOffsetMat;
			boneInfo.mat = RecursiveGetBoneMatirx(bone);
			boneMat.emplace_back(boneInfo);
		}

		mStructure->SetData(boneMat.data(), boneMat.size());
		mStructure->BindSRV(eShaderStage::VS, 10);

		for (Mesh* mesh : mMeshes)
		{
			if (mesh == nullptr)
				continue;

			mesh->BindBuffer();
			mesh->Render();
		}

		mStructure->Clear();
	}

	void Model::Render()
	{
		for (Mesh* mesh : mMeshes)
		{
			if (mesh == nullptr)
				continue;

			//mesh->Render();
		}
	}

	void dru::Model::recursiveProcessNode(aiNode* node, const aiScene* scene, std::wstring rootName)
	{
		std::wstring wNodeName = ConvertToW_String(node->mName.C_Str());

		std::map<std::wstring, ModelNode>::iterator iter = mNodes.find(wNodeName);
		if (iter == mNodes.end())
		{
			ModelNode modelnode = {};
			modelnode.mName = wNodeName;
			modelnode.mRootName = rootName;
			modelnode.mTransformation = ConvertMatrix(node->mTransformation);

			mNodes.insert(std::pair<std::wstring, ModelNode>(modelnode.mName, modelnode));
		}

		for (UINT i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
			recursiveProcessMesh(aiMesh, scene, wNodeName);
		}

		for (UINT i = 0; i < node->mNumChildren; ++i)
		{
			recursiveProcessNode(node->mChildren[i], scene, wNodeName);
		}
	}

	void dru::Model::recursiveProcessMesh(aiMesh* mesh, const aiScene* scene, const std::wstring& nodeName)
	{
		std::map<std::wstring, ModelNode>::iterator iter = mNodes.find(nodeName);

		std::vector<renderer::Vertex> vertexes;
		std::vector<UINT> indexes;
		std::vector<Texture> textures;

		// 정점 정보 로드
		for (UINT i = 0; i < mesh->mNumVertices; ++i)
		{
			renderer::Vertex vertex = {};
			math::Vector3 pos = {};

			pos.x = mesh->mVertices[i].x;
			pos.y = mesh->mVertices[i].y;
			pos.z = mesh->mVertices[i].z;
			vertex.pos = math::Vector4(pos.x, pos.y, pos.z, 1.0f);

			math::Vector3 normal = {};
			normal.x = mesh->mNormals[i].x;
			normal.y = mesh->mNormals[i].y;
			normal.z = mesh->mNormals[i].z;
			vertex.normal = normal;

			math::Vector3 tangent = {};
			tangent.x = mesh->mTangents[i].x;
			tangent.y = mesh->mTangents[i].y;
			tangent.z = mesh->mTangents[i].z;
			vertex.tangent = tangent;

			/*math::Vector3 bitangent = {};
			bitangent .x = mesh->mBitangents[i].x;
			bitangent .y = mesh->mBitangents[i].y;
			bitangent .z = mesh->mBitangents[i].z;
			vertex.biNormal = bitangent;*/

			if (mesh->mTextureCoords[0] != nullptr)
			{
				math::Vector2 uv = {};
				uv.x = mesh->mTextureCoords[0][i].x;
				uv.y = mesh->mTextureCoords[0][i].y;

				vertex.uv = uv;
			}
			else
			{
				vertex.uv = math::Vector2::Zero;
			}

			vertexes.emplace_back(vertex);
		}

		for (UINT i = 0; i < mesh->mNumFaces; ++i)
		{
			aiFace face = mesh->mFaces[i];
			for (UINT j = 0; j < face.mNumIndices; ++j)
			{
				indexes.emplace_back(face.mIndices[j]);
			}
		}

		int numBones = 0;
		for (int i = 0; i < mesh->mNumBones; ++i)
		{
			Mesh::Bone bone = {};
			aiBone* aiBone = mesh->mBones[i];
			bone.mBoneName = ConvertToW_String(aiBone->mName.C_Str());
			bone.mOffsetMat =ConvertMatrix(aiBone->mOffsetMatrix);
			mBones.emplace_back(bone);

			UINT bonIndex = numBones++;
			for (int j = 0; j < mesh->mBones[i]->mNumWeights; ++j)
			{
				UINT vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
				float weight = mesh->mBones[i]->mWeights[j].mWeight;

				if (vertexes[vertexID].mWeight.x == 0.0f)
				{
					vertexes[vertexID].mBones.x = mBones.size() - 1;
					vertexes[vertexID].mWeight.x = weight;
				}
				else if (vertexes[vertexID].mWeight.y == 0.0f)
				{
					vertexes[vertexID].mBones.y = mBones.size() - 1;
					vertexes[vertexID].mWeight.y = weight;
				}
				else if (vertexes[vertexID].mWeight.z == 0.0f)
				{
					vertexes[vertexID].mBones.z = mBones.size() - 1;
					vertexes[vertexID].mWeight.z = weight;
				}
				else if (vertexes[vertexID].mWeight.w == 0.0f)
				{
					vertexes[vertexID].mBones.w = mBones.size() - 1;
					vertexes[vertexID].mWeight.w = weight;
				}
			}
		}

		Mesh* inMesh = new Mesh();
		inMesh->CreateVertexBuffer(vertexes.data(), vertexes.size());
		inMesh->CreateIndexBuffer(indexes.data(), indexes.size());
		mMeshes.emplace_back(inMesh);


		std::wstring wName = ConvertToW_String(mesh->mName.C_Str());
		inMesh->SetName(wName);
		iter->second.mMeshes.emplace_back(inMesh);
		GETSINGLE(ResourceMgr)->Insert<Mesh>(wName, inMesh);
	}

	void dru::Model::recursiveProcessMaterial()
	{

	}

	void Model::recursiveProcessBoneMatrix(math::Matrix& matrix, const std::wstring& nodeName)
	{
		const ModelNode* modelNode = FindNode(nodeName);
		matrix = matrix * modelNode->mTransformation;

		if (L"" == modelNode->mRootName || L"Scene" == modelNode->mRootName)
		{
			return;
		}

		recursiveProcessBoneMatrix(matrix, modelNode->mRootName);
	}

	std::wstring Model::ConvertToW_String(const char* str)
	{
		std::string sName = std::string(str);
		std::wstring wName(sName.begin(), sName.end());
		return wName;
	}

	std::string Model::ConvertToString(const wchar_t* str)
	{
		std::wstring sName = std::wstring(str);
		std::string wName(sName.begin(), sName.end());
		return wName;
	}

	math::Matrix Model::ConvertMatrix(aiMatrix4x4 aimat)
	{
		dru::math::Matrix outMat = dru::math::Matrix::Identity;
		outMat._11 = aimat.a1, outMat._12 = aimat.a2, outMat._13 = aimat.a3, outMat._14 = aimat.a4;
		outMat._21 = aimat.b1, outMat._22 = aimat.b2, outMat._23 = aimat.b3, outMat._24 = aimat.b4;
		outMat._31 = aimat.c1, outMat._32 = aimat.c2, outMat._33 = aimat.c3, outMat._34 = aimat.c4;
		outMat._41 = aimat.d1, outMat._42 = aimat.d2, outMat._43 = aimat.d3, outMat._44 = aimat.d4;

		return outMat;
	}
}