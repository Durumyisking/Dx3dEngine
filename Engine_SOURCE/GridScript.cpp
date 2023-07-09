#include "GridScript.h"
#include "Transform.h"
#include "GameObj.h"
#include "ConstantBuffer.h"
#include "Renderer.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Application.h"

extern dru::Application application;

namespace dru
{
	GridScript::GridScript()
		: Script()
		, mCamera(nullptr)
		, mThickness(2.f)
		, mGridOffset{100.f, 100.f}
	{
		
	}

	GridScript::~GridScript()
	{
	}

	void GridScript::Initialize()
	{
		UINT type = static_cast<UINT>(SceneMgr::mActiveScene->GetType());
		mCamera = renderer::Cameras[type][0];
	}

	void GridScript::update()
	{

	}

	void GridScript::fixedUpdate()
	{
		if (nullptr == mCamera)
			return;

		MeshRenderer* mr = GetOwner()->GetComponent<MeshRenderer>();

		GameObj* cam = mCamera->GetOwner();
		Transform* tr = cam->GetComponent<Transform>();
		Vector3 campos = tr->GetPosition();

		__int32 w = application.GetWidth();
		__int32 h = application.GetHeight();
		Vector2 resolution(w, h);

		mr->GetMaterial()->SetData(eGPUParam::Vector3_1, &campos);
		mr->GetMaterial()->SetData(eGPUParam::Vector2_1, &mGridOffset);
		mr->GetMaterial()->SetData(eGPUParam::Vector2_2, &resolution);
		mr->GetMaterial()->SetData(eGPUParam::Float_1, &mThickness);
	}

	void GridScript::render()
	{
	}

}