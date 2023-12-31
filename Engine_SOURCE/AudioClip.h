#pragma once
#include "EngineResource.h"
#include "Fmod.h"

//#include <Fmod/fmod_studio.hpp>
//#include <Fmod/fmod.hpp>
//#include <Fmod/fmod_common.h>
//#include <Fmod/fmod_codec.h>

//#include "..\External\Include\\DirectXTex\DirectXTex.h"



using namespace math;

class AudioClip : public Resource
{
public:
	AudioClip();
	~AudioClip();

	virtual HRESULT Load(const std::wstring& path) override;
	virtual HRESULT LoadFullpath(const std::wstring& path) override;

	void Play();
	void Stop();
	void Set3DAttributes(const Vector3 pos, const Vector3 vel);
	void SetLoop(bool loop) { mbLoop = loop; }

	bool IsPlaying() const 
	{
		bool playing = false;
		mChannel->isPlaying(&playing);
		return playing;
	}

	void SetVolume(float volume)
	{
		mVolume = volume;
		mChannel->setVolume(mVolume);
	}
	float GetVolume()  
	{
		mChannel->getVolume(&mVolume);
		return mVolume;
	}
	void SetPitch(float pitch)
	{
		mPitch = pitch;
		mChannel->setPitch(mPitch);
	}
	float GetPitch()
	{
		mChannel->getPitch(&mPitch);
		return mPitch;
	}

private:
	FMOD::Sound* mSound;
	FMOD::Channel* mChannel;
	float mMinDistance;
	float mMaxDistance;
	float mVolume;
	float mVolumeRatio;
	float mPitch;
	float mPitchRatio;
	bool mbLoop;
};
