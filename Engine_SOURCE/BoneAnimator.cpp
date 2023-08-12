#include "BoneAnimator.h"


BoneAnimator::BoneAnimator()
	: Component(eComponentType::BoneAnimator)
	, mAnimationClips{}
	, mPlayAnimation(nullptr)
{
}

BoneAnimator::~BoneAnimator()
{
	for (auto& iter : mAnimationClips)
	{
		if (iter.second == nullptr)
			continue;

		delete iter.second;
		iter.second = nullptr;
	}
}

void BoneAnimator::Initialize()
{
}

void BoneAnimator::Update()
{
	if (mPlayAnimation == nullptr)
		return;

	if (mPlayAnimation->IsCompleate())
	{
		if (!mbLoop)
			mPlayAnimation = nullptr;
		else
			mPlayAnimation->Reset();

		return;
	}

	mPlayAnimation->Update();
}

void BoneAnimator::CreateAnimation(const std::wstring& name, const std::wstring& path, float duration)
{
	// 중복된 애니메이션 이름
	if (mAnimationClips.find(name) != mAnimationClips.end())
		return;

	AnimationClip* clip = new AnimationClip();
	mAnimationClips.insert(std::pair<std::wstring, AnimationClip*>(name, clip));

	clip->CreateAnimation(name, path, duration);
	clip->SetAnimator(this);
}

void BoneAnimator::Play(const std::wstring& name, bool loop)
{
	std::map<std::wstring, AnimationClip*>::iterator iter;
	iter = mAnimationClips.find(name);
	if (iter == mAnimationClips.end())
		return;
	
	mbLoop = loop;

	mPlayAnimation = iter->second;
	mPlayAnimation->Reset();
}
