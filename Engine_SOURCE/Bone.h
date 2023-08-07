#pragma once
#include "Engine.h"

#include "..//External/assimp/include/assimp/Importer.hpp"
#include "..//External/assimp/include/assimp/cimport.h"
#include "..//External/assimp/include/assimp/postprocess.h"
#include "..//External/assimp/include/assimp/scene.h"

#pragma comment(lib, "..//External/assimp/lib/Debug/assimp-vc143-mtd.lib")

class Bone
{
public:
	Bone();
	~Bone();

	UINT mIndex;
	std::wstring mName = L"";

	// 바인드 포즈의 역행렬
	aiMatrix4x4 mOffsetMatrix = {};

	// GlobalInverse * NodeTransform * mOffsetMatrix
	aiMatrix4x4 mFinalMatrix = {};

	// 노드의 계층 정보
	aiMatrix4x4 mLocalMatrix = {};
};
