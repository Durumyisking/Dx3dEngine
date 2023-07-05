#pragma once
#include "Engine.h"

namespace dru
{
	using namespace enums;
	class Resource
	{
	public:
		Resource(eResourceType	_Type);
		virtual ~Resource();

		virtual HRESULT Load(const std::wstring& path) = 0;

		const std::wstring& GetKey() { return mKey; }
		const std::wstring& GetPath() { return mPath; }

		void SetKey(const std::wstring& key) { mKey = key; }
		void SetPath(const std::wstring& path) { mPath = path; }

	private:
		std::wstring	mPath;
		std::wstring	mKey;
		eResourceType	mType;
		
	};
}