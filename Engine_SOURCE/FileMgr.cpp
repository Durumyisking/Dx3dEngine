#include "FileMgr.h"
#include "Application.h"

extern dru::Application application;
namespace fs = std::filesystem;
namespace dru
{

	FileMgr::FileMgr()
	{
	}

	FileMgr::~FileMgr()
	{

	}

	void FileMgr::FileLoad(const std::wstring& path)
	{
		std::ifstream file(path, std::ios::in);
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
		float time = 0.f;
		while (1)
		{
			getline(file, buf);
			if (buf.find("skeleton") != std::string::npos)
			{
				continue;;
			}
			else if (buf.find("end") != std::string::npos)
			{
				break;
			}
			else if (buf.find("time") != std::string::npos)
			{
				
			}

			ParsingSkeletonData data = {};
			data = readSkeleton(buf);
			mSkeletonData.emplace_back(data);
		}

		// Triangles

		file.close();
	}

	const FileMgr::ParsingNodeData& FileMgr::readNodes(std::string& buf) const
	{
		ParsingNodeData Data = {};
		std::string str = "";
		std::string::size_type fPos = buf.find_first_not_of('"', 0);
		std::string::size_type lPos = buf.find_first_of('"', fPos);

		str = buf.substr(fPos + 1, lPos);
		Data.Name = str;

		fPos = buf.find_first_not_of(' ', fPos);
		lPos = buf.find_first_not_of('\n', fPos);
		str = buf.substr(fPos + 1, lPos);
		Data.ParentsIDX = std::atoi(str.c_str());

		return Data;
	}

	const FileMgr::ParsingSkeletonData& FileMgr::readSkeleton(std::string& buf) const
	{
		ParsingSkeletonData data = {};
		return data;
	}

	const FileMgr::ParsingTriangleData& FileMgr::readTriangles(std::string& buf) const
	{
		ParsingTriangleData data = {};
		return data;
	}
}