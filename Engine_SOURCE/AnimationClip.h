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
	void SetBoneMatrix();
	void Reset();

	bool IsCompleate() { return mCompleate; }

	// 보간함수
	math::Vector3 Interpolation(math::Vector3& startVec,math::Vector3& endVec, float accTime, float endTime);

	aiMatrix4x4 ToLeftHandMatrix(math::Vector3 pos, math::Vector3 rotation);
	aiMatrix4x4 ToLeftHandMatrix(math::Vector3 pos, aiMatrix3x3 rotation);

public:
	GETSET(const std::wstring&, mName, AnimationName)
	GETSET(double, mDuration, Duration)
	GETSET(double, mTickPerSceond, TickPerSceond)
	GETSET(bool, mCompleate, Compleate)
	GETSET(BoneAnimator*, mAnimator, Animator)
private:
	const std::string parsingString(std::string& buf, const std::string& delValue, std::string::size_type& startPos) const;
	const std::string::size_type startStringPos(std::string& buf, const std::string& delValue) const;
	int getTransformation_Index(const std::string& buf, const std::string& delValue) const;

	const animation::NodeData readNodes(std::string& buf) const;
	const animation::SkeletonData readSkeleton(std::string& buf, int arrSize) const;

private:
	std::vector<animation::NodeData>     mNodeData;
	std::vector<animation::SkeletonData> mSkeletonData;

	double	    mDuration;
	double      mTickPerSceond;
	UINT	    mCurIndex;
	bool		mCompleate;

	BoneAnimator* mAnimator;

	std::wstring mName;
};

