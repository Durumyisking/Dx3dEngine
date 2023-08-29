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
	: mDuration(0.0f)
	, mTickPerSceond(0.0f)
	, mCurIndex(0)
	, mCompleate(false)
	, mAnimator(nullptr)
	, mName(L"")
{
}

AnimationClip::~AnimationClip()
{
}

void AnimationClip::Update()
{
	if (mCompleate)
		return;

	mTickPerSceond += DT;

	if (mTickPerSceond >= mDuration)
	{
		mTickPerSceond -= mDuration;
		mCurIndex++;
	}

	if (mCurIndex >= mSkeletonData.size() - 1)
	{
		// 애니메이션 종료시 리셋
		Reset();
		mCompleate = true;
		return;
	}

	SetBoneMatrix();
}

void AnimationClip::CreateAnimation(const std::wstring& name, const std::wstring& path, double duration)
{
	// 파일 경로 설정
	std::ifstream file(path, std::ios::in);

	std::string buf = "";
	// 파일이 열리지 않으면 Window 종료
	if (!file.is_open())
	{
		std::cout << "파일이 없습니다" << std::endl;
		DestroyWindow(application.GetHwnd());
	}

	// 애니메이션 이름 설정
	SetAnimationName(name);
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
	int frame = 0;
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
			mSkeletonData.emplace_back(animation::SkeletonData{});
			frame = mSkeletonData.size() - 1;
			mSkeletonData[frame].Translation.resize(mNodeData.size());
			mSkeletonData[frame].Rotation.resize(mNodeData.size());

			GameObj* owner = mAnimator->GetOwner();
			BaseRenderer* baseRenderer = mAnimator->GetOwner()->GetComponent<BaseRenderer>();
			Model* model = mAnimator->GetOwner()->GetComponent<BaseRenderer>()->GetModel();

			for (int i = 0; i < mNodeData.size(); ++i)
			{
				ModelNode* node = model->FindNode(mNodeData[i].Name);

				aiMatrix4x4 transform = node->mTransformation;
				aiVector3t pos = aiVector3t<float>();
				aiQuaternion rotation = aiQuaternion();

				transform.DecomposeNoScaling(rotation, pos);

				mSkeletonData[frame].Translation[i] = std::pair(i, Vector3(pos.x,pos.y,pos.z));
				mSkeletonData[frame].Rotation[i] = std::pair(i, Vector3(rotation.x, rotation.y, rotation.z));
			}


			continue;
		}

		animation::SkeletonData data = {};
		data = readSkeleton(buf, mNodeData.size());
		data.Time = static_cast<float>(frame);

		mSkeletonData[frame].Time = frame;

		mSkeletonData[frame].Translation[data.Translation[0].first].second = data.Translation[0].second;
		mSkeletonData[frame].Rotation[data.Rotation[0].first].second = data.Rotation[0].second;
	}

	file.close();
}

void AnimationClip::SetBoneMatrix()
{
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

	animation::SkeletonData curData = mSkeletonData[mCurIndex];
	animation::SkeletonData nextData = mSkeletonData[mCurIndex + 1];

	int size = curData.Translation.size();
	for (int i = 0; i < size; ++i)
	{
		ModelNode* node = model->FindNode(mNodeData[curData.Translation[i].first].Name);
		if (node == nullptr)
			continue;

		// T
		Vector3 positionVec = Interpolation(curData.Translation[i].second, nextData.Translation[i].second, mTickPerSceond, mDuration);

		// R
		Vector3 eRotation = curData.Rotation[i].second;
		aiQuaternion qRotation(-eRotation.y, eRotation.z, -eRotation.x);

		Vector3 eNextRotation = nextData.Rotation[i].second;
		aiQuaternion qNextRotation(-eNextRotation.y, eNextRotation.z, -eNextRotation.x);

		aiQuaternion result = {};
		result.Interpolate(result, qRotation, qNextRotation, mTickPerSceond / mDuration);

		// T * R
		node->SetTransformation(ToLeftHandMatrix(positionVec, result.GetMatrix()));
	}
}

void AnimationClip::Reset()
{
	mCompleate = false;
	mCurIndex = 0;
	mTickPerSceond = 0.0f;
}

math::Vector3 AnimationClip::Interpolation(math::Vector3& startVec, math::Vector3& endVec, float accTime, float endTime)
{
	float x = ((startVec.x * (endTime - accTime)) + (endVec.x * accTime)) / endTime;
	float y = ((startVec.y * (endTime - accTime)) + (endVec.y * accTime)) / endTime;
	float z = ((startVec.z * (endTime - accTime)) + (endVec.z * accTime)) / endTime;
	return Vector3(x, y, z);
}

aiMatrix4x4 AnimationClip::ToLeftHandMatrix(math::Vector3 pos, math::Vector3 rotation)
{
	// 이동 계산
	aiMatrix4x4 traslation = {};
	traslation.Translation(aiVector3D(pos.x, pos.y, -pos.z), traslation);

	// 회전 계산 오일러
	aiMatrix4x4 rotationmatrix = {};

	// 아마? 닌텐도 만에 좌표계가 있는거같다
	// x 오른손좌표계기준, 반시계
	// y 오른손좌표계기준, 반시계
	// z 포지션은 오른손좌표계기준, 회전은 시계방향
	rotationmatrix.FromEulerAnglesXYZ(-rotation.x, -rotation.y, rotation.z);

	return traslation * rotationmatrix;
}

aiMatrix4x4 AnimationClip::ToLeftHandMatrix(math::Vector3 pos, aiMatrix3x3 rotation)
{
	// 이동 계산
	aiMatrix4x4 traslation = {};
	traslation.Translation(aiVector3D(pos.x, pos.y, -pos.z), traslation);

	// 회전 계산 오일러
	aiMatrix4x4 rotationmatrix(rotation);

	// 아마? 닌텐도 만에 좌표계가 있는거같다
	// x 오른손좌표계기준, 반시계
	// y 오른손좌표계기준, 반시계
	// z 포지션은 오른손좌표계기준, 회전은 시계방향
	//rotationmatrix.FromEulerAnglesXYZ(-rotation.x, -rotation.y, rotation.z);

	return traslation * rotationmatrix;
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

int AnimationClip::getTransformation_Index(const std::string& buf, const std::string& delValue) const
{
	std::string::size_type pos = 0;
	pos = buf.find_first_of(delValue, 0);

	std::string temp = buf.substr(0, pos);

	return std::stoi(temp);
}

const animation::NodeData AnimationClip::readNodes(std::string& buf) const
{
	animation::NodeData Data = {};
	std::string str = "";
	std::string::size_type fPos = buf.find_first_of('"', 1);
	std::string::size_type lPos = buf.find_last_of('"', buf.size() - 1);
	fPos += 1;

	str = buf.substr(fPos, lPos - fPos);
	Data.Name = ConvertToW_String(str.c_str());


	fPos = buf.find_last_of(' ', buf.size() - 1);
	str = buf.substr(fPos, buf.size() - 1);
	Data.ParentsIDX = std::atoi(str.c_str());

	return Data;
}

const animation::SkeletonData AnimationClip::readSkeleton(std::string& buf, int arrSize) const
{
	animation::SkeletonData data = {};

	int index = getTransformation_Index(buf, " ");

	std::string str = "";
	std::string::size_type pos = 0;
	pos = startStringPos(buf, " ");

	std::vector<float> temp = {};
	for (int i = 0; i < 3; ++i)
	{
		str = parsingString(buf, " ", pos);
		temp.emplace_back((std::stof(str.c_str())));
	}

	data.Translation.emplace_back(index ,math::Vector3(temp[0], temp[1], temp[2]));
	temp.clear();

	for (int i = 0; i < 3; ++i)
	{
		str = parsingString(buf, " ", pos);
		temp.emplace_back(std::stof(str.c_str()));
	}
	data.Rotation.emplace_back(index, math::Vector3(temp[0], temp[1], temp[2]));

	return data;
}