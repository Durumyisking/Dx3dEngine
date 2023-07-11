#include "FadeScript.h"
#include "Transform.h"
#include "GameObj.h"
#include "Application.h"
#include "ConstantBuffer.h"
#include "TimeMgr.h"
#include "InputMgr.h"
#include "Material.h"
#include "BaseRenderer.h"


namespace dru
{
	FadeScript::FadeScript()
		: mFadeValue(1.f)
		, mFadeType(1)
		, mStart(1)
		, mTime(1.f)
		, mElapsedTime(0.f)
		, mFadeTextureType(1)
	{
	}

	FadeScript::~FadeScript()
	{
	}

	void FadeScript::Initialize()
	{
	}

	void FadeScript::update()
	{
	}

	void FadeScript::fixedUpdate()
	{
	}

	void FadeScript::render()
	{
		if (1 == mStart)
		{
			mElapsedTime += DT;

			BaseRenderer* renderer = GetOwner()->GetComponent<BaseRenderer>();
			dru::Material* material = renderer->GetMaterial();

			renderer::MaterialCB data = {};

			data.fData1 = mFadeValue * (mElapsedTime / mTime);
			data.iData1 = mFadeType;
			data.iData2 = mFadeTextureType;


			material->SetData(eGPUParam::Float_1, &data.fData1);
			material->SetData(eGPUParam::Int_1, &data.iData1);
			material->SetData(eGPUParam::Int_2, &data.iData2);
		}

	}

	void FadeScript::restart(int _fadeType)
	{
		mFadeType = _fadeType;
		mTime = 0;
	}

}