#pragma once
#include "Struct.h"
#include "Engine.h"

class BoneAnimator;
class AnimationClip
{
public:
	AnimationClip();
	~AnimationClip();

	void Update();
	void FixedUpdate();
	void Render();


	void CreateAnimation(const std::wstring& name ,const std::wstring& path, float duration);


public:
	GETSET(const std::wstring&, mName, Name)
	GETSET(float, mDuration, Duration)
	GETSET(float, mDeltaTime, DeltaTime)
	GETSET(bool, mCompleate, Compleate)
	GETSET(BoneAnimator*, mAnimator, Animator)
private:
	const std::string parsingString(std::string& buf, const std::string& delValue, std::string::size_type& startPos) const;
	const std::string::size_type startStringPos(std::string& buf, const std::string& delValue) const;

	const animation::NodeData readNodes(std::string& buf) const;
	const animation::SkeletonData readSkeleton(std::string& buf) const;

private:
	std::vector<animation::NodeData>     mNodeData;
	std::vector<animation::SkeletonData> mSkeletonData;

	float	    mDuration;
	float       mDeltaTime;
	UINT	    mCurIndex;
	bool		mCompleate;

	BoneAnimator* mAnimator;

	std::wstring mName;
};

