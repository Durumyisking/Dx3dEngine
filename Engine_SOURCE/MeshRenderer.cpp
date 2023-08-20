#include "MeshRenderer.h"
#include "GameObj.h"
#include "Transform.h"
#include "Model.h"
#include "Mesh.h"



MeshRenderer::MeshRenderer()
	: BaseRenderer(eComponentType::MeshRenderer)
{
}

MeshRenderer::~MeshRenderer()
{
}

void MeshRenderer::Initialize()
{
	//t.loadFromFile("E:/Dev/3d-my/Dx3dEngine/Resources/environment.hdr");

	BaseRenderer::Initialize();
}

void MeshRenderer::Update()
{
	BaseRenderer::Update();
}

void MeshRenderer::FixedUpdate()
{
	BaseRenderer::FixedUpdate();
}

void MeshRenderer::Render()
{
	BaseRenderer::Render();

	GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

	GetMaterial()->Bind();

	//t.Bind(0);
	//t.Bind(28);

	//Texture* t = GETSINGLE(ResourceMgr)->Find<Texture>(L"noise1");
	//t->BindAllShaderResource(0);
	//t->BindAllShaderResource(28);


	GetModel() != nullptr ? GetModel()->Bind_Render(GetMaterial()) : GetMesh()->BindBuffer(), GetMesh()->Render();

	GetMaterial()->Clear();
}
