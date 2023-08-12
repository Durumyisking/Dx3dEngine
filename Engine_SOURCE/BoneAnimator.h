#pragma once
#include "Component.h"
#include "AnimationClip.h"
#include "Struct.h"

class BoneAnimator : public Component
{
public:
	BoneAnimator();
	~BoneAnimator();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	void CreateAnimation(const std::wstring& name, const std::wstring& path, float duration);

	void Play(const std::wstring& name);
private:
	std::map<std::wstring, AnimationClip> mAnimationClips;
};

