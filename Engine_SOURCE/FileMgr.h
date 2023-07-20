#pragma once
#include "Engine.h"
#include "Renderer.h"

#include "..//External/assimp/include/assimp/Importer.hpp"
#include "..//External/assimp/include/assimp/cimport.h"
#include "..//External/assimp/include/assimp/postprocess.h"
#include "..//External/assimp/include/assimp/scene.h"

#pragma comment(lib, "..//External/assimp/lib/Debug/assimp-vc143-mtd.lib")
#define ASSIMP_LOAD_FLAGES (aiProcess_Triangulate | aiProcess_FlipUVs | aiProcess_JoinIdenticalVertices | aiProcess_CalcTangentSpace |  aiProcess_MakeLeftHanded | aiProcess_FlipWindingOrder)


namespace dru
{
	class FileMgr
	{
		SINGLE(FileMgr)
	public:
		struct ParsingNodeData
		{
			std::string Name;
			int ParentsIDX;
		};

		struct ParsingSkeletonData
		{
			float Time;
			dru::math::Vector3 Translation;
			dru::math::Vector3 Rotaion;
		};

		struct ParsingTriangleData
		{
			int IDX;
			std::string TexName;
			std::vector<float> Value;
		};

		template<typename T>
		using ParsingVector = std::vector<T>;
		template<typename key, typename value>
		using ParsingMap = std::map<key, value>;

	public:
		void FileLoad(const std::wstring& path);
		void TestLoad(const std::wstring& path);
		//std::string ParsingString(int startPos,std::string buf, std::string delValue){};


	private:
		const std::string parsingString(std::string& buf, const std::string& delValue, std::string::size_type& startPos) const;
		const std::string::size_type startStringPos(std::string& buf, const std::string& delValue) const;

		const ParsingNodeData readNodes(std::string& buf) const ;
		const ParsingSkeletonData readSkeleton(std::string& buf) const;
		const ParsingTriangleData readTriangles(std::string& buf) const;
	private:
		void loadModel(const aiScene* scene);
		void processNode(const aiNode* node, const aiScene* scene, std::vector<renderer::Vertex>& meshes);
		void processMesh(const aiMesh* mesh, const aiScene* scene);
		void processMaterial();

	private:
		ParsingVector<ParsingNodeData>					mNodeData;
		ParsingVector<ParsingSkeletonData>				mSkeletonData;
		ParsingVector<ParsingTriangleData>				mTriangleData;

		Assimp::Importer mAssimpImporter;
		
	}; 
}