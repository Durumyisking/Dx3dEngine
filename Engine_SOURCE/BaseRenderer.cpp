#include "BaseRenderer.h"
#include "Texture.h"
#include "Transform.h"
#include "GameObj.h"

namespace dru
{
	BaseRenderer::BaseRenderer(eComponentType _Type)
		:Component(_Type)
		, mbIsChanged(false)
		, mbIsAnim(false)
		, mbUseLOD(true)
		, mSpriteSize(Vector2::Zero)
	{
		// 디폴트 매시 지정
		std::shared_ptr<Mesh> mesh = GETSINGLE(Resources)->Find<Mesh>(L"Cubemesh");

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

	void BaseRenderer::SetMeshByKey(std::wstring _Key)
	{
		mMesh = GETSINGLE(Resources)->Find<Mesh>(_Key);
	}

	void BaseRenderer::SetMaterial(std::shared_ptr<Material> _Material)
	{
		mMaterial = _Material;

		// adjustTexture();
	}

	void BaseRenderer::SetMaterialByKey(std::wstring _Key)
	{
		mMaterial = GETSINGLE(Resources)->Find<Material>(_Key);

		// adjustTexture();
	}

	void BaseRenderer::SetAnimMaterial(std::shared_ptr<Material> _Material, Vector2 _SpriteSize)
	{
		mMaterial = _Material;
		mbIsAnim = true;
		mSpriteSize = _SpriteSize;
		// adjustTexture();
	}

	void BaseRenderer::ChangeColor(Vector4 _color)
	{
		MulColor(Vector4::Zero);
		AddColor(_color);
	}

	void BaseRenderer::MulColor(Vector4 _color)
	{
		mMaterial->SetData(eGPUParam::Vector4_1, &_color);
	}

	void BaseRenderer::AddColor(Vector4 _color)
	{
		mMaterial->SetData(eGPUParam::Vector4_2, &_color);
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
				mMaterial.get()->SetShaderByKey(L"FlatShader");
			}
			else
			{
				mMaterial.get()->SetShaderByKey(L"PhongShader");
			}
		}
	}

	void BaseRenderer::adjustTexture()
	{
		std::shared_ptr<Texture> texture = GetMaterial()->GetTexture(eTextureSlot::T0);

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
