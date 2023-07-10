#include "ResourceMgr.h"

namespace dru
{
	ResourceMgr::ResourceMgr()
		: mResources{}
	{

	}

	ResourceMgr::~ResourceMgr()
	{

	}

	void ResourceMgr::Release()
	{
		for (auto& iter : mResources)
		{
			if (iter.second == nullptr)
				continue;

			delete iter.second;
			iter.second = nullptr;

		}
	}
}