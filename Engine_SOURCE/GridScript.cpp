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
	{
		
	}

	GridScript::~GridScript()
	{
	}

	void GridScript::Initialize()
	{
		UINT type = static_cast<UINT>(GETSINGLE(SceneMgr)->GetActiveScene()->GetType());
		mCamera = renderer::Cameras[type][0];
	}

	void GridScript::update()
	{
		if (nullptr == mCamera)
			return;

		GameObj* gameobj = mCamera->GetOwner();
		Transform* tr = gameobj->GetComponent<Transform>();

		Vector3 campos = tr->GetPosition();
		Vector4 pos = Vector4(campos.x, campos.y, campos.z, 1.f);

		float scale = mCamera->GetScale();;

		RECT winRect;
		GetClientRect(application.GetHwnd(), &winRect);
		float w = static_cast<float>(winRect.right - winRect.left);
		float h = static_cast<float>(winRect.top - winRect.bottom);
		Vector2 resolution(w, h);


		ConstantBuffer* cb = renderer::constantBuffers[static_cast<UINT>(eCBType::Grid)];
		renderer::GridCB data;

		data.cameraPosition = pos;
		data.cameraScale = Vector2(scale, scale);
		data.resolution = resolution;

		cb->SetData(&data);
		cb->Bind(eShaderStage::VS);
		cb->Bind(eShaderStage::PS);
	}

	void GridScript::fixedUpdate()
	{
	}

	void GridScript::render()
	{
	}

}