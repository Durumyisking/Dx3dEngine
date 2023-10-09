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

	// 폴더에 애니메이션을 전부 로드하는 함수
	// 애니메이션 명은 파일명으로 생성
	void LoadAnimations(const std::wstring& folderPath);

	// 애니메이션을 생성하는 함수
	void CreateAnimation(const std::wstring& name, const std::wstring& path, double duration);
	void CreateAnimation(const std::wstring& name, const std::wstring& path, int frameCount = 60);

	// 애니메이션 Play
	void Play(const std::wstring& name, bool loop = true);

	// 애니메이션의 1 프레임 간격 세팅
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
	// 애니메이션들을 담을곳
	std::map<std::wstring, AnimationClip*> mAnimationClips;
	AnimationClip* mPlayAnimation;

	// 1 프레임의 애니메이션 이동정보를 담을곳
	std::map<std::wstring, aiMatrix4x4> mFrameAnimationVector;
	
	float mIntervalAnimation;
	bool mbLoop;
};

