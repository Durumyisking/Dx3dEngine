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
	virtual void FixedUpdate() override{};
	virtual void Render() override{};

	// 폴더에 애니메이션을 전부 로드하는 함수
	// 애니메이션 명은 파일명으로 생성
	void LoadAnimations(const std::wstring& folderPath);

	// 애니메이션을 생성하는 함수
	void CreateAnimation(const std::wstring& name, const std::wstring& path, double duration);
	void CreateAnimation(const std::wstring& name, const std::wstring& path, int frameCount = 45);

	// 애니메이션 Play
	void Play(const std::wstring& name, bool loop = true);

	// 애니메이션의 1 프레임 간격 세팅
	void SetAnimationDruationTime(const std::wstring& name, float duration = 0.1f);

	bool IsComplete() { return mPlayAnimation == nullptr ? true :mPlayAnimation->IsComplete(); }

	const std::wstring PlayAnimationName() const;

	AnimationClip* GetAnimationClip(const std::wstring& animationName) const;
private:
	std::map<std::wstring, AnimationClip*> mAnimationClips;
	AnimationClip* mPlayAnimation;

	bool mbLoop;
};

