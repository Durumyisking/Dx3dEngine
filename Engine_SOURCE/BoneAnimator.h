#pragma once
#include "Component.h"
#include "AnimationClip.h"
#include "Struct.h"

#include "..//External/assimp/include/assimp/Importer.hpp"
#include "..//External/assimp/include/assimp/cimport.h"
#include "..//External/assimp/include/assimp/postprocess.h"
#include "..//External/assimp/include/assimp/scene.h"

#pragma comment(lib, "..//External/assimp/lib/Debug/assimp-vc143-mtd.lib")

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
	void CreateAnimation(const std::wstring& name, const std::wstring& path, int frameCount = 60);

	// �ִϸ��̼� Play
	void Play(const std::wstring& name, bool loop = true);

	// �ִϸ��̼��� 1 ������ ���� ����
	void SetAnimationDruationTime(const std::wstring& name, float duration = 0.1f);
	bool IsComplete() { return mPlayAnimation == nullptr ? true :mPlayAnimation->IsComplete(); }
	const std::wstring PlayAnimationName() const;
	AnimationClip* GetAnimationClip(const std::wstring& animationName) const;
	AnimationClip* GetPlayAnimation() const { return mPlayAnimation; }

	const std::map<std::wstring, aiMatrix4x4>& GetFrameAnimationData() { return mFrameAnimationVector; }
	void ClearFrameAnimationData() { mFrameAnimationVector.clear(); }
	aiMatrix4x4 GetAnimationNodeTransform(const std::wstring& name) const ;
	void AddNodeTransform(const std::wstring& name, aiMatrix4x4 transform);
	void DeleteNodeTransform(const std::wstring& name);

	void FrameAnimationClear() { mFrameAnimationVector.clear(); }
public:
	GETSET(float, mIntervalAnimation, IntervalAnimation)
	GETSET(bool, mbLoop, Loop)
private:
	// �ִϸ��̼ǵ��� ������
	std::map<std::wstring, AnimationClip*> mAnimationClips;
	AnimationClip* mPlayAnimation;

	// 1 �������� �ִϸ��̼� �̵������� ������
	std::map<std::wstring, aiMatrix4x4> mFrameAnimationVector;
	
	float mIntervalAnimation;
	bool mbLoop;
};

