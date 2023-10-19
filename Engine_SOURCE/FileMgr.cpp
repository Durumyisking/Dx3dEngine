#include "FileMgr.h"
#include "Application.h"
#include "Model.h"

extern Application application;
namespace fs = std::filesystem;



FileMgr::FileMgr()
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

	//// Triangles
	//std::string texName = "";
	//while (1)
	//{
	//	getline(file, buf);
	//	if (buf.find("triangles") != std::string::npos)
	//	{
	//		continue;
	//	}
	//	else if (buf.find("end") != std::string::npos)
	//	{
	//		break;
	//	}
	//	else if (buf.find("png") != std::string::npos)
	//	{
	//		texName = buf;
	//		continue;
	//	}

	//	ParsingTriangleData data = {};
	//	data = readTriangles(buf);
	//	data.TexName = texName;

	//	mTriangleData.emplace_back(data);
	//}

	file.close();
}

Model* FileMgr::ModelLoad(const std::wstring& path, const std::wstring& modelName, bool useInstance)
{
	std::wstring fullPath = L"..//" + path;
	fs::path folderPath = std::string(fullPath.begin(), fullPath.end());

	Model* parentModel = nullptr;
	// 폴더 내부 파일들을 순회하며 출력
	for (const auto& entry : fs::directory_iterator(folderPath)) {
		if (entry.is_regular_file())
		{
			wchar_t szExtension[256] = {};
			_wsplitpath_s(entry.path().c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExtension, 256); // 경로에서 확장자만 뽑아오는 녀석

			std::wstring extension(szExtension);

			Model* model = GETSINGLE(ResourceMgr)->Find<Model>(modelName);

			if (model == nullptr)
			{
				model = new Model();
				GETSINGLE(ResourceMgr)->Insert<Model>(modelName, model);
			}
			else
			{
				return nullptr;
			}

			if (L".DAE" == extension || L".Dae" == extension || L".dae" == extension)
			{
				model->SetCurDirectoryPath(fullPath);
				model->SetUseInstance(useInstance);
				model->Load(entry.path());
			}

			return model;
		}
	}


	return nullptr;
}

bool FileMgr::StageFolderLoad(const std::wstring& path, const std::wstring& stageName)
{
	std::wstring fullPath = L"..//" + path;
	fs::path folderPath = std::string(fullPath.begin(), fullPath.end());

	Model* parentModel = nullptr;
	// 폴더 내부 파일들을 순회하며 출력
	for (const auto& entry : fs::directory_iterator(folderPath)) 
	{
		if (entry.is_regular_file())
		{
			wchar_t szExtension[256] = {};
			_wsplitpath_s(entry.path().c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExtension, 256); // 경로에서 확장자만 뽑아오는 녀석

			std::wstring extension(szExtension);

			if (L".DAE" == extension || L".Dae" == extension || L".dae" == extension)
			{
				std::wstring fileName(entry.path().stem().wstring());

				std::wstring modelName = stageName + L"_" + fileName;

				Model* model = GETSINGLE(ResourceMgr)->Find<Model>(modelName);

				if (model == nullptr)
				{
					model = new Model();
					GETSINGLE(ResourceMgr)->Insert<Model>(modelName, model);
				}
				model->SetCurDirectoryPath(fullPath);
				model->Load(entry.path());
			}
			else if (L".OBJ" == extension || L".Obj" == extension || L".obj" == extension)
			{

			}
		}
	}

	return true;
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

	data.Translation = math::Vector3(temp[0], temp[1], temp[2]);
	temp.clear();

	for (int i = 0; i < 3; ++i)
	{
		str = parsingString(buf, " ", pos);
		temp.emplace_back(std::stof(str.c_str()));
	}

	data.Rotaion = math::Vector3(temp[0], temp[1], temp[2]);

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
