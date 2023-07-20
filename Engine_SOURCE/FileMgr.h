#pragma once
#include "Engine.h"



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
		math::Vector3 Translation;
		math::Vector3 Rotaion;
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
	//std::string ParsingString(int startPos,std::string buf, std::string delValue){};


private:
	const std::string parsingString(std::string& buf, const std::string& delValue, std::string::size_type& startPos) const;
	const std::string::size_type startStringPos(std::string& buf, const std::string& delValue) const;

	const ParsingNodeData readNodes(std::string& buf) const ;
	const ParsingSkeletonData readSkeleton(std::string& buf) const;
	const ParsingTriangleData readTriangles(std::string& buf) const;
private:
	ParsingVector<ParsingNodeData>					mNodeData;
	ParsingVector<ParsingSkeletonData>				mSkeletonData;
	ParsingVector<ParsingTriangleData>				mTriangleData;
}; 
