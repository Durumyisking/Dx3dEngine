#pragma once
#include "Component.h"
#include "Animation.h"

namespace dru
{
	class CAnimator : public CComponent
	{
	public:
		struct Events
		{
			struct Event
			{
				void operator=(std::function<void()> func)
				{
					mEvent = std::move(func);
				}
				void operator()()
				{
					if (mEvent)
						mEvent();
					
				}
				std::function<void()> mEvent;
			};

			Event mStartEvent;
			Event mCompleteEvent; // �ִϸ��̼� �� ����Ŭ ���� �� ����
			Event mEndEvent; // �ִϸ��̼� ��ü�� interrupt �ɸ��� ȣ��

			std::vector<Event> mFrameEvents; // �ش� ��������Ʈ(�ε���) ���� ����� �̺�Ʈ
		};

		CAnimator();
		virtual ~CAnimator();

		virtual void Initialize();
		virtual void update();
		virtual void fixedUpdate();
		virtual void render();

<<<<<<< Updated upstream
		bool Create(const std::wstring& _name, std::shared_ptr<CTexture> _atlas, Vector2 _leftTop, Vector2 _size, Vector2 _offset,  UINT _spriteLength, Vector2 _Ratio, float _duration, bool _Reverse = false);
		
		CAnimation* FindAnimation(const std::wstring& _name);
		Events* FindEvents(const std::wstring& _name);
		CAnimation* GetCurrentAnimation() const { return mCurrentAnimation; }
=======
		bool Create(const std::wstring& name, Texture* atlas, Vector2 leftTop, Vector2 size, Vector2 offset,  UINT spriteLength, Vector2 ratio, float duration, bool reverse = false);
		
		Animation* FindAnimation(const std::wstring& name);
		Events* FindEvents(const std::wstring& name);
		Animation* GetCurrentAnimation() const { return mCurrentAnimation; }
>>>>>>> Stashed changes

		bool IsPlaying(const std::wstring& name) { if (mCurrentAnimation->GetAnimationName() == name) return true; return false; }

		void Play(std::wstring name, bool bLoop = true);
		void PauseOn() { mbPause = true; }
		void PauseOff() { mbPause = false; }
		bool IsPause() const { return mbPause; }

		void Binds();
		void BindSprite(renderer::AnimationCB sprite);
		void Reset();
		void Clear();

		std::function<void()>& GetStartEvent(const std::wstring& name);
		std::function<void()>& GetCompleteEvent(const std::wstring& name);
		std::function<void()>& GetEndEvent(const std::wstring& name);
		std::function<void()>& GetFrameEvent(const std::wstring& name, UINT idx);

	private:
		std::map<std::wstring, CAnimation*> mAnimations;
		std::map<std::wstring, Events*> mEvents;
		CAnimation* mCurrentAnimation;

		bool mbLoop;
		bool mbPause;
	};
}


