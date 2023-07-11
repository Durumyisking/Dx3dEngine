#include "AudioSource.h"
#include "Transform.h"
#include "GameObj.h"
#include "AudioClip.h"
#include "TimeMgr.h"
#include "ResourceMgr.h"

namespace dru
{
	AudioSource::AudioSource()
		: Component(eComponentType::AudioSource)
		, mAudioClips{}
	{
	}

	AudioSource::~AudioSource()
	{
	}

	void AudioSource::Initialize()
	{
	}

	void AudioSource::Update()
	{
	}

	void AudioSource::FixedUpdate()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetPosition();
		pos.z = 0.f;
		Vector3 forward = tr->Forward();
		for (std::pair<const std::wstring, AudioClip*> clip : mAudioClips)
		{
			AudioClip* audioClip = clip.second;
			if (audioClip->IsPlaying())
			{
				if (GETSINGLE(TimeMgr)->IsPlayerBulletTimeOn())
				{
					audioClip->SetPitch(0.333f);
				}
				else
				{
					audioClip->SetPitch(1.f);
				}
				audioClip->Set3DAttributes(pos, forward);
			}
		}
	}

	void AudioSource::Render()
	{
	}

	void AudioSource::Play(const std::wstring& key, bool loop)
	{
		GetClip(key)->SetLoop(loop);
		GetClip(key)->Play();
	}

	void AudioSource::Play_NoInterrupt(const std::wstring& key, bool loop)
	{
		if (!GetClip(key)->IsPlaying())
		{
			GetClip(key)->SetLoop(loop);
			GetClip(key)->Play();
		}
	}

	void AudioSource::Stop(const std::wstring& key)
	{
		GetClip(key)->Stop();
	}

	void AudioSource::SetLoop(const std::wstring& key, bool loop)
	{
		GetClip(key)->SetLoop(loop);
	}

	void AudioSource::AddClipByKey(const std::wstring& key)
	{
		AudioClip* resource = GETSINGLE(ResourceMgr)->Find<AudioClip>(key);
		mAudioClips.insert(std::make_pair(key, resource));
	}

	AudioClip* AudioSource::GetClip(const std::wstring& key)
	{
		std::map<std::wstring, AudioClip*>::iterator iter = mAudioClips.find(key);

		if (iter != mAudioClips.end())
		{
			return iter->second;
		}

		return nullptr;
	}

	void AudioSource::SetWholeVolume(float volume)
	{
		for (std::pair<const std::wstring, AudioClip*> clip : mAudioClips)
		{
			AudioClip* audioClip = clip.second;
			audioClip->SetVolume(volume);
		}
	}

	void AudioSource::SetWholePitch(float pitch)
	{
		for (std::pair<const std::wstring, AudioClip*> clip : mAudioClips)
		{
			AudioClip* audioClip = clip.second;
			audioClip->SetPitch(pitch);
		}
	}

	void AudioSource::SetWholeVolume_Ratio(float volume)
	{
		for (std::pair<const std::wstring, AudioClip*> clip : mAudioClips)
		{
			AudioClip* audioClip = clip.second;
			float vol = audioClip->GetVolume();
			vol *= volume;
			audioClip->SetVolume(vol);
		}
	}

	void AudioSource::SetWholePitch_Ratio(float pitchValue)
	{
		for (std::pair<const std::wstring, AudioClip*> clip : mAudioClips)
		{
			AudioClip* audioClip = clip.second;
			float pitch = audioClip->GetPitch();
			pitch *= pitchValue;
			audioClip->SetPitch(pitch);
		}
	}

	void AudioSource::SetVolume(const std::wstring& key, float volume)
	{
		mAudioClips.find(key)->second->SetVolume(volume);
	}

	void AudioSource::SetPitch(const std::wstring& key, float pitch)
	{
		mAudioClips.find(key)->second->SetPitch(pitch);
	}

	float AudioSource::GetVolume(const std::wstring& key)
	{
		return mAudioClips.find(key)->second->GetVolume();
	}

	float AudioSource::GetPitch(const std::wstring& key)
	{
		return mAudioClips.find(key)->second->GetPitch();
	}


}