#pragma once
#include "Engine.h"

#include "..//External/assimp/include/assimp/Importer.hpp"
#include "..//External/assimp/include/assimp/cimport.h"
#include "..//External/assimp/include/assimp/postprocess.h"
#include "..//External/assimp/include/assimp/scene.h"

#ifdef _DEBUG
#pragma comment(lib, "..//External/assimp/lib/Debug/assimp-vc143-mtd.lib")
#else
#pragma comment(lib, "..//External/assimp/lib/Release/assimp-vc143-mt.lib")
#endif

class ModelNode
{
public:
	ModelNode();
	~ModelNode();

	std::wstring mName = L"";
	aiMatrix4x4 mTransformation = {};

	std::wstring mRootName = L"";
	ModelNode* mRootNode = nullptr;

	std::vector<ModelNode*> mChilds;

	GETSET(aiMatrix4x4, mTransformation, Transformation)
};
