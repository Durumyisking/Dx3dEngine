#pragma once
#include "Entity.h"
#include "Graphics.h"
#include "Texture.h"
#include "Renderer.h"


using namespace dru::math;
using namespace dru::graphics;

namespace dru
{

	class CAnimation : public CEntity
	{
	public:
		struct Sprite
		{
			Vector2 LT;
			Vector2 size;	// �߶� �ȼ� ũ��
			Vector2 offset;
			Vector2 altasSize;
			float duration; // �̹��� �����Ӵ� �ð� ����

			Sprite()
				: LT(Vector2::Zero)
				, size(Vector2::Zero)
				, offset(Vector2::Zero)
				, altasSize(Vector2::Zero)
				, duration(0.1f)
			{

			}
		};

		CAnimation();
		~CAnimation();
		
		UINT update();
		void fixedUpdate();
		void render();

<<<<<<< Updated upstream
		void Create(const std::wstring& _name, std::shared_ptr<CTexture> _atlas, Vector2 _leftTop, Vector2 _size, Vector2 _offset, UINT _spriteLength, Vector2 _Ratio, float _duration, bool _Reverse);
=======
		void Create(const std::wstring& name, Texture* atlas, Vector2 leftTop, Vector2 size, Vector2 offset, UINT spriteLength, Vector2 ratio, float duration, bool reverse);
>>>>>>> Stashed changes
		void BindShader();
		void BindSpriteToShader(renderer::AnimationCB sprite);
		void Clear();
		void Reset();
		void SetDuration(float value);
		 
		bool IsCompleted() const { return mbComplete; }
		bool IsCompleteEventPlayed() const { return mbCompleteEventPlayed; }
		void SetCompleteEventPlayed(bool bFlag) { mbCompleteEventPlayed = bFlag; }

		int GetIndex() const { return mIndex; }

		std::wstring& GetAnimationName() { return mAnimationName; }

		renderer::AnimationCB GetAnimationData();

	private:
		class CAnimator* mAnimator;
		std::wstring mAnimationName;

		std::shared_ptr<CTexture> mAtlas;
		std::vector<Sprite> mSpriteSheet;
		int mIndex;
		float mTime;
		bool mbComplete;
		bool mbCompleteEventPlayed;
		bool mbReversePlay;
		UINT mSpriteLength;

	};
}

