#pragma once
#include "Engine.h"
#include "EngineResource.h"
#include "SimpleMath.h"
#include "Graphics.h"


class Material;
class ResourceMgr
{
	SINGLE(ResourceMgr)

public:
	template <typename T>
	T* Find(const std::wstring& key)
	{
		std::map<std::wstring, Resource*>::iterator iter = mResources.find(key);

		// 이미 동일한 키값으로 다른 리소스가 먼저 등록되어 있었다.
		if (iter != mResources.end())
		{
			return dynamic_cast<T*>(iter->second);
		}

		return nullptr;
	}

	template <typename T>
	std::vector<T*> Finds()
	{
		std::vector<T*> resources = {};
		for (auto iter : mResources)
		{
			T* resource
				= dynamic_cast<T*>(iter.second);

			if (nullptr != resource)
				resources.push_back(resource);
		}

		return resources;
	}

	template <typename T>
	T* Load(const std::wstring& key, const std::wstring& path)
	{
		// 키값으로 탐색
		T* resource = GETSINGLE(ResourceMgr)->Find<T>(key);
		if (nullptr != resource)
		{
			// 해당키로 이미 로딩된게 있으면 해당 리소스를 반환
			return resource;
		}

		// 해당 키로 로딩된 리소스가 없다.
		resource = new T();
		if (FAILED(resource->Load(path)))
		{
			MessageBox(nullptr, L"Image Load Failed!", L"Error", MB_OK);
			assert(false);
			return nullptr;
		}

		resource->SetKey(key);
		resource->SetPath(path);
		mResources.insert(std::make_pair(key, resource));

		return resource;
	}

	template <typename T>
	void Insert(const std::wstring& key, T* resource)
	{
		if (Find<T>(key))
		{
			std::wstring newKey = key;
			std::wstring idx = std::to_wstring(math::GetRandomNumber(65535));
			newKey += idx;
			mResources.insert(std::make_pair(newKey, resource));
			dynamic_cast<DruEntity*>(resource)->SetName(newKey);
		}
		else
		{
			mResources.insert(std::make_pair(key, resource));
			dynamic_cast<DruEntity*>(resource)->SetName(key);
		}
	}

	Material* CreateMaterial(std::wstring texture, eTextureSlot slot, std::wstring shaderName, std::wstring materialName);
	Material* CreateMaterial(std::wstring textureColor, std::wstring shaderName, std::wstring materialName);
	Material* CreateMaterial(std::wstring textureColor, std::wstring textureNormal, std::wstring shaderName, std::wstring materialName);
	Material* CreateMaterial(std::wstring textureColor, std::wstring textureNormal, std::wstring textureMetal, std::wstring shaderName, std::wstring materialName);
	Material* CreateMaterial(std::wstring textureColor, std::wstring textureNormal, std::wstring textureMetal, std::wstring textureRoughness, std::wstring shaderName, std::wstring materialName);
	Material* CreateMaterial(std::wstring textureColor, std::wstring textureNormal, std::wstring textureMetal, std::wstring textureRoughness, std::wstring textureEmissive, std::wstring shaderName, std::wstring materialName);

	void Release();
private:
	std::map<std::wstring, Resource* > mResources;
};
