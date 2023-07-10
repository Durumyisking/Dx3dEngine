#pragma once
#include "PostProcess.h"

namespace dru
{
    class PostProcessWave :
        public PostProcess
    {
	public:
		PostProcessWave();
		virtual ~PostProcessWave();

		virtual void Initialize();
		virtual void update();
		virtual void fixedUpdate();
		virtual void render();

		virtual void Bind();

		void SetWaveInfo(float _Amout, float _Speed, float _Distortion)
		{
			mAmount = _Amout;
			mSpeed = _Speed;
			mDistortion = _Distortion;
		}

	private:
		float mAmount;
		float mSpeed;
		float mDistortion;
    };

}