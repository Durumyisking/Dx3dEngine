#include "FileMgr.h"
#include "Application.h"
#include "Texture.h"

extern dru::Application application;
namespace fs = std::filesystem;
namespace dru
{

	FileMgr::FileMgr()
		: mAssimpImporter{}
	{

	}

	FileMgr::~FileMgr()
	{

	}

	void FileMgr::FileLoad(const std::wstring& path)
	{
		// 파일 경로 설정
		std::ifstream file(L"..//" + path, std::ios::in);

		std::string buf = "";
		// 파일이 열리지 않으면 Window 종료
		if (!file.is_open())
		{
			std::cout << "파일이 없습니다" << std::endl;
			DestroyWindow(application.GetHwnd());
		}

		// Nodes
		while (1)
		{
			getline(file, buf);
			if ((buf.find("nodes") != std::string::npos)
				|| (buf.find("version") != std::string::npos))
			{
				continue;;
			}

			if (buf.find("end") != std::string::npos)
			{
				break;
			}

			ParsingNodeData data = {};
			data = readNodes(buf);
			mNodeData.emplace_back(data);
		}

		// Skeleton
		while (1)
		{
			getline(file, buf);
			if (buf.find("skeleton") != std::string::npos)
			{
				continue;
			}
			else if (buf.find("end") != std::string::npos)
			{
				break;
			}
			else if (buf.find("time") != std::string::npos)
			{
				continue;
			}

			ParsingSkeletonData data = {};
			data = readSkeleton(buf);
			mSkeletonData.emplace_back(data);
		}

		// Triangles
		std::string texName = "";
		while (1)
		{
			getline(file, buf);
			if (buf.find("triangles") != std::string::npos)
			{
				continue;
			}
			else if (buf.find("end") != std::string::npos)
			{
				break;
			}
			else if (buf.find("png") != std::string::npos)
			{
				texName = buf;
				continue;
			}

			ParsingTriangleData data = {};
			data = readTriangles(buf);
			data.TexName = texName;

			mTriangleData.emplace_back(data);
		}

		file.close();
	}

	void FileMgr::TestLoad(const std::wstring& path)
	{
		std::string sPath = "..//" + std::string(path.begin(), path.end());
		const aiScene* aiscene = mAssimpImporter.ReadFile(sPath, ASSIMP_LOAD_FLAGES);

		if (aiscene == nullptr || aiscene->mRootNode == nullptr)
		{
			// 파일 로드 실패
			return;
		}

		std::vector<renderer::Vertex> meshes;
		processNode(aiscene->mRootNode, aiscene, meshes);
		int a = 0;

	}

	const std::string FileMgr::parsingString(std::string& buf, const std::string& delValue, std::string::size_type& startPos) const
	{
		std::string::size_type fPos = buf.find_first_of(delValue, startPos);
		std::string::size_type lPos = buf.find_first_of(delValue, fPos + 1);
		if (fPos >= buf.size())
		{
			return std::string("fail");
		}

		startPos = fPos + 1;

		std::string str = buf.substr((fPos + 1), lPos - (fPos + 1));

		return str;
	}

	const std::string::size_type FileMgr::startStringPos(std::string& buf, const std::string& delValue) const
	{
		std::string::size_type pos = 0;
		pos = buf.find_first_not_of(delValue, 0);

		return pos;
	}

	const FileMgr::ParsingNodeData FileMgr::readNodes(std::string& buf) const
	{
		ParsingNodeData Data = {};
		std::string str = "";
		std::string::size_type fPos = buf.find_first_of('"', 1);
		std::string::size_type lPos = buf.find_last_of('"', buf.size() - 1);
		fPos += 1;

		str = buf.substr(fPos, lPos - fPos);
		Data.Name = str;


		fPos = buf.find_last_of(' ', buf.size() - 1);
		str = buf.substr(fPos, buf.size() - 1);
		Data.ParentsIDX = std::atoi(str.c_str());

		return Data;
	}

	const FileMgr::ParsingSkeletonData FileMgr::readSkeleton(std::string& buf) const
	{
		ParsingSkeletonData data = {};
		std::string str = "";
		std::string::size_type pos = 0;
		pos = startStringPos(buf, " ");

		std::vector<float> temp = {};
		for (int i = 0; i < 3; ++i)
		{
			str = parsingString(buf, " ", pos);
			temp.emplace_back((std::stof(str.c_str())));
		}

		data.Translation = dru::math::Vector3(temp[0], temp[1], temp[2]);
		temp.clear();

		for (int i = 0; i < 3; ++i)
		{
			str = parsingString(buf, " ", pos);
			temp.emplace_back(std::stof(str.c_str()));
		}

		data.Rotaion = dru::math::Vector3(temp[0], temp[1], temp[2]);

		return data;
	}

	const FileMgr::ParsingTriangleData FileMgr::readTriangles(std::string& buf) const
	{
		ParsingTriangleData data = {};
		std::string::size_type pos = startStringPos(buf, " ");


		std::string str = buf.substr(pos, pos + 1);
		data.IDX = std::atoi(str.c_str());
		std::vector<float> temp = {};
		while (1)
		{
			str = parsingString(buf, " ", pos);
			if (str.find("fail") != std::string::npos)
			{
				break;
			}
			temp.emplace_back(std::stof(str.c_str()));
		}
		data.Value = temp;

		return data;
	}
	void FileMgr::loadModel(const aiScene* scene)
	{

	}
	void FileMgr::processNode(const aiNode* node, const aiScene* scene, std::vector<renderer::Vertex>& meshes)
	{
		// 현제 노드 메쉬 정보 저장
		for (UINT i = 0; i < node->mNumMeshes; ++i)
		{
			aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
			processMesh(mesh, scene);
			//meshes.emplace_back(processMesh(mesh, scene));
		}

		// 재귀적으로 메쉬 호출
		for (UINT i = 0; i < node->mNumChildren; ++i)
		{
			processNode(node->mChildren[i], scene, meshes);
		}
	}

	void FileMgr::processMesh(const aiMesh* mesh, const aiScene* scene)
	{
		std::vector<renderer::Vertex> vertexes;
		std::vector<UINT> indexes;
		std::vector<Texture> textures;

		// 정점 정보 로드
		for (UINT i = 0; i < mesh->mNumVertices; ++i)
		{
			renderer::Vertex vertex= {};
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
			tangent .x = mesh->mTangents[i].x;
			tangent .y = mesh->mTangents[i].y;
			tangent .z = mesh->mTangents[i].z;
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

		Mesh* myMesh = new Mesh();
		myMesh->CreateVertexBuffer(vertexes.data(), vertexes.size());
		myMesh->CreateIndexBuffer(indexes.data(), indexes.size());

		static int a = 0;
		std::string sName(mesh->mName.C_Str());
		//std::wstring name = std::wstring(sName.begin(), sName.end());
		std::wstring name = L"test" + std::to_wstring(a);
		a++;

		GETSINGLE(ResourceMgr)->Insert<Mesh>(name, myMesh);

		std::cout << a << std::endl;
	}

	void FileMgr::processMaterial()
	{
	}
}