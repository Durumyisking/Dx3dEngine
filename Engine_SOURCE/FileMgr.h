#pragma once
#include "Engine.h"

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
			dru::math::Vector3 Tranlation;
			dru::math::Vector3 Rotaion;
		};

		struct ParsingTriangleData
		{
			int IDX;
			std::string TexName;

			dru::math::Vector3 Postion;
			dru::math::Vector3 Normal;
			dru::math::Vector2 UV;
		};

		template<typename T>
		using ParsingVector = std::vector<T>;

	public:
		void FileLoad(const std::wstring& path);

	private:
		const ParsingNodeData& readNodes(std::string& buf) const ;
		const ParsingSkeletonData& readSkeleton(std::string& buf) const;
		const ParsingTriangleData& readTriangles(std::string& buf) const;
	private:
		ParsingVector<ParsingNodeData>		mNodeData;
		ParsingVector<ParsingSkeletonData>	mSkeletonData;
		ParsingVector<ParsingTriangleData>	mTriangleData;
	}; 
}