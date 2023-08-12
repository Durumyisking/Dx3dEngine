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
		math::Vector3 Translation;
		math::Vector3 Rotaion;
	};
}