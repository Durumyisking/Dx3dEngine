#pragma once
#include "PostProcess.h"



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

	void SetWaveInfo(float amout, float speed, float distortion)
	{
		mAmount = amout;
		mSpeed = speed;
		mDistortion = distortion;
	}

private:
	float mAmount;
	float mSpeed;
	float mDistortion;
};

