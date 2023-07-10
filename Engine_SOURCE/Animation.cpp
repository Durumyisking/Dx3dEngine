#include "Animation.h"
#include "TimeMgr.h"


namespace dru
{
	CAnimation::CAnimation()
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

	CAnimation::~CAnimation()
	{
	}

	UINT CAnimation::update()
	{
		if (mbComplete)
			return -1;

		// �ð� üũ
		mTime += CTimeMgr::DeltaTime();

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

	void CAnimation::fixedUpdate()
	{
	}

	void CAnimation::render()
	{
	}

<<<<<<< Updated upstream
	void CAnimation::Create(const std::wstring& _name, std::shared_ptr<CTexture> _atlas, Vector2 _leftTop, Vector2 _size, Vector2 _offset, UINT _spriteLength, Vector2 _Ratio, float _duration, bool _Reverse)
=======
	void Animation::Create(const std::wstring& name, Texture* atlas, Vector2 leftTop, Vector2 size, Vector2 offset, UINT spriteLength, Vector2 ratio, float duration, bool reverse)
>>>>>>> Stashed changes
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

	void CAnimation::BindShader()
	{
		mAtlas->BindShaderResource(eShaderStage::PS, 12); // ��Ʋ�� srv�� ���ε�

		CConstantBuffer* cb =  renderer::constantBuffers[(UINT)eCBType::Animation];

		renderer::AnimationCB data = {};

		data.type = (UINT)eAnimationType::SecondDimension;
		data.LT = mSpriteSheet[mIndex].LT;
		data.offset = mSpriteSheet[mIndex].offset;
		data.size = mSpriteSheet[mIndex].size;
		data.atlasSize = mSpriteSheet[mIndex].altasSize;

		cb->SetData(&data);
		cb->Bind(eShaderStage::PS);
	}

<<<<<<< Updated upstream
	void CAnimation::BindSpriteToShader(renderer::AnimationCB _Sprite)
=======
	void Animation::BindSpriteToShader(renderer::AnimationCB sprite)
>>>>>>> Stashed changes
	{
		mAtlas->BindShaderResource(eShaderStage::PS, 12);

		CConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::Animation];

		cb->SetData(&sprite);
		cb->Bind(eShaderStage::PS);
	}


	void CAnimation::Clear()
	{
		// Texture Clear
		CTexture::Clear(12);

		CConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::Animation];
		renderer::AnimationCB data = {};
		data.type = (UINT)eAnimationType::None;

		cb->SetData(&data);
		cb->Bind(eShaderStage::PS);

	}

	void CAnimation::Reset()
	{
		mTime = 0.f;

		if(mbReversePlay)
			mIndex = mSpriteLength;
		else
			mIndex = 0;

		mbComplete = false;
		mbCompleteEventPlayed = false;
	}

<<<<<<< Updated upstream
	void CAnimation::SetDuration(float _Value)
=======
	void Animation::SetDuration(float value)
>>>>>>> Stashed changes
	{
		for (size_t i = 0; i < mSpriteSheet.size(); i++)
		{	
			mSpriteSheet[i].duration = value;
		}
	}

	renderer::AnimationCB CAnimation::GetAnimationData()
	{
		renderer::AnimationCB data = {};
		
		data.type = (UINT)eAnimationType::SecondDimension;
		data.LT = mSpriteSheet[mIndex].LT;
		data.offset = mSpriteSheet[mIndex].offset;
		data.size = mSpriteSheet[mIndex].size;
		data.atlasSize = mSpriteSheet[mIndex].altasSize;

		return data;
	}

}

// �� k s if�� ����
// �� j �Լ�ã��
// �� mm �ڵ��� �ݰ� ����
// ��Ʈ�� RO �Լ� ��������
