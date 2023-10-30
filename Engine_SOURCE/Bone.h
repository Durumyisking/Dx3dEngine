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

class Bone
{
public:
	Bone();
	~Bone();

	UINT mIndex;
	std::wstring mName = L"";

	// ���ε� ������ �����
	aiMatrix4x4 mOffsetMatrix = {};

	// GlobalInverse * NodeTransform * mOffsetMatrix
	aiMatrix4x4 mFinalMatrix = {};

	// ����� ���� ����
	aiMatrix4x4 mLocalMatrix = {};
};
