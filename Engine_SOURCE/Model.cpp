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
	, mParentModel(nullptr)
	, mStructure(nullptr)
{
}

Model::~Model()
{
	if (mStructure)
	{
		mStructure->Clear();
		delete mStructure;
	}

	mStructure = nullptr;

	for (Model* model : mChildModel)
	{
		if (model == nullptr)
			continue;

		delete model;
		model = nullptr;
	}
}

HRESULT Model::Load(const std::wstring& path)
{
	std::string sPath = ConvertToString(path.c_str());
	const aiScene* aiscene = mAssimpImporter.ReadFile(sPath, ASSIMP_LOAD_FLAGES | ASSIMP_D3D_FLAGES);

	if (aiscene == nullptr || aiscene->mRootNode == nullptr)
	{
		// ���� �ε� ����
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

		// assimp �׵� ����� ��Ȥ �ƴ� ��찡���� (assimp ���� ���� �ڵ�)
		// ���� ù ��� Ž������ �ֻ��� ����� ����� �׵���ķ� �����
		aiscene->mRootNode->mTransformation = aiMatrix4x4() * mat;
	}

	std::wstring sceneName = ConvertToW_String(aiscene->mName.C_Str());
	mRootNodeName = ConvertToW_String(aiscene->mRootNode->mName.C_Str());

	// ��� Ž�� 
	recursiveProcessNode(aiscene->mRootNode, aiscene, nullptr);

	if (mStructure == nullptr)
	{
		mStructure = new StructedBuffer();
		mStructure->Create(sizeof(BoneMat), mBones.size(), eSRVType::SRV, nullptr, true);
	}


	mAssimpImporter.FreeScene();

	return S_OK;
}

Model::ModelNode* Model::FindNode(const std::wstring& nodeName)
{
	// �ܾ ���Ե� ��带 Ž��
	for (auto& iter : mNodes)
	{
		if (iter.second.mName.find(nodeName) != std::wstring::npos)
		{
			return &iter.second;
		}
	}

	return nullptr;
}

Model::Bone* Model::FindBone(const std::wstring& nodeName)
{
	// Ž�� ���н� nullptr ��ȯ
	return mBoneMap.find(nodeName) == mBoneMap.end() ? nullptr : &mBoneMap.find(nodeName)->second;
}

void Model::RecursiveGetBoneMatirx()
{
	// ��Ʈ��带 �������� ���� ��带 Ž���ϰ� 
	// mBonse �� Local, Final ����� ä���
	const ModelNode* node = FindNode(mRootNodeName);
	if (node == nullptr)
		return;

	aiMatrix4x4 rootmat = node->mTransformation;

	for (size_t i = 0; i < node->mChilds.size(); ++i)
	{
		recursiveProcessBoneMatrix(rootmat, node->mChilds[i]->mName);
	}
}

void Model::Bind_Render(Material* material)
{
	if (mStructure == nullptr)
		return;

	BoneMat boneInfo = {};
	std::vector<BoneMat> boneMat = {};

	// �ڽ��� �� ����� �ڽ� Model ��ü�� ���
	// ��� �̸� �ߺ��������� �ش� ��带 �����ϱ�����
	// �ۼ��� �����̴� ���� �ڵ� ���� �ʿ�
	RecursiveGetBoneMatirx();

	for (Model* model : mChildModel)
	{
		if (model == nullptr)
			continue;

		model->Bind_Render(material);
	}

	// Ž���� ���� ������ ����� �迭�� ������ GPU �� ���ε�
	boneMat.reserve(mBones.size());

	for (Bone& bone : mBones)
	{
		aiMatrix4x4 finalMat = bone.mFinalMatrix;
		boneInfo.FinalTransformation = ConvertMatrix(finalMat);
		boneMat.emplace_back(boneInfo);
	}

	mStructure->SetData(boneMat.data(), static_cast<UINT>(boneMat.size()));
	mStructure->BindSRV(eShaderStage::VS, 30);

	// ���� �� ���� Material �޸� �Ҵ��Ͽ� ����ؾ���
	// ���� �ϳ��� Material�� texture �� ���ε��Ͽ� ����ؼ�
	// �𵨵��� Texture �� ���� ����
	for (size_t i = 0; i < mMeshes.size(); ++i)
	{
		if (mMeshes[i] == nullptr)
			continue;

		std::vector<Texture*> Textures = GetTexture(i);
		for (int slot = 0; slot < Textures.size(); ++slot)
		{
			if (Textures[slot] == nullptr)
				continue;

			material->SetTexture(static_cast<eTextureSlot>(slot), Textures[slot]);
		}

		// �޽����� ���ε�
		mMeshes[i]->BindBuffer();
		mMeshes[i]->Render();
	}

	mStructure->Clear();
	boneMat.clear();
}


void Model::recursiveProcessNode(aiNode* node, const aiScene* scene, ModelNode* rootNode)
{
	std::wstring wNodeName = ConvertToW_String(node->mName.C_Str());
	std::map<std::wstring, ModelNode>::iterator iter = mNodes.find(wNodeName);
	ModelNode* curNode = nullptr;
	// ���ڷ� ���� Node �� ó�� ��� �°��
	if (iter == mNodes.end())
	{
		ModelNode modelnode = {};
		modelnode.mName = wNodeName;
		modelnode.mTransformation = node->mTransformation;
		modelnode.mRootName = rootNode == nullptr ? wNodeName : rootNode->mName;
		modelnode.mRootNode = rootNode;

		mNodes.insert(std::pair<std::wstring, ModelNode>(modelnode.mName, modelnode));
		curNode = &(mNodes.find(wNodeName)->second);
	}
	else // ���ڷ� ���� Node Name �� �ߺ��� ���
	{
		curNode = &iter->second;
		if (curNode->mRootNode != nullptr)
		{
			rootNode = curNode->mRootNode;
		}
	}

	if (rootNode)
		rootNode->mChilds.emplace_back(curNode);

	// ���� ����� �޽��� ��ȸ
	for (UINT i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
		recursiveProcessMesh(aiMesh, scene, wNodeName);
	}

	// ���� ����� �ڽ��� ��ȸ
	for (UINT i = 0; i < node->mNumChildren; ++i)
	{
		recursiveProcessNode(node->mChildren[i], scene, curNode);
	}
}

void Model::recursiveProcessMesh(aiMesh* mesh, const aiScene* scene, const std::wstring& nodeName)
{
	std::map<std::wstring, ModelNode>::iterator iter = mNodes.find(nodeName);

	std::vector<renderer::Vertex> vertexes;
	std::vector<UINT> indexes;
	std::vector<Texture> textures;

	// ���� ���� �ε�
	vertexes.reserve(mesh->mNumVertices);
	for (UINT i = 0; i < mesh->mNumVertices; ++i)
	{
		renderer::Vertex vertex = {};
		math::Vector3 pos = {};

		// ���� ��ġ
		pos.x = mesh->mVertices[i].x;
		pos.y = mesh->mVertices[i].y;
		pos.z = mesh->mVertices[i].z;
		vertex.pos = math::Vector4(pos.x, pos.y, pos.z, 1.0f);


		// ���� �븻
		math::Vector3 normal = {};
		normal.x = mesh->mNormals[i].x;
		normal.y = mesh->mNormals[i].y;
		normal.z = mesh->mNormals[i].z;
		vertex.normal = normal;

		// ź��Ʈ
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

	// �ε��� ���� �ε�
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
		Bone bone = {};
		aiBone* aiBone = mesh->mBones[i];

		UINT bonIndex = 0;
		// �� ������ ó�� �ΰ��
		if (mBoneMap.end() == mBoneMap.find(ConvertToW_String(aiBone->mName.C_Str())))
		{
			bone.mName = ConvertToW_String(aiBone->mName.C_Str());
			bone.mOffsetMatrix = aiBone->mOffsetMatrix;
			bone.mIndex = mBones.size();
			mBones.emplace_back(bone);
			mBoneMap.insert(std::pair<std::wstring, Bone>(bone.mName, bone));

			bonIndex = bone.mIndex;
		}
		else // �������� �ִ� ���
		{
			Bone* bone = &(mBoneMap.find(ConvertToW_String(aiBone->mName.C_Str()))->second);
			bone->mOffsetMatrix = aiBone->mOffsetMatrix;
			bonIndex = bone->mIndex;

			mBones[bonIndex].mOffsetMatrix = aiBone->mOffsetMatrix;
		}


		// ������ �ε����� ����ġ ����
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

	// material ���� �ε�
	if (mesh->mMaterialIndex >= 0)
	{
		aiMaterial* aiMater = scene->mMaterials[mesh->mMaterialIndex];
		Model::TextureVector textureBuff = {};
		for (int type = static_cast<int>(aiTextureType_NONE); type < static_cast<int>(aiTextureType_UNKNOWN); ++type)
		{
			Model::TextureVector texInfo = processMaterial(aiMater, (aiTextureType)type);
			textureBuff.insert(textureBuff.end(), texInfo.begin(), texInfo.end());
		}
		mTextures.emplace_back(textureBuff);
	}

	// �� ��ü ����
	Mesh* inMesh = new Mesh();
	inMesh->CreateVertexBuffer(vertexes.data(), vertexes.size());
	inMesh->CreateIndexBuffer(indexes.data(), indexes.size());
	mMeshes.emplace_back(inMesh);


	std::wstring wName = ConvertToW_String(mesh->mName.C_Str());
	inMesh->SetName(wName);
	iter->second.mMeshes.emplace_back(inMesh);
	GETSINGLE(ResourceMgr)->Insert<Mesh>(wName, inMesh);
}

Model::TextureVector Model::processMaterial(aiMaterial* mater, aiTextureType type, const std::wstring& typeName)
{
	Model::TextureVector outTexVector;
	UINT texCount = mater->GetTextureCount(type);
	for (UINT i = 0; i < texCount; ++i)
	{
		TextureInfo texInfo = {};
		aiString aiStr;
		mater->GetTexture(type, i, &aiStr);
		texInfo.type = type;
		texInfo.texName = typeName;
		texInfo.texPath = GetCurDirectoryPath() + L"/" + ConvertToW_String(aiStr.C_Str());
		outTexVector.emplace_back(texInfo);
	}

	return outTexVector;
}

Model::TextureVector Model::processMaterial(aiMaterial* mater, aiTextureType type)
{
	Model::TextureVector outTexVector;
	UINT texCount = mater->GetTextureCount(type);
	for (UINT i = 0; i < texCount; ++i)
	{
		TextureInfo texInfo = {};
		aiString aiStr;
		mater->GetTexture(type, i, &aiStr);
		std::wstring texname = ConvertToW_String(aiStr.C_Str());
		texInfo.texName = texname.substr(0, texname.find_last_of(L"."));
		texInfo.texPath = GetCurDirectoryPath() + L"/" + texname;
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
	std::vector<Texture*> outTex;
	TextureVector& texVec = mTextures[index];
	for (TextureInfo& tex : texVec)
	{
		outTex.emplace_back(tex.pTex);
	}
	return outTex;
}

void Model::recursiveProcessBoneMatrix(aiMatrix4x4 matrix, const std::wstring& nodeName)
{
	const ModelNode* modelNode = FindNode(nodeName);
	aiMatrix4x4 transform = modelNode->mTransformation;
	if (mParentModel)
	{
		// �𵨿� ���� �̸��� ��带 Ž���ϱ� ���� �ۼ����ڵ�
		ModelNode* parentModelNode = mParentModel->FindNode(nodeName);
		if (parentModelNode)
		{
			// �̺κ� ���� ���ɼ� ����
			// - ����� �ʿ�!! -

			// �θ��� �ش� ��� ���������� �� ���� ������ �����Ѵ�.
			// ������ �ش� ����� ��ġ�� ���� �̵��� ������
			// ȸ���� ������ �����.
			Bone* bone = mParentModel->FindBone(nodeName);

			if (bone != nullptr)
			{
				bone = mParentModel->GetBone(bone->mIndex);

				matrix = bone->mLocalMatrix;
			}
		}
	}

	matrix = matrix * transform;

	if (mBoneMap.find(nodeName) != mBoneMap.end())
	{
		Bone* bone = &mBoneMap.find(nodeName)->second;
		aiMatrix4x4 glovalInvers = FindNode(L"Scene")->GetTransformation();

		// bone->mOffsetMatrix - ������ �� �������� �̵� ( world, view, projection ����� ����)
		// ���ε� ������ ������̴�
		//matrix - ���� ��� ������ ��ȯ�� ���
		// glovalInvers.Inverse() - �׵� ����� �� ������ ���� �������� ��ȯ?
		// �ֵ� ���� ���� �Ȱ��� �����ϱ⶧���� ���� ��Ȯ���� �𸣰ڴ�
		bone->mFinalMatrix = glovalInvers.Inverse() * matrix * bone->mOffsetMatrix;
		bone->mLocalMatrix = matrix;

		mBones[bone->mIndex].mFinalMatrix = bone->mFinalMatrix;
		mBones[bone->mIndex].mLocalMatrix = matrix;
	}

 	for (size_t i = 0; i < modelNode->mChilds.size(); ++i)
	{
		recursiveProcessBoneMatrix(matrix, modelNode->mChilds[i]->mName);
	}
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
