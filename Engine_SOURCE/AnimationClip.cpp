#include "AnimationClip.h"
#include "Application.h"
#include "BoneAnimator.h"
#include "Model.h"
#include "GameObj.h"
#include "BaseRenderer.h"


#include "TimeMgr.h"

extern Application application;
namespace fs = std::filesystem;
AnimationClip::AnimationClip()
{
}

AnimationClip::~AnimationClip()
{
}

void AnimationClip::Update()
{
	if (mCompleate)
		return;

	mDeltaTime += DT;

	if (mDeltaTime >= mDuration)
	{
		mDeltaTime -= mDuration;
		mCurIndex++;

		if (mCurIndex >= mSkeletonData.size() - 1)
		{
			// 애니메이션 종료시 리셋
			mCompleate = true;
			mCurIndex = 0;
			mDeltaTime = 0.0f;

			return;
		}

		// 본 정보들을 변경.
		if (!mAnimator)
			return;

		GameObj* owner = mAnimator->GetOwner();
		if (!owner)
			return;

		BaseRenderer* baseRenderer = mAnimator->GetOwner()->GetComponent<BaseRenderer>();
		if (!baseRenderer)
			return;
		
		Model* model = mAnimator->GetOwner()->GetComponent<BaseRenderer>()->GetModel();
		if (!model)
			return;

		UINT curFrameIDX = mCurIndex;
		UINT nextFrameIDX = mCurIndex++;

		animation::SkeletonData curDT = mSkeletonData[curFrameIDX];
		animation::SkeletonData nextDT = mSkeletonData[nextFrameIDX];

	}
}

void AnimationClip::FixedUpdate()
{
}

void AnimationClip::Render()
{
}

void AnimationClip::CreateAnimation(const std::wstring& name, const std::wstring& path, float duration)
{
	// 파일 경로 설정
	std::ifstream file(L"..//" + path, std::ios::in);

	std::string buf = "";
	// 파일이 열리지 않으면 Window 종료
	if (!file.is_open())
	{
		std::cout << "파일이 없습니다" << std::endl;
		DestroyWindow(application.GetHwnd());
	}

	// 애니메이션 이름 설정
	SetName(name);
	SetDuration(duration);

	// Nodes
	while (1)
	{
		getline(file, buf);
		if ((buf.find("nodes") != std::string::npos)
			|| (buf.find("version") != std::string::npos))
		{
			continue;;
		}

		if (buf.find("end") != std::string::npos)
		{
			break;
		}

		animation::NodeData data = {};
		data = readNodes(buf);
		mNodeData.emplace_back(data);
	}

	// Skeleton
	while (1)
	{
		getline(file, buf);
		if (buf.find("skeleton") != std::string::npos)
		{
			continue;
		}
		else if (buf.find("end") != std::string::npos)
		{
			break;
		}
		else if (buf.find("time") != std::string::npos)
		{
			continue;
		}

		animation::SkeletonData data = {};
		data = readSkeleton(buf);
		mSkeletonData.emplace_back(data);
	}

	file.close();
}

const std::string AnimationClip::parsingString(std::string& buf, const std::string& delValue, std::string::size_type& startPos) const
{
	std::string::size_type fPos = buf.find_first_of(delValue, startPos);
	std::string::size_type lPos = buf.find_first_of(delValue, fPos + 1);
	if (fPos >= buf.size())
	{
		return std::string("fail");
	}

	startPos = fPos + 1;

	std::string str = buf.substr((fPos + 1), lPos - (fPos + 1));

	return str;
}

const std::string::size_type AnimationClip::startStringPos(std::string& buf, const std::string& delValue) const
{
	std::string::size_type pos = 0;
	pos = buf.find_first_not_of(delValue, 0);

	return pos;
}

const animation::NodeData AnimationClip::readNodes(std::string& buf) const
{
	animation::NodeData Data = {};
	std::string str = "";
	std::string::size_type fPos = buf.find_first_of('"', 1);
	std::string::size_type lPos = buf.find_last_of('"', buf.size() - 1);
	fPos += 1;

	str = buf.substr(fPos, lPos - fPos);
	Data.Name = str;


	fPos = buf.find_last_of(' ', buf.size() - 1);
	str = buf.substr(fPos, buf.size() - 1);
	Data.ParentsIDX = std::atoi(str.c_str());

	return Data;
}

const animation::SkeletonData AnimationClip::readSkeleton(std::string& buf) const
{
	animation::SkeletonData data = {};
	std::string str = "";
	std::string::size_type pos = 0;
	pos = startStringPos(buf, " ");

	std::vector<float> temp = {};
	for (int i = 0; i < 3; ++i)
	{
		str = parsingString(buf, " ", pos);
		temp.emplace_back((std::stof(str.c_str())));
	}

	data.Translation = math::Vector3(temp[0], temp[1], temp[2]);
	temp.clear();

	for (int i = 0; i < 3; ++i)
	{
		str = parsingString(buf, " ", pos);
		temp.emplace_back(std::stof(str.c_str()));
	}

	data.Rotaion = math::Vector3(temp[0], temp[1], temp[2]);

	return data;
}