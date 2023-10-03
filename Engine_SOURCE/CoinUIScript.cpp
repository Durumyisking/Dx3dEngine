#include "CoinUIScript.h"
#include "Texture.h"
#include "UIManager.h"
#include "Material.h"
#include "TimeMgr.h"

CoinUIScript::CoinUIScript()
	: UIScript()
	, mPanal(nullptr)
	, mbCoin(false)
	, mCurrentTime(0.0f)
	, mCoin(0)
{

}

CoinUIScript::~CoinUIScript()
{

}

void CoinUIScript::Initialize()
{
	mPanal = dynamic_cast<Panal*>(this->GetOwner());
}

void CoinUIScript::Update()
{
	if (!isRunning)
		return;

	if (mbCoin)
	{
		mCurrentTime += DT;


		if (mCurrentTime >= 1.0f)
		{
			for (size_t i = 0; i < mPanal->GetChilds().size(); i++)
			{
				mPanal->GetChilds()[i]->SetColor(Vector4(1.0f, 1.0f, 1.0f, 1.0f), true);
			}
			mCurrentTime = 0;
			mbCoin = false;
		}
	}
}

void CoinUIScript::FixedUpdate()
{
	if (!isRunning)
		return;


}

void CoinUIScript::GetCoin()
{
	mCoin++;
	mCurrentTime = 0;

	int ones = mCoin % 10;
	int ten = (mCoin / 10) % 10;
	int hundreds = mCoin / 100;

	for (size_t i = 0; i < mPanal->GetChilds().size(); i++)
	{
		mPanal->GetChilds()[i]->SetColor(Vector4(1.f, 0.92f, 0.016f, 1.f), true);

	}

	Texture* tex = (GETSINGLE(ResourceMgr)->Find<Texture>(std::to_wstring(ones)));
	mPanal->GetChilds()[3]->GetMaterial()->SetTexture(tex);

	tex = (GETSINGLE(ResourceMgr)->Find<Texture>(std::to_wstring(ten)));
	mPanal->GetChilds()[2]->GetMaterial()->SetTexture(tex);

	tex = (GETSINGLE(ResourceMgr)->Find<Texture>(std::to_wstring(hundreds)));
	mPanal->GetChilds()[1]->GetMaterial()->SetTexture(tex);
	
	mbCoin = true;
}
