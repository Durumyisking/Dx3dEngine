#pragma once
#include "druMath.h"
#include <string>

namespace animation
{
	struct NodeData
	{
		std::wstring Name;
		int ParentsIDX;
	};

	struct SkeletonData
	{
		float Time;
		std::vector<std::pair<int,math::Vector3>> Translation;
		std::vector< std::pair<int,math::Vector3>> Rotation;
	};
}