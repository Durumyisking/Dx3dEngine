#include "ResourceMgr.h"
#include "Material.h"
#include "FileMgr.h"


ResourceMgr::ResourceMgr()
	: mResources{}
{

}

ResourceMgr::~ResourceMgr()
{

}

Material* ResourceMgr::CreateMaterial(std::wstring texture, eTextureSlot slot, std::wstring shaderName, std::wstring materialName)
{
	Material* mat = new Material(texture, slot, shaderName);
	Insert<Material>(materialName, mat);
	return mat;
}

Material* ResourceMgr::CreateMaterial(std::wstring shaderName, std::wstring materialName)
{
	Material* mat = new Material(shaderName);
	Insert<Material>(materialName, mat);
	return mat;
}

Material* ResourceMgr::CreateMaterial(std::wstring textureColor, std::wstring shaderName, std::wstring materialName)
{
	Material* mat = new Material(textureColor, shaderName);
	Insert<Material>(materialName, mat);
	return mat;
}

Material* ResourceMgr::CreateMaterial(std::wstring textureColor, std::wstring textureNormal, std::wstring shaderName, std::wstring materialName)
{
	Material* mat = new Material(textureColor, textureNormal, shaderName);
	Insert<Material>(materialName, mat);
	return mat;
}

Material* ResourceMgr::CreateMaterial(std::wstring textureColor, std::wstring textureNormal, std::wstring textureMetal, std::wstring shaderName, std::wstring materialName)
{
	Material* mat = new Material(textureColor, textureNormal, textureMetal, shaderName);
	Insert<Material>(materialName, mat);
	return mat;
}

Material* ResourceMgr::CreateMaterial(std::wstring textureColor, std::wstring textureNormal, std::wstring textureMetal, std::wstring textureRoughness, std::wstring shaderName, std::wstring materialName)
{
	Material* mat = new Material(textureColor, textureNormal, textureMetal, textureRoughness, shaderName);
	Insert<Material>(materialName, mat);
	return mat;
}

Material* ResourceMgr::CreateMaterial(std::wstring textureColor, std::wstring textureNormal, std::wstring textureMetal, std::wstring textureRoughness, std::wstring textureEmissive, std::wstring shaderName, std::wstring materialName)
{
	Material* mat = new Material(textureColor, textureNormal, textureMetal, textureRoughness, textureEmissive, shaderName);
	Insert<Material>(materialName, mat);
	return mat;
}

void ResourceMgr::Initalize()
{
	//// Mario
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/MarioBody", L"Mario");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/MarioHandL", L"MarioHandL");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/MarioHandR", L"MarioHandR");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/MarioFace", L"MarioFace");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/MarioHead", L"MarioHead");

	//// Monster
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/Tank", L"Tank");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/BirdCity", L"BirdCity");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CapMan", L"CapMan");
	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/goomba", L"goomba");
	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/Packun", L"Packun");

	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWomans", L"CityWomans");

	//GETSINGLE(FileMgr)->ModelLoad(L"..//Resources//brick", L"BlockBrick");

	GETSINGLE(FileMgr)->ModelLoad(L"..//Resources/CityWorld/CityWorldHomeGround/000", L"CityWorldHomeGround000");

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
