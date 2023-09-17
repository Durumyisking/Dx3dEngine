#pragma once
#include "Struct.h"
#include "StringFunctions.h"
#include "Engine.h"


#include "..//External/assimp/include/assimp/Importer.hpp"
#include "..//External/assimp/include/assimp/cimport.h"
#include "..//External/assimp/include/assimp/postprocess.h"
#include "..//External/assimp/include/assimp/scene.h"

#pragma comment(lib, "..//External/assimp/lib/Debug/assimp-vc143-mtd.lib")

class BoneAnimator;
class AnimationClip
{
public:
	AnimationClip();
	~AnimationClip();

	void Update();
	void FixedUpdate(){};
	void Render(){};

	void CreateAnimation(const std::wstring& name ,const std::wstring& path, double duration);
	void CreateAnimation(const std::wstring& name, const std::wstring& path, int frameCount);
	void SetBoneMatrix(const animation::SkeletonData& inCurData, const animation::SkeletonData& inNextData, double drutation = 1.f/ 60.f);
	void Reset();

	void InterpolationPrveToCurAnimation();

	bool IsCompleate() { return mbComplete; }

	// 보간함수
	math::Vector3 Interpolation(math::Vector3& startVec,math::Vector3& endVec, float accTime, float endTime);

	aiMatrix4x4 ToLeftHandMatrix(math::Vector3 pos, math::Vector3 rotation);
	aiMatrix4x4 ToLeftHandMatrix(math::Vector3 pos, aiMatrix3x3 rotation);

public:
	std::function<void()> GetStartEvent() { return mStartEvent; }
	std::function<void()> GetCompleteEvent() { return mCompleteEvent; }
	std::function<void()> GetEndEvent() { return mEndEvent; }
	std::function<void()> GetKeyFrameEvent(UINT index);

	void SetStartEvent(std::function<void()> inEvent) { mStartEvent = std::move(inEvent); }
	void SetCompleteEvent(std::function<void()> inEvent) { mCompleteEvent = std::move(inEvent); }
	void SetEndEvent(std::function<void()> inEvent) { mEndEvent = std::move(inEvent); }
	void SetKeyFrameEvent(UINT index, std::function<void()> inEvent);

public:
	GETSET(const std::wstring&, mName, AnimationName)
	GETSET(double, mDuration, Duration)
	GETSET(double, mTickPerSceond, TickPerSceond)
	GETSET(bool, mbComplete, Complete)
	GETSET(BoneAnimator*, mAnimator, Animator)
	GETSET(const animation::SkeletonData*, mPreveAnimationData, PreveAnimationData)
	GETSET(float , mConnectionDuration, ConnectionDuration)
	GETSET(UINT, mCurIndex, CurIndex)

	const animation::SkeletonData* GetCurFrameAnimation(UINT index) const;
	void SetPreveAnimationData(const animation::SkeletonData* data, float connectionDuration = 1.0f);
private:
	const std::string parsingString(std::string& buf, const std::string& delValue, std::string::size_type& startPos) const;
	const std::string::size_type startStringPos(std::string& buf, const std::string& delValue) const;
	int getTransformation_Index(const std::string& buf, const std::string& delValue) const;

	const animation::NodeData readNodes(std::string& buf) const;
	const animation::SkeletonData readSkeleton(std::string& buf, int arrSize) const;

private:
	std::vector<animation::NodeData>     mNodeData;
	std::vector<animation::SkeletonData> mSkeletonData;

	const animation::SkeletonData* mPreveAnimationData;
	float mConnectionDuration;

	double	    mDuration;
	double      mTickPerSceond;
	UINT	    mCurIndex;
	bool		mbComplete;

	BoneAnimator* mAnimator;

	std::function<void()> mStartEvent;
	std::function<void()> mCompleteEvent;
	std::function<void()> mEndEvent;

	std::map<UINT, std::function<void()>> mKeyFrameEvent;

	std::wstring mName;
};

