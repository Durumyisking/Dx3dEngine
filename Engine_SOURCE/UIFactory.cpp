#include "UIFactory.h"
#include "Panal.h"
#include "UIFactory.h"
#include  "UIBase.h"
#include "Panal.h"
#include "HUD.h"
#include "ImageUI.h"
#include "Button.h"
#include "UIManager.h"

UIFactory::UIFactory()
{
}

UIFactory::~UIFactory()
{

}

void UIFactory::Initialize()
{
}

Panal* UIFactory::CreatePanal(GameObj* parent, Vector3 pos, Vector3 scale, const std::wstring& name, Scene* sceene, eUIType type)
{
	Panal* panal = object::Instantiate<Panal>(eLayerType::UI, sceene);
	panal->SetPos(pos);
	panal->SetScale(scale);
	panal->SetName(name);
	panal->GetComponent<Transform>()->SetParent(parent);


	(GETSINGLE(UIManager)->PushPanal(type, panal));
	return panal;
}

HUD* UIFactory::CreateHud(const std::wstring& name, const std::wstring& key, Vector3 pos, Vector3 scale, GameObj* parent, Scene* sceene)
{
	HUD* hud = object::Instantiate<HUD>(eLayerType::UI, sceene);
	hud->SetPos(pos);
	hud->SetScale(scale);
	hud->SetName(name);
	SpriteRenderer* hudRender = hud->AddComponent<SpriteRenderer>(eComponentType::UI);
	hudRender->SetMeshByKey(L"Rectmesh");
	hudRender->SetMaterial(GETSINGLE(ResourceMgr)->Find<Material>(key));
	hud->GetComponent<Transform>()->SetParent(parent);

	return hud;
}

ImageUI* UIFactory::CreateImage(const std::wstring& name, const std::wstring& key, Vector3 pos, Vector3 scale, GameObj* parent, Scene* sceene)
{
	ImageUI* image = object::Instantiate<ImageUI>(eLayerType::UI, sceene);
	image->SetPos(pos);
	image->SetScale(scale);
	image->SetName(name);
	SpriteRenderer* imageRender = image->AddComponent<SpriteRenderer>(eComponentType::UI);
	imageRender->SetMeshByKey(L"Rectmesh");
	imageRender->SetMaterial(GETSINGLE(ResourceMgr)->Find<Material>(key));
	image->GetComponent<Transform>()->SetParent(parent);


	return image;
}


ImageUI* UIFactory::CreateImage(const std::wstring& name, const std::wstring& key, Vector3 pos, Vector3 scale, Scene* sceene)
{
	ImageUI* image = object::Instantiate<ImageUI>(eLayerType::UI, sceene);
	image->SetPos(pos);
	image->SetScale(scale);
	image->SetName(name);
	SpriteRenderer* imageRender = image->AddComponent<SpriteRenderer>(eComponentType::UI);
	imageRender->SetMeshByKey(L"Rectmesh");
	imageRender->SetMaterial(GETSINGLE(ResourceMgr)->Find<Material>(key));

	return image;
}

HUD* UIFactory::CreateHud(const std::wstring& name, const std::wstring& key, Vector3 pos, Vector3 scale, Scene* sceene)
{
	HUD* hud = object::Instantiate<HUD>(eLayerType::UI, sceene);
	hud->SetPos(pos);
	hud->SetScale(scale);
	hud->SetName(name);
	SpriteRenderer* hudRender = hud->AddComponent<SpriteRenderer>(eComponentType::UI);
	hudRender->SetMeshByKey(L"Rectmesh");
	hudRender->SetMaterial(GETSINGLE(ResourceMgr)->Find<Material>(key));

	return hud;
}

void UIFactory::CreateButton()
{
}



void UIFactory::CreateString(const std::wstring& string, Vector3 pos, Vector3 scale, float interval, UIBase* parent, Scene* scene)
{
	for (size_t i = 0; i < string.size(); i++)
	{
		if (isspace(string[i]))
			continue;

		if (!iswdigit(string[i]))
		{
			if (isupper(string[i]))
			{
				int a = (int)string[i];
				parent->Addchild(CreateImage(L"Name" + string, L"uppercase_" + std::to_wstring(a) + L"Material", Vector3(pos.x + (interval * i), pos.y, pos.z), scale, parent, scene));
			}
			else
			{
				int a = (int)string[i];
				parent->Addchild(CreateImage(L"Name" + string, L"lowercase_" + std::to_wstring(a) + L"Material", Vector3(pos.x + (interval * i), pos.y, pos.z), scale, parent, scene));
			}
		}
		else if (iswdigit(string[i]))
		{
			int a = string[i];
			a = a - '0';
			parent->Addchild(CreateImage(L"Name" + string, std::to_wstring(a) + L"Material", Vector3(pos.x + (interval * i), pos.y, pos.z), scale, parent, scene));
		}
	}
}

void UIFactory::CreateString(const std::wstring& string, const std::wstring& matKey, Vector3 pos, Vector3 scale, Vector3 rot, float interval, UIBase* parent, Scene* scene)
{
	for (size_t i = 0; i < string.size(); i++)
	{
		if (isspace(string[i]))
			continue;

		if (!iswdigit(string[i]))
		{
			if (isupper(string[i]))
			{
				int a = (int)string[i];
				parent->Addchild(CreateImage(L"Name" + string, L"uppercase_" + std::to_wstring(a) + L"Material", Vector3(pos.x + (interval * i), pos.y, pos.z), scale, parent, scene));
			}
			else
			{
				int a = (int)string[i];
				parent->Addchild(CreateImage(L"Name" + string, L"lowercase_" + std::to_wstring(a) + L"Material", Vector3(pos.x + (interval * i), pos.y, pos.z), scale, parent, scene));
			}
		}
		else if (iswdigit(string[i]))
		{
			int a = string[i];
			a = a - '0';

			parent->Addchild(CreateImage(L"Name" + string, matKey + std::to_wstring(a) + L"Material", Vector3(pos.x + (interval * i), pos.y + (i * 0.0f), pos.z), scale, parent, scene));
			parent->GetChilds()[i]->GetComponent<Transform>()->SetRotation(rot);
		}
	}
}
