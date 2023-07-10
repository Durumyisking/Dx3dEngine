#include "Animation.h"
#include "TimeMgr.h"


namespace dru
{
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

	void Animation::Create(const std::wstring& _name, Texture* _atlas, Vector2 _leftTop, Vector2 _size, Vector2 _offset, UINT _spriteLength, Vector2 _Ratio, float _duration, bool _Reverse)
	{
		mAnimationName = _name;
		mAtlas = _atlas;
		mbReversePlay = _Reverse;
		mSpriteLength = _spriteLength;

		float width = (float)_atlas->GetWidth();
		float height = (float)_atlas->GetHeight();

		size_t CollSkipCount = 0;
		for (size_t i = 0; i < _spriteLength; i++)
		{
			Sprite sprite = {};

			// ��������Ʈ�� LT�� + ����� width�� �Ѿ�� �����ٷ� �ٲ۴�.
			if (_leftTop.x + (_size.x * (float)i) >= width)
			{
				_leftTop.x = 0.f;
				_leftTop.y += _size.y;

				CollSkipCount = i;
			}

			// uv��ǥ�� �ѱ�� ���� width, height�� ������.
			sprite.LT = Vector2(
				(_leftTop.x + (_size.x * (float)(i - CollSkipCount))) / width,
				_leftTop.y / height
			);

			sprite.size = Vector2(fabs(_size.x) / width, fabs(_size.y) / height);
			sprite.offset = Vector2(_offset.x / width, _offset.y / height);
			sprite.duration = _duration;
			sprite.altasSize = Vector2(_Ratio.x / width, _Ratio.y / height);

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
		// Texture Clear
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

	void Animation::SetDuration(float _Value)
	{
		for (size_t i = 0; i < mSpriteSheet.size(); i++)
		{	
			mSpriteSheet[i].duration = _Value;
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

}

// �� k s if�� ����
// �� j �Լ�ã��
// �� mm �ڵ��� �ݰ� ����
// ��Ʈ�� RO �Լ� ��������
