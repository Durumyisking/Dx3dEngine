#include "UIFactory.h"
#include "Panal.h"
#include "UIFactory.h"
#include "Panal.h"
#include "HUD.h"
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

Panal* UIFactory::CreatePanal(GameObj* parent, Vector3 pos, Vector3 scale, const std::wstring& name)
{
	Panal* panal = object::Instantiate<Panal>(eLayerType::UI);
	panal->SetPos(pos);
	panal->SetScale(scale);
	panal->SetName(name);
	panal->GetComponent<Transform>()->SetParent(parent);

	return panal;
}

HUD* UIFactory::CreateHud(const std::wstring& name, const std::wstring& key, Vector3 pos, Vector3 scale, GameObj* parent)
{
	HUD* hud = object::Instantiate<HUD>(eLayerType::UI);
	hud->SetPos(pos);
	hud->SetScale(scale);
	hud->SetName(name);
	SpriteRenderer* hudRender = hud->AddComponent<SpriteRenderer>(eComponentType::UI);
	hudRender->SetMeshByKey(L"Rectmesh");
	hudRender->SetMaterial(GETSINGLE(ResourceMgr)->Find<Material>(key));
	hud->GetComponent<Transform>()->SetParent(parent);

	return hud;
}

HUD* UIFactory::CreateHud(const std::wstring& name, const std::wstring& key, Vector3 pos, Vector3 scale)
{
	HUD* hud = object::Instantiate<HUD>(eLayerType::UI);
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
