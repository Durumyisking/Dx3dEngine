#include "Animator.h"

namespace dru
{
	Animator::Animator()
		: Component(eComponentType::Animator)
		, mAnimations{}
		, mEvents{}
		, mCurrentAnimation(nullptr)
		, mbLoop(false)
		, mbPause(false)
	{
	}

	Animator::~Animator()
	{
		for (auto anim : mAnimations)
		{
			delete anim.second;
			anim.second = nullptr;
		}
		for (auto evnt : mEvents)
		{
			delete evnt.second;
			evnt.second = nullptr;
		}

	}

	void Animator::Initialize()
	{
	}

	void Animator::update()
	{
		if (!mCurrentAnimation)
			return;

		if (!mbPause)
		{
			Events* events = FindEvents(mCurrentAnimation->GetAnimationName());

			if (mCurrentAnimation->IsCompleted())
			{
				if (events)
				{
					if (!mCurrentAnimation->IsCompleteEventPlayed())
					{
						mCurrentAnimation->SetCompleteEventPlayed(true);
						events->mCompleteEvent();
					}
				}

				if (mbLoop)
					mCurrentAnimation->Reset();
			}
			UINT spriteIndex = mCurrentAnimation->update();

			if (nullptr != events)
			{
				if (spriteIndex != -1 && events->mFrameEvents[spriteIndex].mEvent)
				{
					events->mFrameEvents[spriteIndex].mEvent();
				}
			}
		}
	}

	void Animator::fixedUpdate()
	{
	}

	void Animator::render()
	{
	}


	bool Animator::Create(const std::wstring& name, Texture* atlas, Vector2 leftTop, Vector2 size, Vector2 offset, UINT spriteLength, Vector2 ratio, float duration, bool reverse)
	{
		if (!_atlas)
			return false;

		Animation* animation = FindAnimation(name);
		if (animation)
			return false;

		animation = new Animation();
		animation->Create(name, atlas, leftTop, size, offset, spriteLength, ratio, duration, reverse);

		mAnimations.insert(std::make_pair(name, animation));

		Events* events = new Events();
		events->mFrameEvents.resize(spriteLength);
		mEvents.insert(std::make_pair(name, events));

		return true;
	}


	Animation* Animator::FindAnimation(const std::wstring& name)
	{
		std::map<std::wstring, Animation*>::iterator iter = mAnimations.find(name);

		if (mAnimations.end() == iter)
		{
			return nullptr;
		}

		return iter->second;
	}


	Animator::Events* Animator::FindEvents(const std::wstring& name)
	{
		std::map<std::wstring, Events*>::iterator iter = mEvents.find(name);

		if (mEvents.end() == iter)
		{
			return nullptr;
		}

		return iter->second;
	}


	void Animator::Play(std::wstring name, bool bLoop)
	{
		Animation* prevAnimation = mCurrentAnimation;
		Events* events = nullptr;
		if (prevAnimation)
			events = FindEvents(mCurrentAnimation->GetAnimationName());

		if (events)
			events->mEndEvent();

		mCurrentAnimation = FindAnimation(name);
		mCurrentAnimation->Reset();
		mbLoop = bLoop;

		events = FindEvents(mCurrentAnimation->GetAnimationName());

		if (events)
			events->mStartEvent();
	}


	void Animator::Binds()
	{
		if (!mCurrentAnimation)
			return;

		mCurrentAnimation->BindShader();
	}

	void Animator::BindSprite(renderer::AnimationCB sprite)
	{
		if (!mCurrentAnimation)
			return;

		mCurrentAnimation->BindSpriteToShader(sprite);
	}

	void Animator::Reset()
	{
		mCurrentAnimation->Reset();
	}

	void Animator::Clear()
	{
		mCurrentAnimation->Clear();
	}

	std::function<void()>& Animator::GetStartEvent(const std::wstring& _name)
	{
		Events* events = FindEvents(_name);

		return events->mStartEvent.mEvent;
	}

	std::function<void()>& Animator::GetCompleteEvent(const std::wstring& _name)
	{
		Events* events = FindEvents(_name);

		return events->mCompleteEvent.mEvent;
	}

	std::function<void()>& Animator::GetEndEvent(const std::wstring& _name)
	{
		Events* events = FindEvents(_name);

		return events->mEndEvent.mEvent;
	}

	std::function<void()>& Animator::GetFrameEvent(const std::wstring& _name, UINT _idx)
	{
		Events* events = FindEvents(_name);

		return events->mFrameEvents[_idx].mEvent;
	}

}