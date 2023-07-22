#include "Animation.h"
#include "TimeMgr.h"



Animation::Animation()
	: mAnimator(nullptr)
	, mAtlas(nullptr)
	, mSpriteSheet{}
	, mIndex(-1)
	, mTime(0.f)
	, mbComplete(false)
	, mSpriteLength(0)
	, mbReversePlay(false)
	, mbCompleteEventPlayed(false)
{

}

Animation::~Animation()
{
}

UINT Animation::update()
{
	if (mbComplete)
		return -1;

	// �ð� üũ
	mTime += DT;

	// ���� �ð��� �ش� �������� �����ð��� �Ѿ�� 
	if (mSpriteSheet[mIndex].duration < mTime)
	{
		mTime = 0.f;
		if (mbReversePlay)
		{
			--mIndex;

			if (0 == mIndex)
			{
				mbComplete = true;
				mIndex = mSpriteLength;
			}

			return mIndex;

		}
		else
		{
			++mIndex;

			if (mSpriteSheet.size() <= mIndex)
			{
				mbComplete = true;
				mIndex = static_cast<int>(mSpriteSheet.size() - 1);

			}

			return mIndex;
		}	
	}
	return -1;
}

void Animation::fixedUpdate()
{
}

void Animation::render()
{
}

void Animation::Create(const std::wstring& name, Texture* atlas, Vector2 leftTop, Vector2 size, Vector2 offset, UINT spriteLength, Vector2 ratio, float duration, bool reverse)
{
	mAnimationName = name;
	mAtlas = atlas;
	mbReversePlay = reverse;
	mSpriteLength = spriteLength;

	float width = (float)atlas->GetWidth();
	float height = (float)atlas->GetHeight();

	size_t CollSkipCount = 0;
	for (size_t i = 0; i < spriteLength; i++)
	{
		Sprite sprite = {};

		// ��������Ʈ�� LT�� + ����� width�� �Ѿ�� �����ٷ� �ٲ۴�.
		if (leftTop.x + (size.x * (float)i) >= width)
		{
			leftTop.x = 0.f;
			leftTop.y += size.y;

			CollSkipCount = i;
		}

		// uv��ǥ�� �ѱ�� ���� width, height�� ������.
		sprite.LT = Vector2(
			(leftTop.x + (size.x * (float)(i - CollSkipCount))) / width,
			leftTop.y / height
		);

		sprite.size = Vector2(fabs(size.x) / width, fabs(size.y) / height);
		sprite.offset = Vector2(offset.x / width, offset.y / height);
		sprite.duration = duration;
		sprite.altasSize = Vector2(ratio.x / width, ratio.y / height);

		mSpriteSheet.push_back(sprite);
	}
}

void Animation::BindShader()
{
	mAtlas->BindShaderResource(eShaderStage::PS, 12); // ��Ʋ�� srv�� ���ε�

	ConstantBuffer* cb =  renderer::constantBuffers[static_cast<UINT>(eCBType::Animation)];

	renderer::AnimationCB data = {};
	data.type = static_cast<UINT>(eAnimationType::SecondDimension);
	data.LT = mSpriteSheet[mIndex].LT;
	data.offset = mSpriteSheet[mIndex].offset;
	data.size = mSpriteSheet[mIndex].size;
	data.atlasSize = mSpriteSheet[mIndex].altasSize;

	cb->SetData(&data);
	cb->Bind(eShaderStage::PS);
}

void Animation::BindSpriteToShader(renderer::AnimationCB _Sprite)
{
	mAtlas->BindShaderResource(eShaderStage::PS, 12);

	ConstantBuffer* cb = renderer::constantBuffers[static_cast<UINT>(eCBType::Animation)];

	cb->SetData(&_Sprite);
	cb->Bind(eShaderStage::PS);
}


void Animation::Clear()
{
	// GUITexture Clear
	Texture::Clear(12);

	ConstantBuffer* cb = renderer::constantBuffers[static_cast<UINT>(eCBType::Animation)];
	renderer::AnimationCB data = {};
	data.type = static_cast<UINT>(eAnimationType::None);

	cb->SetData(&data);
	cb->Bind(eShaderStage::PS);

}

void Animation::Reset()
{
	mTime = 0.f;

	if(mbReversePlay)
		mIndex = mSpriteLength;
	else
		mIndex = 0;

	mbComplete = false;
	mbCompleteEventPlayed = false;
}

void Animation::SetDuration(float value)
{
	for (size_t i = 0; i < mSpriteSheet.size(); i++)
	{	
		mSpriteSheet[i].duration = value;
	}
}

renderer::AnimationCB Animation::GetAnimationData()
{
	renderer::AnimationCB data = {};
		
	data.type = static_cast<UINT>(eAnimationType::SecondDimension);
	data.LT = mSpriteSheet[mIndex].LT;
	data.offset = mSpriteSheet[mIndex].offset;
	data.size = mSpriteSheet[mIndex].size;
	data.atlasSize = mSpriteSheet[mIndex].altasSize;

	return data;
}


// �� k s if�� ����
// �� j �Լ�ã��
// �� mm �ڵ��� �ݰ� ����
// ��Ʈ�� RO �Լ� ��������