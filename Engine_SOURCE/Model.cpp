#include "Model.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "def.h"
#include "StructedBuffer.h"
#include "Material.h"
#include "TimeMgr.h"

namespace fs = std::filesystem;

Model::Model()
	: Resource(eResourceType::Model)
	, mOwner(nullptr)
	, mStructure(nullptr)
{
	
}

Model::~Model()
{
	release();
}

HRESULT Model::Load(const std::wstring& path)
{
	std::string sPath = ConvertToString(path.c_str());
	const aiScene* aiscene = mAssimpImporter.ReadFile(sPath, ASSIMP_LOAD_FLAGES | ASSIMP_D3D_FLAGES);

	if (aiscene == nullptr || aiscene->mRootNode == nullptr)
	{
		// 파일 로드 실패
		return E_FAIL;
	}
	
	// https://github.com/assimp/assimp/issues/849
	if (aiscene->mMetaData)
	{
		int32_t UpAxis = 1, UpAxisSign = 1, FrontAxis = 2, FrontAxisSign = 1, CoordAxis = 0, CoordAxisSign = 1;
		double UnitScaleFactor = 1.0;
		for (unsigned MetadataIndex = 0; MetadataIndex < aiscene->mMetaData->mNumProperties; ++MetadataIndex)
		{
			if (strcmp(aiscene->mMetaData->mKeys[MetadataIndex].C_Str(), "UpAxis") == 0)
			{
				aiscene->mMetaData->Get<int32_t>(MetadataIndex, UpAxis);
			}
			if (strcmp(aiscene->mMetaData->mKeys[MetadataIndex].C_Str(), "UpAxisSign") == 0)
			{
				aiscene->mMetaData->Get<int32_t>(MetadataIndex, UpAxisSign);
			}
			if (strcmp(aiscene->mMetaData->mKeys[MetadataIndex].C_Str(), "FrontAxis") == 0)
			{
				aiscene->mMetaData->Get<int32_t>(MetadataIndex, FrontAxis);
			}
			if (strcmp(aiscene->mMetaData->mKeys[MetadataIndex].C_Str(), "FrontAxisSign") == 0)
			{
				aiscene->mMetaData->Get<int32_t>(MetadataIndex, FrontAxisSign);
			}
			if (strcmp(aiscene->mMetaData->mKeys[MetadataIndex].C_Str(), "CoordAxis") == 0)
			{
				aiscene->mMetaData->Get<int32_t>(MetadataIndex, CoordAxis);
			}
			if (strcmp(aiscene->mMetaData->mKeys[MetadataIndex].C_Str(), "CoordAxisSign") == 0)
			{
				aiscene->mMetaData->Get<int32_t>(MetadataIndex, CoordAxisSign);
			}
			if (strcmp(aiscene->mMetaData->mKeys[MetadataIndex].C_Str(), "UnitScaleFactor") == 0)
			{
				aiscene->mMetaData->Get<double>(MetadataIndex, UnitScaleFactor);
			}
		}

		aiVector3D upVec, forwardVec, rightVec;

		upVec[UpAxis] = UpAxisSign * (float)UnitScaleFactor;
		forwardVec[FrontAxis] = FrontAxisSign * (float)UnitScaleFactor;
		rightVec[CoordAxis] = CoordAxisSign * (float)UnitScaleFactor;

		aiMatrix4x4 mat(rightVec.x, rightVec.y, rightVec.z, 0.0f,
			upVec.x, upVec.y, upVec.z, 0.0f,
			forwardVec.x, forwardVec.y, forwardVec.z, 0.0f,
			0.0f, 0.0f, 0.0f, 1.0f);

		// assimp 항등 행렬이 간혹 아닌 경우가있음 (assimp 버그 수정 코드)
		// 따라서 첫 노드 탐색전에 최상위 노드의 행렬을 항등행렬로 만든다
		aiscene->mRootNode->mTransformation = aiMatrix4x4() * mat;
	}

	std::wstring sceneName = ConvertToW_String(aiscene->mName.C_Str());
	mRootNodeName = ConvertToW_String(aiscene->mRootNode->mName.C_Str());

	// 노드 탐색 
	recursiveProcessNode(aiscene->mRootNode, aiscene, nullptr);

	if (mStructure == nullptr)
	{
		mStructure = new StructedBuffer();
		mStructure->Create(sizeof(BoneMat), mBones.size(), eSRVType::SRV, nullptr, true);
	}


	mAssimpImporter.FreeScene();

	return S_OK;
}

ModelNode* Model::FindNode(const std::wstring& nodeName)
{
	// 단어가 포함된 노드를 탐색
	for (auto& iter : mNodes)
	{
		if (iter.second->mName.find(nodeName) != std::wstring::npos)
		{
			return iter.second;
		}
	}

	return nullptr;
}

Bone* Model::FindBone(const std::wstring& nodeName)
{
	// 탐색 실패시 nullptr 반환
	return mBoneMap.find(nodeName) == mBoneMap.end() ? nullptr : mBoneMap.find(nodeName)->second;
}

void Model::RecursiveGetBoneMatirx()
{
	// 루트노드를 시작으로 하위 노드를 탐색하고 
	// mBonse 에 Local, Final 행렬을 채운다
	const ModelNode* node = FindNode(mRootNodeName);
	if (node == nullptr)
		return;

	aiMatrix4x4 rootmat = node->mTransformation;

	for (size_t i = 0; i < node->mChilds.size(); ++i)
	{
		recursiveProcessBoneMatrix(rootmat, node->mChilds[i]->mName);
	}
}

void Model::Bind_Render()
{
	if (mStructure == nullptr)
		return;

	BoneMat boneInfo = {};
	std::vector<BoneMat> boneMat = {};

	// 본 transform 계산
	RecursiveGetBoneMatirx();

	// 탐색후 계산된 본들의 행렬을 배열로 저장후 GPU 에 바인딩
	boneMat.reserve(mBones.size());

	for (Bone* bone : mBones)
	{

		aiMatrix4x4 finalMat = bone->mFinalMatrix;
		boneInfo.FinalTransformation = ConvertMatrix(finalMat);
		boneMat.emplace_back(boneInfo);
	}

	mStructure->SetData(boneMat.data(), static_cast<UINT>(boneMat.size()));
	mStructure->BindSRV(eShaderStage::VS, 30);

	// 렌더
	for (size_t i = 0; i < mMeshes.size(); ++i)
	{
		if (mMeshes[i] == nullptr)
			continue;

		if (mMaterials[i] == nullptr)
			continue;

		//텍스처 바인딩
		std::vector<Texture*> Textures = GetTexture(i);
		for (int slot = 0; slot < Textures.size(); ++slot)
		{
			if (Textures[slot] == nullptr)
				continue;

			mMaterials[i]->SetTexture(static_cast<eTextureSlot>(slot), Textures[slot]);
		}

		//머터리얼 바인딩
		mMaterials[i]->Bind();

		// 메쉬들의 바인딩
		mMeshes[i]->BindBuffer();
		mMeshes[i]->Render();

		mMaterials[i]->Clear();
	}

	mStructure->Clear();
	boneMat.clear();
}


void Model::recursiveProcessNode(aiNode* node, const aiScene* scene, ModelNode* rootNode)
{
	std::wstring wNodeName = ConvertToW_String(node->mName.C_Str());
	std::map<std::wstring, ModelNode*>::iterator iter = mNodes.find(wNodeName);
	ModelNode* curNode = nullptr;
	// 인자로 들어온 Node 가 처음 들어 온경우
	if (iter == mNodes.end())
	{
		ModelNode* modelnode = new ModelNode();
		modelnode->mName = wNodeName;
		modelnode->mTransformation = node->mTransformation;
		modelnode->mRootName = rootNode == nullptr ? wNodeName : rootNode->mName;
		modelnode->mRootNode = rootNode;

		mNodes.insert(std::pair<std::wstring, ModelNode*>(modelnode->mName, modelnode));
		curNode = mNodes.find(wNodeName)->second;
	}
	else // 인자로 들어온 Node Name 이 중복된 경우
	{
		curNode = iter->second;
		if (curNode->mRootNode != nullptr)
		{
			rootNode = curNode->mRootNode;
		}
	}

	if (rootNode)
		rootNode->mChilds.emplace_back(curNode);

	// 현재 노드의 메쉬를 순회
	for (UINT i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
		recursiveProcessMesh(aiMesh, scene, wNodeName);
	}

	// 현재 노드의 자식을 순회
	for (UINT i = 0; i < node->mNumChildren; ++i)
	{
		recursiveProcessNode(node->mChildren[i], scene, curNode);
	}
}

void Model::recursiveProcessMesh(aiMesh* mesh, const aiScene* scene, const std::wstring& nodeName)
{
	std::map<std::wstring, ModelNode*>::iterator iter = mNodes.find(nodeName);

	std::vector<renderer::Vertex> vertexes;
	std::vector<UINT> indexes;
	std::vector<Texture> textures;

	// 정점 정보 로드
	vertexes.reserve(mesh->mNumVertices);
	for (UINT i = 0; i < mesh->mNumVertices; ++i)
	{
		renderer::Vertex vertex = {};
		math::Vector3 pos = {};

		// 정점 위치
		pos.x = mesh->mVertices[i].x;
		pos.y = mesh->mVertices[i].y;
		pos.z = mesh->mVertices[i].z;
		vertex.pos = math::Vector4(pos.x, pos.y, pos.z, 1.0f);


		// 정점 노말
		math::Vector3 normal = {};
		normal.x = mesh->mNormals[i].x;
		normal.y = mesh->mNormals[i].y;
		normal.z = mesh->mNormals[i].z;
		vertex.normal = normal;

		// 탄젠트
		math::Vector3 tangent = {};
		tangent.x = mesh->mTangents[i].x;
		tangent.y = mesh->mTangents[i].y;
		tangent.z = mesh->mTangents[i].z;
		vertex.tangent = tangent;

		// UV
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

	// 인덱스 정보 로드
	indexes.reserve(mesh->mNumFaces);
	for (UINT i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		for (UINT j = 0; j < face.mNumIndices; ++j)
		{
			indexes.emplace_back(face.mIndices[j]);
		}
	}

	for (int i = 0; i < mesh->mNumBones; ++i)
	{
		Bone* bone = nullptr;
		aiBone* aiBone = mesh->mBones[i];

		UINT bonIndex = 0;
		// 본 정보가 처음 인경우
		if (mBoneMap.end() == mBoneMap.find(ConvertToW_String(aiBone->mName.C_Str())))
		{
			bone = new Bone();
			bone->mName = ConvertToW_String(aiBone->mName.C_Str());
			bone->mOffsetMatrix = aiBone->mOffsetMatrix;
			bone->mIndex = mBones.size();
			mBones.emplace_back(bone);
			mBoneMap.insert(std::pair<std::wstring, Bone*>(bone->mName, bone));

			bonIndex = bone->mIndex;
		}
		else // 본정보가 있던 경우
		{
			bone = mBoneMap.find(ConvertToW_String(aiBone->mName.C_Str()))->second;
			bone->mOffsetMatrix = aiBone->mOffsetMatrix;
			bonIndex = bone->mIndex;

			mBones[bonIndex]->mOffsetMatrix = aiBone->mOffsetMatrix;
		}


		// 정점에 인덱스와 가중치 설정
		for (int j = 0; j < mesh->mBones[i]->mNumWeights; ++j)
		{
			UINT vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
			float weight = mesh->mBones[i]->mWeights[j].mWeight;

			if (vertexes[vertexID].BlendWeight.x == 0.0f)
			{
				vertexes[vertexID].BlendID.x = bonIndex;
				vertexes[vertexID].BlendWeight.x = weight;
			}
			else if (vertexes[vertexID].BlendWeight.y == 0.0f)
			{
				vertexes[vertexID].BlendID.y = bonIndex;
				vertexes[vertexID].BlendWeight.y = weight;
			}
			else if (vertexes[vertexID].BlendWeight.z == 0.0f)
			{
				vertexes[vertexID].BlendID.z = bonIndex;
				vertexes[vertexID].BlendWeight.z = weight;
			}
			else if (vertexes[vertexID].BlendWeight.w == 0.0f)
			{
				vertexes[vertexID].BlendID.w = bonIndex;
				vertexes[vertexID].BlendWeight.w = weight;
			}
		}
	}

	// material 정보 로드
	if (mesh->mMaterialIndex >= 0)
	{
		// TextureLoad
		aiMaterial* aiMater = scene->mMaterials[mesh->mMaterialIndex];
		std::vector<Model::TextureInfo> textureBuff = {};
		for (int type = static_cast<int>(aiTextureType_NONE); type < static_cast<int>(aiTextureType_UNKNOWN); ++type)
		{
			std::vector<Model::TextureInfo> texInfo = processMaterial(aiMater, (aiTextureType)type);
			textureBuff.insert(textureBuff.end(), texInfo.begin(), texInfo.end());
		}
		mTextures.emplace_back(textureBuff);

		std::vector<TextureInfo>& texInfo = mTextures[mTextures.size() - 1];
		for (auto& tex : texInfo)
		{
			if (tex.texPath == L"")
				continue;

			tex.pTex = new Texture();
			tex.pTex->Load(tex.texPath, tex);
		}

		//Material 생성
		Material* inMaterial = new Material();
		inMaterial->SetRenderingMode(eRenderingMode::Transparent);
		Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"PhongShader");
		inMaterial->SetShader(shader);

		mMaterials.emplace_back(inMaterial);
	}

	// 모델 객체 생성
	Mesh* inMesh = new Mesh();
	inMesh->CreateVertexBuffer(vertexes.data(), vertexes.size());
	inMesh->CreateIndexBuffer(indexes.data(), indexes.size());
	mMeshes.emplace_back(inMesh);

	std::wstring wName = ConvertToW_String(mesh->mName.C_Str());
	inMesh->SetName(wName);
	GETSINGLE(ResourceMgr)->Insert<Mesh>(wName, inMesh);
}

std::vector<Model::TextureInfo>  Model::processMaterial(aiMaterial* mater, aiTextureType type, const std::wstring& typeName)
{
	std::vector<Model::TextureInfo>  outTexVector;
	UINT texCount = mater->GetTextureCount(type);
	for (UINT i = 0; i < texCount; ++i)
	{
		TextureInfo texInfo = {};
		aiString aiStr;
		mater->GetTexture(type, i, &aiStr);
		texInfo.type = type;
		texInfo.texName = typeName;
		texInfo.texPath = GetCurDirectoryPath() + L"/Image/" + ConvertToW_String(aiStr.C_Str());
		outTexVector.emplace_back(texInfo);
	}

	return outTexVector;
}

std::vector<Model::TextureInfo> Model::processMaterial(aiMaterial* mater, aiTextureType type)
{
	std::vector<TextureInfo> outTexVector;
	UINT texCount = mater->GetTextureCount(type);
	for (UINT i = 0; i < texCount; ++i)
	{
		TextureInfo texInfo = {};
		aiString aiStr;
		mater->GetTexture(type, i, &aiStr);
		std::wstring texname = ConvertToW_String(aiStr.C_Str());
		texInfo.texName = texname.substr(0, texname.find_last_of(L"."));
		texInfo.texPath = GetCurDirectoryPath() + L"/Image/" + texname;
		texInfo.type = type;
		outTexVector.emplace_back(texInfo);
	}

	return outTexVector;
}

void Model::CreateTexture()
{
	for (auto& textureVec : mTextures)
	{
		for (TextureInfo& texInfo : textureVec)
		{
			Texture* tex = new Texture();
			tex->Load(texInfo.texPath, texInfo);

			texInfo.texID = tex->GetID();
			texInfo.pTex = tex;

			GETSINGLE(ResourceMgr)->Insert<Texture>(texInfo.texName, texInfo.pTex);
		}
	}
}

std::vector<Texture*> Model::GetTexture(int index)
{
	if (index >= mTextures.size())
		return std::vector<Texture*>{};

	std::vector<Texture*> outTexVector; 
	const std::vector<TextureInfo>& texInfos = mTextures[index];
	for (const TextureInfo& tex : texInfos)
	{
		if(tex.pTex != nullptr)
			outTexVector.emplace_back(tex.pTex);
	}

	return outTexVector;
}

void Model::recursiveProcessBoneMatrix(aiMatrix4x4 matrix, const std::wstring& nodeName)
{
	const ModelNode* modelNode = FindNode(nodeName);
	aiMatrix4x4 transform = modelNode->mTransformation;

	matrix = matrix * transform;

	if (mBoneMap.find(nodeName) != mBoneMap.end())
	{
		Bone* bone = mBoneMap.find(nodeName)->second;
		aiMatrix4x4 glovalInvers = FindNode(L"Scene")->GetTransformation();

		// bone->mOffsetMatrix - 정점을 본 공간으로 이동 ( world, view, projection 비슷한 개념)
		// 바인드 포즈의 역행렬이다
		//matrix - 현재 노드 까지의 변환된 행렬
		// glovalInvers.Inverse() - 항등 행렬의 역 정점을 전역 공간으로 변환?
		// 있든 없든 현재 똑같이 동작하기때문에 아직 정확히는 모르겠다
		bone->mFinalMatrix = glovalInvers.Inverse() * matrix * bone->mOffsetMatrix;
		bone->mLocalMatrix = matrix;

		mBones[bone->mIndex]->mFinalMatrix = bone->mFinalMatrix;
		mBones[bone->mIndex]->mLocalMatrix = matrix;
	}

 	for (size_t i = 0; i < modelNode->mChilds.size(); ++i)
	{
		recursiveProcessBoneMatrix(matrix, modelNode->mChilds[i]->mName);
	}
}

void Model::release()
{
	for (auto& node : mNodes)
	{
		if (node.second == nullptr)
			continue;

		delete node.second;
		node.second = nullptr;
	}

	for (Bone* bone : mBones)
	{
		if (bone == nullptr)
			continue;

		delete bone;
		bone = nullptr;
	}

	for (Material* mater : mMaterials)
	{
		if (mater == nullptr)
			continue;

		delete mater;
		mater = nullptr;
	}

	for (size_t i = 0; i < mTextures.size(); i++)
	{
		for (size_t j = 0; j < mTextures[i].size(); j++)
		{
			if (mTextures[i][j].pTex == nullptr)
				continue;

			delete mTextures[i][j].pTex;
			mTextures[i][j].pTex = nullptr;
		}
	}

	if (mStructure)
	{
		mStructure->Clear();
		delete mStructure;
	}

	mStructure = nullptr;
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
	math::Matrix outMat;
	outMat._11 = aimat.a1, outMat._12 = aimat.a2, outMat._13 = aimat.a3, outMat._14 = aimat.a4;
	outMat._21 = aimat.b1, outMat._22 = aimat.b2, outMat._23 = aimat.b3, outMat._24 = aimat.b4;
	outMat._31 = aimat.c1, outMat._32 = aimat.c2, outMat._33 = aimat.c3, outMat._34 = aimat.c4;
	outMat._41 = aimat.d1, outMat._42 = aimat.d2, outMat._43 = aimat.d3, outMat._44 = aimat.d4;

	return outMat;
}
