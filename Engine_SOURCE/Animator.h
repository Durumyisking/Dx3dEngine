#pragma once
#include "Component.h"
#include "Animation.h"

namespace dru
{
	class Animator : public Component
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

		Animator();
		virtual ~Animator();

		virtual void Initialize() final;
		virtual void Update() final;
		virtual void FixedUpdate() final;
		virtual void Render() final;

		bool Create(const std::wstring& name, Texture* atlas, Vector2 leftTop, Vector2 size, Vector2 offset,  UINT spriteLength, Vector2 ratio, float duration, bool reverse = false);
		
		Animation* FindAnimation(const std::wstring& name);
		Events* FindEvents(const std::wstring& name);
		Animation* GetCurrentAnimation() const { return mCurrentAnimation; }

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
		std::map<std::wstring, Animation*> mAnimations;
		std::map<std::wstring, Events*> mEvents;
		Animation* mCurrentAnimation;

		bool mbLoop;
		bool mbPause;
	};
}


