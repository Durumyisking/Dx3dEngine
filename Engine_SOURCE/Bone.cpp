#include "Bone.h"

Bone::Bone()
	: mIndex(0)
	, mName(L"")
	, mOffsetMatrix{}
	, mFinalMatrix{}
	, mLocalMatrix{}
{
}

Bone::~Bone()
{
}
