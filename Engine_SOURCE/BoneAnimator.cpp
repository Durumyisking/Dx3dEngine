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

	if (mPlayAnimation->IsCompleate())
	{
		if (!mbLoop)
		{
			mPlayAnimation = nullptr;
			return;
		}

		mPlayAnimation->Reset();
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
			_wsplitpath_s(entry.path().c_str(), nullptr, 0, nullptr, 0, nullptr, 0, szExtension, 256); // 경로에서 확장자만 뽑아오는 녀석

			std::wstring extension(szExtension);

			int startPos = -1;
			int EndPos = -1;

			std::string sfileName = ConvertToString(entry.path().c_str());
			if (sfileName.find("Demo") != std::string::npos)
				continue;

			startPos = sfileName.find_last_of("\\");
			EndPos = sfileName.find_last_of(".");

			startPos += 1;
			std::wstring wfileName = ConvertToW_String(sfileName.c_str());
			std::wstring fileName = wfileName.substr(startPos, EndPos - startPos);


			if (L".SMD" == extension || L".Smd" == extension || L".smd" == extension)
			{
				CreateAnimation(fileName, entry.path(), 0.1f);
			}
		}
	}
}

void BoneAnimator::CreateAnimation(const std::wstring& name, const std::wstring& path, double duration)
{
	// 중복된 애니메이션 이름
	if (mAnimationClips.find(name) != mAnimationClips.end())
		return;

	AnimationClip* clip = new AnimationClip();
	mAnimationClips.insert(std::pair<std::wstring, AnimationClip*>(name, clip));

	clip->CreateAnimation(name, path, duration);
	clip->SetAnimator(this);
}

void BoneAnimator::Play(const std::wstring& name, bool loop)
{
	std::map<std::wstring, AnimationClip*>::iterator iter;
	iter = mAnimationClips.find(name);
	if (iter == mAnimationClips.end())
		return;
	
	mbLoop = loop;

	mPlayAnimation = iter->second;
	mPlayAnimation->Reset();
}

void BoneAnimator::SetAnimationDruationTime(const std::wstring& name, float duration)
{
	std::map<std::wstring, AnimationClip*>::iterator iter;
	iter = mAnimationClips.find(name);
	if (iter == mAnimationClips.end())
		return;

	iter->second->SetDuration(duration);
}
