#include "GridScript.h"
#include "Transform.h"
#include "GameObj.h"
#include "ConstantBuffer.h"
#include "Renderer.h"
#include "SceneMgr.h"
#include "Scene.h"
#include "Application.h"
#include "MeshRenderer.h"

extern Application application;



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
	UINT type = static_cast<UINT>(GETSINGLE(SceneMgr)->GetActiveScene()->GetType());
	mCamera = renderer::Cameras[type][0];
}

void GridScript::Update()
{

}

void GridScript::FixedUpdate()
{
	if (nullptr == mCamera)
		return;

	MeshRenderer* mr = GetOwner()->GetComponent<MeshRenderer>();

	GameObj* cam = mCamera->GetOwner();
	Transform* tr = cam->GetComponent<Transform>();
	Vector3 campos = tr->GetPosition();

	float w = static_cast<float>(application.GetWidth());
	float h = static_cast<float>(application.GetHeight());
	Vector2 resolution(w, h);

	mr->GetMaterial()->SetData(eGPUParam::Vector3_1, &campos);
	mr->GetMaterial()->SetData(eGPUParam::Vector2_1, &mGridOffset);
	mr->GetMaterial()->SetData(eGPUParam::Vector2_2, &resolution);
	mr->GetMaterial()->SetData(eGPUParam::Float_1, &mThickness);
}

void GridScript::Render()
{
}

