#pragma once
#include "Engine.h"
#include "Resource.h"

namespace dru
{
	class Resources
	{
	public:
		template <typename T>
		static std::shared_ptr<T> Find(const std::wstring& key)
		{
			std::map<std::wstring, std::shared_ptr<Resource> >::iterator iter = mResources.find(key);

			// �̹� ������ Ű������ �ٸ� ���ҽ��� ���� ��ϵǾ� �־���.
			if (iter != mResources.end())
			{
				return std::dynamic_pointer_cast<T>(iter->second);
			}

			return nullptr;
		}

		template <typename T>
		static std::vector<std::shared_ptr<T>> Finds()
		{
			std::vector<std::shared_ptr<T>> resources = {};
			for (auto iter : mResources)
			{
				std::shared_ptr<T> resource
					= std::dynamic_pointer_cast<T>(iter.second);

				if (nullptr != resource)
					resources.push_back(resource);
			}

			return resources;
		}

		template <typename T>
		static std::shared_ptr<T> Load(const std::wstring& key, const std::wstring& path)
		{
			// Ű������ Ž��
			std::shared_ptr<T> resource = Resources::Find<T>(key);
			if (nullptr != resource)
			{
				// �ش�Ű�� �̹� �ε��Ȱ� ������ �ش� ���ҽ��� ��ȯ
				return resource;
			}

			// �ش� Ű�� �ε��� ���ҽ��� ����.
			resource = std::make_shared<T>();
			if (FAILED(resource->Load(path)))
			{
				MessageBox(nullptr, L"Image Load Failed!", L"Error", MB_OK);
				assert(false);
				return nullptr;
			}

			resource->SetKey(key);
			resource->SetPath(path);
			mResources.insert(std::make_pair(key, std::dynamic_pointer_cast<Resource>(resource)));

			return resource;
		}

		template <typename T>
		static void Insert(const std::wstring& key, std::shared_ptr<T> resource)
		{
			//if (Find<T>(key))
			//{
			//	//std::wstring newKey = key;
			//	//std::wstring idx = std::to_wstring(GetRandomNumber(65535));
			//	//newKey += idx;
			//	mResources.insert(make_pair(key, std::dynamic_pointer_cast<Resource>(resource)));
			//}
			//else
			//{
				mResources.insert(make_pair(key, std::dynamic_pointer_cast<Resource>(resource)));
			//}
		}


	private:
		Resources() = delete;
		~Resources() = delete;

	private:
		static std::map<std::wstring, std::shared_ptr<Resource> > mResources;
	};
}
