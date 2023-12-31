#pragma once
#include "BaseRenderer.h"
#include "ParticleShader.h"
#include "Renderer.h"
#include "ParticleFormat.h"



using namespace renderer;
class ParticleSystem : public BaseRenderer
{
	enum class eSimulationSpace
	{
		Local,
		World,
	};

public:
	ParticleSystem();
	virtual ~ParticleSystem();

	virtual void Initialize() final {};
	virtual void Update() final {};
	virtual void FixedUpdate() final;
	virtual void Render() final {};
	void ParticleRender();

	void SetComputeShader(const std::wstring& shaderName);
	ParticleFormat* InsertParticle(const std::wstring& name, const std::wstring& modelname, UINT particleType = 1, int count = 1);
	bool AddParticle(ParticleFormat* particle, const std::wstring& name);

	ParticleFormat* Play(const std::wstring& name, int activeCount = 1, bool loop = true);
	void Stop();

	ParticleFormat* GetParticleFormat(const std::wstring& name);

private:
	std::map<std::wstring, ParticleFormat*> mParticles;
	ParticleFormat* mCurParticle;

	std::function<void()> mParticleEventFunC;
	ParticleShader* mCS;

	bool mbLoop;
};

