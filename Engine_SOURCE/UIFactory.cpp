#include "UIFactory.h"
#include "Panal.h"
#include "UIFactory.h"
#include "Panal.h"
#include "HUD.h"
#include "ImageUI.h"
#include "Button.h"
#include "SpriteRenderer.h"


UIFactory::UIFactory()
{
}

UIFactory::~UIFactory()
{

}

void UIFactory::Initialize()
{
}

Panal* UIFactory::CreatePanal(GameObj* parent, Vector3 pos, Vector3 scale, const std::wstring& name,Scene* sceene)
{
	Panal* panal = object::Instantiate<Panal>(eLayerType::UI, sceene);
	panal->SetPos(pos);
	panal->SetScale(scale);
	panal->SetName(name);
	panal->GetComponent<Transform>()->SetParent(parent);

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

HUD* UIFactory::CreateHud(const std::wstring& name, const std::wstring& key, Vector3 pos, Vector3 scale, Scene* sceene)
{
	HUD* hud = object::Instantiate<HUD>(eLayerType::UI, sceene);
	hud->SetPos(pos);
	hud->SetScale(scale);
	hud->SetName(name);
	MeshRenderer* hudRender = hud->AddComponent<MeshRenderer>(eComponentType::UI);
	hudRender->SetMeshByKey(L"Rectmesh");
	hudRender->SetMaterial(GETSINGLE(ResourceMgr)->Find<Material>(key));

	return hud;
}

void UIFactory::CreateButton()
{
}
