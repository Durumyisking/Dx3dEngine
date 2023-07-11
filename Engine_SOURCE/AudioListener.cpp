#include "AudioListener.h"
#include "Transform.h"
#include "GameObj.h"
#include "Fmod.h"

namespace dru
{
	AudioListener::AudioListener()
		: Component(eComponentType::AudioListener)
	{
	}

	AudioListener::~AudioListener()
	{
	}

	void AudioListener::Initialize()
	{
	}

	void AudioListener::Update()
	{
	}

	void AudioListener::FixedUpdate()
	{
		Transform* tr = GetOwner()->GetComponent<Transform>();
		Vector3 pos = tr->GetWorldPosition();
		pos.z = 0.f;
		Vector3 foward = tr->Forward();
		Vector3 up = tr->Up();

		Vector3 vel = { 0.0f, 0.0f, 0.0f };
		GETSINGLE(Fmod)->Set3DListenerAttributes(&pos, &vel, &foward, &up);
	}

	void AudioListener::Render()
	{
	}

}