#include "BoneAnimator.h"

namespace fs = std::filesystem;
BoneAnimator::BoneAnimator()
	: Component(eComponentType::BoneAnimator)
	, mAnimationClips{}
	, mPlayAnimation(nullptr)
	, mbLoop(true)
{
}

BoneAnimator::~BoneAnimator()
{
	for (auto& iter : mAnimationClips)
	{
		if (iter.second == nullptr)
			continue;

		delete iter.second;
		iter.second = nullptr;
	}
}

void BoneAnimator::Initialize()
{
}

void BoneAnimator::Update()
{
	if (mPlayAnimation == nullptr)
		return;

	if (mPlayAnimation->IsComplete())
	{
		if (!mbLoop)
		{
			mPlayAnimation = nullptr;
			return;
		}

		mPlayAnimation->Reset();
		mPlayAnimation->SetCurIndex(0);
	}

	mPlayAnimation->Update();
}

void BoneAnimator::LoadAnimations(const std::wstring& path)
{
	std::wstring fullPath = L"..//" + path;
	fs::path folderPath = std::string(fullPath.begin(), fullPath.end());

	for (const auto& entry : fs::directory_iterator(folderPath)) {
		if (entry.is_regular_file())
		{
			wchar_t szExtension[256] = {};
			_wsplitpath_s(entry.path().c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExtension, 256); // ��ο��� Ȯ���ڸ� �̾ƿ��� �༮

			std::wstring extension(szExtension);

			int startPos = -1;
			int EndPos = -1;

			std::string sfileName = ConvertToString(entry.path().c_str());
			if (sfileName.find("Demo") != std::string::npos)
				continue;

			startPos = static_cast<int>(sfileName.find_last_of("\\"));
			EndPos = static_cast<int>(sfileName.find_last_of("."));

			startPos += 1;
			std::wstring wfileName = ConvertToW_String(sfileName.c_str());
			std::wstring fileName = wfileName.substr(startPos, EndPos - startPos);


			if (L".SMD" == extension || L".Smd" == extension || L".smd" == extension)
			{
				CreateAnimation(fileName, entry.path());
			}
		}
	}
}

void BoneAnimator::CreateAnimation(const std::wstring& name, const std::wstring& path, double duration)
{
	// �ߺ��� �ִϸ��̼� �̸�
	if (mAnimationClips.find(name) != mAnimationClips.end())
		return;

	AnimationClip* clip = new AnimationClip();
	mAnimationClips.insert(std::pair<std::wstring, AnimationClip*>(name, clip));

	clip->SetAnimationName(name);
	clip->SetAnimator(this);
	clip->CreateAnimation(name, path, duration);
}

void BoneAnimator::CreateAnimation(const std::wstring& name, const std::wstring& path, int frameCount)
{
	// �ߺ��� �ִϸ��̼� �̸�
	if (mAnimationClips.find(name) != mAnimationClips.end())
		return;

	AnimationClip* clip = new AnimationClip();
	mAnimationClips.insert(std::pair<std::wstring, AnimationClip*>(name, clip));

	clip->SetAnimator(this);
	clip->CreateAnimation(name, path, frameCount);
}

void BoneAnimator::Play(const std::wstring& name, bool loop)
{
	std::map<std::wstring, AnimationClip*>::iterator iter;
	iter = mAnimationClips.find(name);
	if (iter == mAnimationClips.end())
		return;
	
	mbLoop = loop;

	// EndEventȣ��
	if (nullptr != mPlayAnimation && nullptr != mPlayAnimation->GetEndEvent())
		mPlayAnimation->GetEndEvent()();

	//if (nullptr != mPlayAnimation)
	//{
	//	// ���� �ִϸ��̼��� ������ ������ ����
	//	UINT frame = mPlayAnimation->GetCurIndex();
	//	const animation::SkeletonData* preveAnimation = mPlayAnimation->GetCurFrameAnimation(frame);

	//	// ���� �ִϸ��̼��� ���� �ִϸ��̼� ���� ����
	//	iter->second->SetPreveAnimationData(preveAnimation, 0.1f);
	//}

	// Next Animation
	mPlayAnimation = iter->second;
	
	// �ִϸ��̼� �ε��� �ʱ�ȭ
	mPlayAnimation->SetCurIndex(0);
	mPlayAnimation->Reset();

	// StartEventȣ��
	if (nullptr != mPlayAnimation && nullptr != mPlayAnimation->GetStartEvent())
		mPlayAnimation->GetStartEvent()();
}

void BoneAnimator::SetAnimationDruationTime(const std::wstring& name, float duration)
{
	std::map<std::wstring, AnimationClip*>::iterator iter;
	iter = mAnimationClips.find(name);
	if (iter == mAnimationClips.end())
		return;

	iter->second->SetDuration(duration);
}

const std::wstring BoneAnimator::PlayAnimationName() const
{
	if (mPlayAnimation == nullptr)
		return L"Empty";

	return mPlayAnimation->GetAnimationName();
}

AnimationClip* BoneAnimator::GetAnimationClip(const std::wstring& animationName) const
{
	const auto& iter = mAnimationClips.find(animationName);
	if (iter == mAnimationClips.end())
		return nullptr;

	return iter->second;
}
