#pragma once
#include "Component.h"
#include "AudioClip.h"



class AudioSource : public Component
{
public:
	AudioSource();
	~AudioSource();

	virtual void Initialize() override;
	virtual void Update() override;
	virtual void FixedUpdate() override;
	virtual void Render() override;

	void Play(const std::wstring& key, bool loop = false);
	void Play_NoInterrupt(const std::wstring& key, bool loop = false);
	void Stop(const std::wstring& key);
	void SetLoop(const std::wstring& key, bool loop);

//		void AddClip(std::shared_ptr<AudioClip> clip) { mAudioClip = clip; }
	void AddClipByKey(const std::wstring& key);
	AudioClip* GetClip(const std::wstring& key);

	void SetWholeVolume(float volume);
	void SetWholePitch(float pitch);

	void SetWholeVolume_Ratio(float volume);
	void SetWholePitch_Ratio(float pitch);

	void SetVolume(const std::wstring& key, float volume);
	void SetPitch(const std::wstring& key, float pitch);

	float GetVolume(const std::wstring& key);
	float GetPitch(const std::wstring& key);

	//void Play_interval(const std::wstring& key, float time);
		

private:
	std::map<std::wstring, AudioClip*> mAudioClips;
	float mCurrentTime = 0;
};
