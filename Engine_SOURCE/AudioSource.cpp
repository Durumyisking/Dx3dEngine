#include "AudioSource.h"
#include "Transform.h"
#include "GameObj.h"
#include "AudioClip.h"
#include "TimeMgr.h"

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

	void AudioSource::update()
	{
	}

	void AudioSource::fixedUpdate()
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

	void AudioSource::render()
	{
	}

	void AudioSource::Play(const std::wstring& _key, bool _loop)
	{
		GetClip(_key)->SetLoop(_loop);
		GetClip(_key)->Play();
	}

	void AudioSource::Play_NoInterrupt(const std::wstring& _key, bool _loop)
	{
		if (!GetClip(_key)->IsPlaying())
		{
			GetClip(_key)->SetLoop(_loop);
			GetClip(_key)->Play();
		}
	}

	void AudioSource::Stop(const std::wstring& _key)
	{
		GetClip(_key)->Stop();
	}

	void AudioSource::SetLoop(const std::wstring& _key, bool loop)
	{
		GetClip(_key)->SetLoop(loop);
	}

	void AudioSource::AddClipByKey(const std::wstring& _key)
	{
		std::shared_ptr<AudioClip> resource = GETSINGLE(Resources)->Find<AudioClip>(_key);
		mAudioClips.insert(std::make_pair(_key, resource.get()));
	}

	AudioClip* AudioSource::GetClip(const std::wstring& _key)
	{
		std::map<std::wstring, AudioClip*>::iterator iter = mAudioClips.find(_key);

		if (iter != mAudioClips.end())
		{
			return iter->second;
		}

		return nullptr;
	}

	void AudioSource::SetWholeVolume(float _Volume)
	{
		for (std::pair<const std::wstring, AudioClip*> clip : mAudioClips)
		{
			AudioClip* audioClip = clip.second;
			audioClip->SetVolume(_Volume);
		}
	}

	void AudioSource::SetWholePitch(float _Pitch)
	{
		for (std::pair<const std::wstring, AudioClip*> clip : mAudioClips)
		{
			AudioClip* audioClip = clip.second;
			audioClip->SetPitch(_Pitch);
		}
	}

	void AudioSource::SetWholeVolume_Ratio(float _Volume)
	{
		for (std::pair<const std::wstring, AudioClip*> clip : mAudioClips)
		{
			AudioClip* audioClip = clip.second;
			float vol = audioClip->GetVolume();
			vol *= _Volume;
			audioClip->SetVolume(vol);
		}
	}

	void AudioSource::SetWholePitch_Ratio(float _Pitch)
	{
		for (std::pair<const std::wstring, AudioClip*> clip : mAudioClips)
		{
			AudioClip* audioClip = clip.second;
			float pitch = audioClip->GetPitch();
			pitch *= _Pitch;
			audioClip->SetPitch(pitch);
		}
	}

	void AudioSource::SetVolume(const std::wstring& _key, float _Volume)
	{
		mAudioClips.find(_key)->second->SetVolume(_Volume);
	}

	void AudioSource::SetPitch(const std::wstring& _key, float _Pitch)
	{
		mAudioClips.find(_key)->second->SetPitch(_Pitch);
	}

	float AudioSource::GetVolume(const std::wstring& _key)
	{
		return mAudioClips.find(_key)->second->GetVolume();
	}

	float AudioSource::GetPitch(const std::wstring& _key)
	{
		return mAudioClips.find(_key)->second->GetPitch();
	}


}