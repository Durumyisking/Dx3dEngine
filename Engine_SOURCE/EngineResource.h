#pragma once
#include "Entity.h"


using namespace enums;

class Resource : public DruEntity
{
public:
	Resource(eResourceType	type);
	virtual ~Resource();

	virtual HRESULT Load(const std::wstring& path) = 0;
	virtual HRESULT LoadFullpath(const std::wstring& path) = 0;

	const std::wstring& GetKey() const { return mKey; }
	const std::wstring& GetPath() const { return mPath; }

	void SetKey(const std::wstring& key) { mKey = key; }
	void SetPath(const std::wstring& path) { mPath = path; }
	eResourceType GetType() { return mType; }


private:
	std::wstring	mPath;
	std::wstring	mKey;
	eResourceType	mType;
		
};
