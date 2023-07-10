#include "BaseRenderer.h"
#include "Texture.h"
#include "Transform.h"
#include "GameObj.h"
#include "Material.h"
#include "Mesh.h"

namespace dru
{
	BaseRenderer::BaseRenderer(eComponentType type)
		:Component(type)
		, mbIsChanged(false)
		, mbIsAnim(false)
		, mbUseLOD(true)
		, mSpriteSize(Vector2::Zero)
	{
		// µðÆúÆ® ¸Å½Ã ÁöÁ¤
		Mesh* mesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"Cubemesh");

		SetMesh(mesh);
	}
	BaseRenderer::~BaseRenderer()
	{
	}
	void BaseRenderer::Initialize()
	{
	}
	void BaseRenderer::update()
	{
	}
	void BaseRenderer::fixedUpdate()
	{
		if (mbIsChanged)
		{

			if (mWidthRatio == 0.f && mHeightRatio == 0.f)
				return;

			Transform* transform = GetOwner()->GetComponent<Transform>();

			Vector3 scale = transform->GetScale();
			Vector3 scaleTemp = transform->GetScale();

			scale.x = mWidthRatio;
			scale.y = mHeightRatio;

			scale *= scaleTemp;


			transform->SetScale(scale);

			mbIsChanged = false;
		}
	}

	void BaseRenderer::render()
	{
		if(mbUseLOD)
			LOD();	
	}


	void BaseRenderer::SetMeshByKey(std::wstring key)
	{
		mMesh = GETSINGLE(ResourceMgr)->Find<Mesh>(key);
	}

	void BaseRenderer::SetMaterial(Material* material)
	{
		mMaterial = material;

		// adjustTexture();
	}

	void BaseRenderer::SetMaterialByKey(std::wstring key)
	{
		mMaterial = GETSINGLE(ResourceMgr)->Find<Material>(key);
		// adjustTexture();
	}

	void BaseRenderer::SetAnimMaterial(Material* material, Vector2 spriteSize)
	{
		mMaterial = material;
		mbIsAnim = true;
		mSpriteSize = spriteSize;
		// adjustTexture();
	}

	void BaseRenderer::ChangeColor(Vector4 color)
	{
		MulColor(Vector4::Zero);
		AddColor(color);
	}

	void BaseRenderer::MulColor(Vector4 color)
	{
		mMaterial->SetData(eGPUParam::Vector4_1, &color);
	}

	void BaseRenderer::AddColor(Vector4 color)
	{
		mMaterial->SetData(eGPUParam::Vector4_2, &color);
	}

	void BaseRenderer::LOD()
	{
		Vector3 camPos = renderer::mainCamera->GetOwnerWorldPos();
		Vector3 objPos = GetOwnerWorldPos();

		float distance = Vector3::Distance(camPos, objPos);

		if (mMaterial)
		{
			if (distance > 20.f)
			{
				mMaterial->SetShaderByKey(L"FlatShader");
			}
			else
			{
				mMaterial->SetShaderByKey(L"PhongShader");
			}
		}
	}

	void BaseRenderer::adjustTexture()
	{
		Texture* texture = GetMaterial()->GetTexture(eTextureSlot::T0);

		if (nullptr == texture)
			return;

		int width;
		int height;

		if (mbIsAnim)
		{
			width = static_cast<int>(mSpriteSize.x);
			height = static_cast<int>(mSpriteSize.y);
		}
		else
		{
			width = static_cast<int>(texture->GetScratchImage().GetMetadata().width);
			height = static_cast<int>(texture->GetScratchImage().GetMetadata().height);
		}

		if (width == 0 || height == 0)
			return;

		int widthcount = 0;
		int heightcount = 0;

		int GCD = std::gcd(width, height);

		float fwidth = static_cast<float>(width /= GCD);
		float fheight = static_cast<float>(height /= GCD);


		while (width > 0.f)
		{
			fwidth /= 10.f;
			width /= 10;
			++widthcount;
		}
		while (height > 0.f)
		{
			fheight /= 10.f;
			height /= 10;
			++heightcount;
		}


		if (widthcount == heightcount)
		{
			fwidth *= 10.f;
			fheight *= 10.f;
		}
		else if (widthcount < heightcount)
		{
			fwidth *= 10.f;
			fheight *= 100.f;
		}
		else
		{
			fwidth *= 100.f;
			fheight *= 10.f;
		}

		mWidthRatio = fwidth;
		mHeightRatio = fheight;

	}
}
