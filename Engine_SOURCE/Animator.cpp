#include "Animator.h"

namespace dru
{
	CAnimator::CAnimator()
		: CComponent(eComponentType::Animator)
		, mAnimations{}
		, mEvents{}
		, mCurrentAnimation(nullptr)
		, mbLoop(false)
		, mbPause(false)
	{
	}

	CAnimator::~CAnimator()
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

	void CAnimator::Initialize()
	{
	}

	void CAnimator::update()
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

	void CAnimator::fixedUpdate()
	{
	}

	void CAnimator::render()
	{
	}

<<<<<<< Updated upstream
	bool CAnimator::Create(const std::wstring& _name, std::shared_ptr<CTexture> _atlas, Vector2 _leftTop, Vector2 _size, Vector2 _offset, UINT _spriteLength, Vector2 _Ratio, float _duration, bool _Reverse)
=======
	bool Animator::Create(const std::wstring& name, Texture* atlas, Vector2 leftTop, Vector2 size, Vector2 offset, UINT spriteLength, Vector2 ratio, float duration, bool reverse)
>>>>>>> Stashed changes
	{
		if (!atlas)
			return false;

<<<<<<< Updated upstream
		CAnimation* animation = FindAnimation(_name);
		if (animation)
			return false;

		animation = new CAnimation();
		animation->Create(_name, _atlas, _leftTop, _size, _offset, _spriteLength, _Ratio, _duration, _Reverse);
=======
		Animation* animation = FindAnimation(name);
		if (animation)
			return false;

		animation = new Animation();
		animation->Create(name, atlas, leftTop, size, offset, spriteLength, ratio, duration, reverse);
>>>>>>> Stashed changes

		mAnimations.insert(std::make_pair(name, animation));

		Events* events = new Events();
		events->mFrameEvents.resize(spriteLength);
		mEvents.insert(std::make_pair(name, events));

		return true;
	}

<<<<<<< Updated upstream
	CAnimation* CAnimator::FindAnimation(const std::wstring& _name)
	{
		std::map<std::wstring, CAnimation*>::iterator iter = mAnimations.find(_name);
=======
	Animation* Animator::FindAnimation(const std::wstring& name)
	{
		std::map<std::wstring, Animation*>::iterator iter = mAnimations.find(name);
>>>>>>> Stashed changes

		if (mAnimations.end() == iter)
		{
			return nullptr;
		}

		return iter->second;
	}

<<<<<<< Updated upstream
	CAnimator::Events* CAnimator::FindEvents(const std::wstring& _name)
=======
	Animator::Events* Animator::FindEvents(const std::wstring& name)
>>>>>>> Stashed changes
	{
		std::map<std::wstring, Events*>::iterator iter = mEvents.find(name);

		if (mEvents.end() == iter)
		{
			return nullptr;
		}

		return iter->second;
	}

<<<<<<< Updated upstream
	void CAnimator::Play(std::wstring _name, bool _bLoop)
=======
	void Animator::Play(std::wstring name, bool bLoop)
>>>>>>> Stashed changes
	{
		CAnimation* prevAnimation = mCurrentAnimation;
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


	void CAnimator::Binds()
	{
		if (!mCurrentAnimation)
			return;

		mCurrentAnimation->BindShader();
	}

<<<<<<< Updated upstream
	void CAnimator::BindSprite(renderer::AnimationCB _Sprite)
=======
	void Animator::BindSprite(renderer::AnimationCB sprite)
>>>>>>> Stashed changes
	{
		if (!mCurrentAnimation)
			return;

		mCurrentAnimation->BindSpriteToShader(sprite);
	}

	void CAnimator::Reset()
	{
		mCurrentAnimation->Reset();
	}

	void CAnimator::Clear()
	{
		mCurrentAnimation->Clear();
	}

	std::function<void()>& CAnimator::GetStartEvent(const std::wstring& _name)
	{
		Events* events = FindEvents(_name);

		return events->mStartEvent.mEvent;
	}

	std::function<void()>& CAnimator::GetCompleteEvent(const std::wstring& _name)
	{
		Events* events = FindEvents(_name);

		return events->mCompleteEvent.mEvent;
	}

	std::function<void()>& CAnimator::GetEndEvent(const std::wstring& _name)
	{
		Events* events = FindEvents(_name);

		return events->mEndEvent.mEvent;
	}

	std::function<void()>& CAnimator::GetFrameEvent(const std::wstring& _name, UINT _idx)
	{
		Events* events = FindEvents(_name);

		return events->mFrameEvents[_idx].mEvent;
	}

}