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

	// ������ �ִϸ��̼��� ���� �ε��ϴ� �Լ�
	// �ִϸ��̼� ���� ���ϸ����� ����
	void LoadAnimations(const std::wstring& folderPath);

	// �ִϸ��̼��� �����ϴ� �Լ�
	void CreateAnimation(const std::wstring& name, const std::wstring& path, double duration);
	void CreateAnimation(const std::wstring& name, const std::wstring& path, int frameCount = 45);

	// �ִϸ��̼� Play
	void Play(const std::wstring& name, bool loop = true);

	// �ִϸ��̼��� 1 ������ ���� ����
	void SetAnimationDruationTime(const std::wstring& name, float duration = 0.1f);

	bool IsComplete() { return mPlayAnimation == nullptr ? true :mPlayAnimation->IsComplete(); }

	const std::wstring PlayAnimationName() const;

	AnimationClip* GetAnimationClip(const std::wstring& animationName) const;
private:
	std::map<std::wstring, AnimationClip*> mAnimationClips;
	AnimationClip* mPlayAnimation;

	bool mbLoop;
};

