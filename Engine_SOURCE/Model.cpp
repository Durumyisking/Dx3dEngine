#include "Model.h"
#include "Mesh.h"
#include "Renderer.h"
#include "Texture.h"
#include "def.h"
#include "StructedBuffer.h"
#include "Material.h"

#include "TimeMgr.h"
#include "ResourceMgr.h"

namespace fs = std::filesystem;

Model::Model()
	: Resource(eResourceType::Model)
	, mStructure(nullptr)
	, mAssimpImporter{}
	, mNodes{}
	, mBoneMap{}
	, mBones{}
	, mMeshes{}
	, mMaterials{}
	, mTextures{}
	, mRootNodeName(L"")
	, mCurDirectoryPath(L"")
	, mParentModel(nullptr)
	, mParentTargetBone(L"")
	, mTargetBone(L"")
	, mOffsetRotation(math::Vector3(0.0f, 0.0f, 0.0f))
	, mbUseInstance(false)
	//, mVariableMaterials(22)
{

}


Model::~Model()
{
	release();
}

HRESULT Model::Load(const std::wstring& path)
{
	std::string sPath = ConvertToString(path.c_str());
	const aiScene* aiscene = mAssimpImporter.ReadFile(sPath, ASSIMP_LOAD_FLAGES);

	if (aiscene == nullptr || aiscene->mRootNode == nullptr)
	{
		// 파일 로드 실패
		return E_FAIL;
	}

	aiscene->mRootNode->mTransformation = aiMatrix4x4();

	std::wstring sceneName = ConvertToW_String(aiscene->mName.C_Str());
	mRootNodeName = ConvertToW_String(aiscene->mRootNode->mName.C_Str());

	recursiveProcessNode(aiscene->mRootNode, aiscene, nullptr);

	if (mStructure == nullptr)
	{
		mStructure = new StructedBuffer();
		mStructure->Create(static_cast<UINT>(sizeof(BoneMat)), static_cast<UINT>(mBones.size()), eSRVType::SRV, nullptr, true);
	}

	mVariableMaterials.resize(22);
	mAssimpImporter.FreeScene();

	return S_OK;
}

HRESULT Model::LoadFullpath(const std::wstring& path)
{
	std::string sPath = ConvertToString(path.c_str());
	const aiScene* aiscene = mAssimpImporter.ReadFile(sPath, ASSIMP_LOAD_FLAGES);

	if (aiscene == nullptr || aiscene->mRootNode == nullptr)
	{
		// 파일 로드 실패
		return E_FAIL;
	}

	aiscene->mRootNode->mTransformation = aiMatrix4x4();

	std::wstring sceneName = ConvertToW_String(aiscene->mName.C_Str());
	mRootNodeName = ConvertToW_String(aiscene->mRootNode->mName.C_Str());

	recursiveProcessNode(aiscene->mRootNode, aiscene, nullptr);

	if (mStructure == nullptr)
	{
		mStructure = new StructedBuffer();
		mStructure->Create(static_cast<UINT>(sizeof(BoneMat)), static_cast<UINT>(mBones.size()), eSRVType::SRV, nullptr, true);
	}

	mVariableMaterials.resize(mMaterials.size());
	mAssimpImporter.FreeScene();

	return S_OK;
}

ModelNode* Model::FindNode(const std::wstring& nodeName)
{

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
	return mBoneMap.find(nodeName) == mBoneMap.end() ? nullptr : mBoneMap.find(nodeName)->second;
}

void Model::BindBoneMatrix()
{
	const ModelNode* node = FindNode(mRootNodeName);
	if (node == nullptr)
		return;

	aiMatrix4x4 rootmat = node->mTransformation;

	for (size_t i = 0; i < node->mChilds.size(); ++i)
	{
		recursiveProcessBoneMatrix(rootmat, node->mChilds[i]->mName);
	}
}

void Model::Bind_Render(bool bindMaterial)
{
	if (mStructure == nullptr)
		return;

	BoneMat boneInfo = {};
	std::vector<BoneMat> boneMat = {};

	BindBoneMatrix();

	boneMat.reserve(mBones.size());

	for (Bone* bone : mBones)
	{
		aiMatrix4x4 finalMat = bone->mFinalMatrix;
		boneInfo.FinalTransformation = ConvertMatrix(finalMat);
		boneMat.emplace_back(boneInfo);
	}

	mStructure->SetData(boneMat.data(), static_cast<UINT>(boneMat.size()));
	mStructure->BindSRV(eShaderStage::VS, 30);

	for (size_t i = 0; i < mMeshes.size(); ++i)
	{
		if (mMeshes[i] == nullptr)
			continue;

		//if (mMaterials[i] == nullptr)
		//	continue;

		if (mMeshes[i]->IsRender() == false)
			continue;

		if (bindMaterial)
		{
			//std::vector<Texture*> Textures = GetTexture(static_cast<int>(i));
			//for (int slot = 0; slot < Textures.size(); ++slot)
			//{
			//	if (Textures[slot] == nullptr)
			//		continue;

			//	mMaterials[i]->SetTexture(static_cast<eTextureSlot>(slot), Textures[slot]);
			//}

			if (!(mVariableMaterials.empty() && mMaterials.empty()))
				mVariableMaterials[i] == nullptr ? mMaterials[i]->Bind() : mVariableMaterials[i]->Bind();
		}

		mMeshes[i]->BindBuffer();
		mMeshes[i]->Render();

		if(!(mVariableMaterials.empty() && mMaterials.empty()))
			mVariableMaterials[i] == nullptr ? mMaterials[i]->Clear() : mVariableMaterials[i]->Clear();
	}

	mFrameAnimationVector = nullptr;
	mStructure->Clear();
	boneMat.clear();
}

void Model::Bind_RenderInstance(UINT instanceCount, bool bindMaterial)
{
	if (mStructure == nullptr)
		return;

	BoneMat boneInfo = {};
	std::vector<BoneMat> boneMat = {};

	BindBoneMatrix();

	boneMat.reserve(mBones.size());

	for (Bone* bone : mBones)
	{
		aiMatrix4x4 finalMat = bone->mFinalMatrix;
		boneInfo.FinalTransformation = ConvertMatrix(finalMat);
		boneMat.emplace_back(boneInfo);
	}

	mStructure->SetData(boneMat.data(), static_cast<UINT>(boneMat.size()));
	mStructure->BindSRV(eShaderStage::VS, 30);

	for (size_t i = 0; i < mMeshes.size(); ++i)
	{
		if (mMeshes[i] == nullptr)
			continue;

		//if (mMaterials[i] == nullptr)
		//	continue;

		if (mMeshes[i]->IsRender() == false)
			continue;

		if (bindMaterial)
		{
			if (!(mVariableMaterials.empty() && mMaterials.empty()))
				mVariableMaterials[i] == nullptr ? mMaterials[i]->Bind() : mVariableMaterials[i]->Bind();
		}

		mMeshes[i]->BindBuffer(true);
		mMeshes[i]->RenderInstanced(instanceCount);

		if (!(mVariableMaterials.empty() && mMaterials.empty()))
			mVariableMaterials[i] == nullptr ? mMaterials[i]->Clear() : mVariableMaterials[i]->Clear();
	}

	mFrameAnimationVector = nullptr;
	mStructure->Clear();
	boneMat.clear();
}

void Model::SetFrameAnimationVector(const std::map<std::wstring, aiMatrix4x4>* animationVector)
{
	mFrameAnimationVector = animationVector;
}


void Model::MeshRenderSwtich(const std::wstring& name, bool renderSwitch)
{
	for (auto iter = mMeshes.begin(); iter != mMeshes.end(); ++iter)
	{
		if ((*iter)->GetName() == name)
		{ 
			(*iter)->SetRender(renderSwitch);
			break;
		}
	}
}

void Model::AllMeshRenderSwtichOff()
{
	for (auto iter = mMeshes.begin(); iter != mMeshes.end(); ++iter)
	{
		(*iter)->SetRender(false);
	}
}

void Model::recursiveProcessNode(aiNode* node, const aiScene* scene, ModelNode* rootNode)
{
	std::wstring wNodeName = ConvertToW_String(node->mName.C_Str());
	std::map<std::wstring, ModelNode*>::iterator iter = mNodes.find(wNodeName);
	ModelNode* curNode = nullptr;
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
	else
	{
		curNode = iter->second;
		if (curNode->mRootNode != nullptr)
		{
			rootNode = curNode->mRootNode;
		}
	}

	if (rootNode)
		rootNode->mChilds.emplace_back(curNode);

	for (UINT i = 0; i < node->mNumMeshes; ++i)
	{
		aiMesh* aiMesh = scene->mMeshes[node->mMeshes[i]];
		recursiveProcessMesh(aiMesh, scene, wNodeName);
	}

	for (UINT i = 0; i < node->mNumChildren; ++i)
	{
		recursiveProcessNode(node->mChildren[i], scene, curNode);
	}
}

void Model::recursiveProcessMesh(aiMesh* mesh, const aiScene* scene, const std::wstring& nodeName)
{
	std::map<std::wstring, ModelNode*>::iterator iter = mNodes.find(nodeName);

	std::vector<Vertex> vertexes;
	std::vector<UINT> indexes;
	std::vector<Texture> textures;


	vertexes.reserve(mesh->mNumVertices);

	for (UINT i = 0; i < mesh->mNumVertices; ++i)
	{
		Vertex vertex = {};
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

	//indexes.reserve(mesh->mNumFaces);
	//for (UINT i = 0; i < mesh->mNumFaces; ++i)
	//{
	//	aiFace face = mesh->mFaces[i];
	//	if (mesh->mFaces[i].mNumIndices != 3) {
	//		int a = 0;
	//	}
	//	for (UINT j = 0; j < face.mNumIndices; ++j)
	//	{
	//		indexes.emplace_back(face.mIndices[j]);
	//	}
	//}

	indexes.reserve(mesh->mNumFaces);
	for (UINT i = 0; i < mesh->mNumFaces; ++i)
	{
		aiFace face = mesh->mFaces[i];
		if (mesh->mFaces[i].mNumIndices != 3) {
			int a = 0;
		}
		for (UINT j = 0; j < face.mNumIndices; ++j)
		{
			indexes.emplace_back(face.mIndices[j]);
		}
	}

	for (unsigned int i = 0; i < mesh->mNumBones; ++i)
	{
		Bone* bone = nullptr;
		aiBone* aiBone = mesh->mBones[i];

		UINT bonIndex = 0;

		if (mBoneMap.end() == mBoneMap.find(ConvertToW_String(aiBone->mName.C_Str())))
		{
			bone = new Bone();
			bone->mName = ConvertToW_String(aiBone->mName.C_Str());
			bone->mOffsetMatrix = aiBone->mOffsetMatrix;
			bone->mIndex = static_cast<UINT>(mBones.size());
			mBones.emplace_back(bone);
			mBoneMap.insert(std::pair<std::wstring, Bone*>(bone->mName, bone));

			bonIndex = bone->mIndex;
		}
		else
		{
			bone = mBoneMap.find(ConvertToW_String(aiBone->mName.C_Str()))->second;
			bone->mOffsetMatrix = aiBone->mOffsetMatrix;
			bonIndex = bone->mIndex;

			mBones[bonIndex]->mOffsetMatrix = aiBone->mOffsetMatrix;
		}


		for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; ++j)
		{
			UINT vertexID = mesh->mBones[i]->mWeights[j].mVertexId;
			float weight = mesh->mBones[i]->mWeights[j].mWeight;

			if (vertexes[vertexID].BlendWeight.x == 0.0f)
			{
				vertexes[vertexID].BlendID.x = static_cast<float>(bonIndex);
				vertexes[vertexID].BlendWeight.x = weight;
			}
			else if (vertexes[vertexID].BlendWeight.y == 0.0f)
			{
				vertexes[vertexID].BlendID.y = static_cast<float>(bonIndex);
				vertexes[vertexID].BlendWeight.y = weight;
			}
			else if (vertexes[vertexID].BlendWeight.z == 0.0f)
			{
				vertexes[vertexID].BlendID.z = static_cast<float>(bonIndex);
				vertexes[vertexID].BlendWeight.z = weight;
			}
			else if (vertexes[vertexID].BlendWeight.w == 0.0f)
			{
				vertexes[vertexID].BlendID.w = static_cast<float>(bonIndex);
				vertexes[vertexID].BlendWeight.w = weight;
			}
		}
	}

	//if (mesh->mMaterialIndex >= 0)
	//{
	//	// textureLoad
	//	aiMaterial* aiMater = scene->mMaterials[mesh->mMaterialIndex];
	//	std::vector<Model::TextureInfo> textureBuff = {};
	//	for (int type = static_cast<int>(aiTextureType_NONE); type < static_cast<int>(aiTextureType_UNKNOWN); ++type)
	//	{
	//		std::vector<Model::TextureInfo> texInfo = processMaterial(aiMater, static_cast<aiTextureType>(type));
	//		textureBuff.insert(textureBuff.end(), texInfo.begin(), texInfo.end());
	//	}

	//	if (textureBuff.size() > 0)
	//	{
	//		mTextures.emplace_back(textureBuff);

	//		std::vector<TextureInfo>& texInfo = mTextures[mTextures.size() - 1];
	//		for (auto& tex : texInfo)
	//		{
	//			if (tex.texPath == L"")
	//				continue;

	//			tex.pTex = new Texture();
	//			tex.pTex->Load(tex.texPath, tex);
	//		}
	//	}

	//	//Material
	//	Material* inMaterial = new Material();
	//	inMaterial->SetRenderingMode(eRenderingMode::DeferredMask);
	//	Shader* shader = GETSINGLE(ResourceMgr)->Find<Shader>(L"DeferredShader");
	//	inMaterial->SetShader(shader);

	//	mMaterials.emplace_back(inMaterial);
	//}

	Mesh* inMesh = new Mesh();
	inMesh->CreateVertexBuffer(vertexes.data(), static_cast<UINT>(vertexes.size()));
	if (mbUseInstance)
	{
		std::vector<InstancingData> mats = {};
		mats.resize(1000);
		for (int i = 0; i < mats.size(); i++)
		{
			mats[i].world = Matrix::Identity;		
			mats[i].worldIT = Matrix::Identity;
		}
		inMesh->CreateInstanceBuffer(mats.data(), static_cast<UINT>(mats.size()));
	}

	inMesh->CreateIndexBuffer(indexes.data(), static_cast<UINT>(indexes.size()));
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

			texInfo.texID = static_cast<UINT>(tex->GetID());
			texInfo.pTex = tex;

			GETSINGLE(ResourceMgr)->Insert<Texture>(texInfo.texName, texInfo.pTex);
		}
	}
}

void Model::CreateMaterial()
{
	Material* inMaterial = new Material();
	std::wstring matName = {};
	for (auto& textureVec : mTextures)
	{
		for (TextureInfo texInfo : textureVec)
		{
			switch (texInfo.type)
			{
			case aiTextureType_DIFFUSE:
			{
				inMaterial->SetTexture(eTextureSlot::Albedo, texInfo.pTex);
				matName = texInfo.texName;
				std::size_t found = matName.find(L"_");
				if (found != std::wstring::npos) {
					matName = matName.substr(0, found); // _ 이전까지의 문자열 추출
				}
			}
			break;
			case aiTextureType_EMISSIVE:
				inMaterial->SetTexture(eTextureSlot::Emissive, texInfo.pTex);
				break;
			case aiTextureType_NORMALS:
				inMaterial->SetTexture(eTextureSlot::Normal, texInfo.pTex);
				break;
			case aiTextureType_METALNESS:
				inMaterial->SetTexture(eTextureSlot::Metallic, texInfo.pTex);
				break;
			case aiTextureType_DIFFUSE_ROUGHNESS:
				inMaterial->SetTexture(eTextureSlot::Roughness, texInfo.pTex);
				break;
			default:
				break;
			}
		}
		inMaterial->SetShaderByKey(L"DeferredShader");
		GETSINGLE(ResourceMgr)->Insert<Material>(matName, inMaterial);
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
		if (tex.pTex != nullptr)
			outTexVector.emplace_back(tex.pTex);
	}

	return outTexVector;
}

void Model::recursiveProcessBoneMatrix(aiMatrix4x4 matrix, const std::wstring& nodeName)
{
	const ModelNode* modelNode = FindNode(nodeName);
	aiMatrix4x4 transform = modelNode->mTransformation;

	if (mFrameAnimationVector)
	{
		auto iter = mFrameAnimationVector->find(nodeName);
		if (iter != mFrameAnimationVector->end())
		{
			transform = iter->second;
		}
	}

	matrix = matrix * transform;

	if (nodeName == mTargetBone && mParentModel)
	{
		Bone* parentBone = mParentModel->FindBone(mParentTargetBone);
		aiMatrix4x4 testMat;
		transform = transform * testMat.FromEulerAnglesXYZ(mOffsetRotation.x, mOffsetRotation.y, mOffsetRotation.z);
		matrix = parentBone->mLocalMatrix * transform;
	}

	if (mBoneMap.find(nodeName) != mBoneMap.end())
	{
		Bone* bone = mBoneMap.find(nodeName)->second;

		bone->mFinalMatrix = matrix * bone->mOffsetMatrix;
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

math::Matrix Model::ConvertMatrix(aiMatrix4x4 aimat)
{
	math::Matrix outMat;
	outMat._11 = aimat.a1, outMat._12 = aimat.a2, outMat._13 = aimat.a3, outMat._14 = aimat.a4;
	outMat._21 = aimat.b1, outMat._22 = aimat.b2, outMat._23 = aimat.b3, outMat._24 = aimat.b4;
	outMat._31 = aimat.c1, outMat._32 = aimat.c2, outMat._33 = aimat.c3, outMat._34 = aimat.c4;
	outMat._41 = aimat.d1, outMat._42 = aimat.d2, outMat._43 = aimat.d3, outMat._44 = aimat.d4;

	return outMat;
}

Material* Model::GetVariableMaterials(UINT index)
{
	return index >= mVariableMaterials.size() ? nullptr : mVariableMaterials[index];
}

void Model::SetVariableMaterials(UINT index, Material* mater)
{
	//if (index >= mVariableMaterials.size())
	//	return;

	mVariableMaterials[index] = mater;
}

void Model::SetVariableMaterialsByKey(UINT index, const std::wstring& key)
{
	//if (index >= mVariableMaterials.size())
	//	return;

	Material* mater = GETSINGLE(ResourceMgr)->Find<Material>(key);

	if (mater)
	{
		mVariableMaterials[index] = mater;
	}
}