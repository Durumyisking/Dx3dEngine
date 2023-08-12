#pragma once
#include "druMath.h"
#include <string>

namespace animation
{
	struct NodeData
	{
		std::string Name;
		int ParentsIDX;
	};

	struct SkeletonData
	{
		float Time;
		std::vector<math::Vector3> Translation;
		std::vector<math::Vector3> Rotaion;
	};
}