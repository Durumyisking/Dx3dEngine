#include "BoneAnimator.h"


BoneAnimator::BoneAnimator()
	: Component(eComponentType::BoneAnimator)
	, mAnimationClips{}
{
}

BoneAnimator::~BoneAnimator()
{
}

void BoneAnimator::Initialize()
{
}

void BoneAnimator::Update()
{
}

void BoneAnimator::FixedUpdate()
{
}

void BoneAnimator::Render()
{
}

void BoneAnimator::CreateAnimation(const std::wstring& name, const std::wstring& path, float duration)
{


}

void BoneAnimator::Play(const std::wstring& name)
{
}
