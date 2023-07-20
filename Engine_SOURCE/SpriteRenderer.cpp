#include "SpriteRenderer.h"
#include "GameObj.h"
#include "Transform.h"
#include "Animator.h"



SpriteRenderer::SpriteRenderer()
	: BaseRenderer(eComponentType::Renderer)
{
}

SpriteRenderer::~SpriteRenderer()
{
}

void SpriteRenderer::Initialize()
{
	BaseRenderer::Initialize();
}

void SpriteRenderer::Update()
{
	BaseRenderer::Update();
}

void SpriteRenderer::FixedUpdate()
{
	BaseRenderer::FixedUpdate();
}

void SpriteRenderer::Render()
{
//		BaseRenderer::Render();

	GetOwner()->GetComponent<Transform>()->SetConstantBuffer();

	GetMesh()->BindBuffer();
	GetMaterial()->Bind();


	Animator* animator = GetOwner()->GetComponent<Animator>();

	if (animator)
	{
		animator->Binds();
	}	

	GetMesh()->Render();

	GetMaterial()->Clear();
			
	if (animator)
		animator->Clear();

}

