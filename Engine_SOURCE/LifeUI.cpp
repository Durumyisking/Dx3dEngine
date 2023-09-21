#include "LifeUI.h"
#include "SpriteRenderer.h"
#include "TimeMgr.h"

LifeUI::LifeUI()
	:UIBase(eUIType::None)
	, mCurrentTime(0.0f)
	, mActivate(false)
{
}

LifeUI::LifeUI(const std::wstring& key, eUIType type)
	:UIBase(type)
	, mCurrentTime(0.0f)
	, mActivate(false)
{
	SpriteRenderer* hudRender = this->AddComponent<SpriteRenderer>(eComponentType::UI);
	hudRender->SetMeshByKey(L"Rectmesh");
	hudRender->SetMaterial(GETSINGLE(ResourceMgr)->Find<Material>(key));
}


LifeUI::~LifeUI()
{
}

void LifeUI::OnInit()
{
}

void LifeUI::OnActive()
{
	mActivate = true;
	this->Active();
}

void LifeUI::OnInActive()
{
	mActivate = false;
	this->Die();
}

void LifeUI::OnUpdate()
{
	if (mbUIEnable == false)
		return;

	if (mActivate == false)
		return;


	if (mbColor)
	{
		mCurrentTime += DT;
		this->SetColor(Vector4(0.0f + mCurrentTime, 0.0f + mCurrentTime, 0.0f + mCurrentTime, 1.f), true);

		if (mCurrentTime >= 1.f)
		{
			mCurrentTime = 0;
			//mbColor = false;
		}
	}
}

void LifeUI::OnFixedUpdate()
{
}

void LifeUI::OnRender()
{
}

void LifeUI::OnFontRender()
{
}

void LifeUI::OnClear()
{
}

void LifeUI::SetColor(Vector4 color, bool isColor)
{
	SpriteRenderer* renderer = this->GetComponent<SpriteRenderer>();
	Material* material = renderer->GetMaterial();

	renderer::MaterialCB data = {};
	data.bool3 = isColor;
	data.xyzw1 = color;
	material->SetData(eGPUParam::Bool_3, &data.bool3);
	material->SetData(eGPUParam::Vector4_1, &data.xyzw1);

	mbColor = true;
	mCurrentTime = 0;
}