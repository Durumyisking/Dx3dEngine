#include "PostProcessWave.h"

namespace dru
{
	PostProcessWave::PostProcessWave()
		: mAmount(35.f)
		, mSpeed(10.f)
		, mDistortion(40.f)

	{
	}

	PostProcessWave::~PostProcessWave()
	{
	}

	void PostProcessWave::Initialize()
	{
		PostProcess::Initialize();
	}

	void PostProcessWave::update()
	{
		PostProcess::update();
	}

	void PostProcessWave::fixedUpdate()
	{
		PostProcess::fixedUpdate();
	}

	void PostProcessWave::render()
	{
		PostProcess::render();
	}

	void PostProcessWave::Bind()
	{
		ConstantBuffer* cb = renderer::constantBuffers[(UINT)eCBType::PostProcess];
		mConstantBuffer.wave_amount = mAmount;
		mConstantBuffer.wave_speed = mSpeed;
		mConstantBuffer.wave_distortion = mDistortion;

		PostProcess::Bind();
	}

}