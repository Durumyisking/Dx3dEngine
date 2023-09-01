#include "UIFactory.h"
#include "Panal.h"
#include "HUD.h"
#include "Button.h"

UIFactory::UIFactory()
{
}

UIFactory::~UIFactory()
{

}

void UIFactory::Initialize()
{
}

Panal* UIFactory::CreatePanal(GameObj* parent, Vector3 pos, Vector3 scale , const std::wstring& name)
{
	Panal* panal = object::Instantiate<Panal>(eLayerType::UI);
	panal->SetPos(pos);
	panal->SetScale(scale);
	panal->SetName(name);
	panal->GetComponent<Transform>()->SetParent(parent);

	return panal;
}

void UIFactory::CreateHud()
{
}

void UIFactory::CreateButton()
{
}
