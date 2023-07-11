#include "SpriteRenderer.h"
#include "GameObj.h"
#include "Transform.h"
#include "Animator.h"

namespace dru
{
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

	void SpriteRenderer::update()
	{
		BaseRenderer::update();
	}

	void SpriteRenderer::fixedUpdate()
	{
		BaseRenderer::fixedUpdate();
	}

	void SpriteRenderer::render()
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


}