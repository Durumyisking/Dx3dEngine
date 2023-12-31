#include "ParticleSystem.h"
#include "Mesh.h"

#include "GameObj.h"
#include "Transform.h"
#include "Mesh.h"
#include "ResourceMgr.h"
#include "StructedBuffer.h"
#include "Texture.h"
#include "TimeMgr.h"
#include "Renderer.h"
#include "Material.h"

#include "ParticleFormat.h"


ParticleSystem::ParticleSystem()
	: BaseRenderer(eComponentType::Particle)
	, mParticles{}
	, mCurParticle(nullptr)
	, mParticleEventFunC(nullptr)
	, mCS(nullptr)
	, mbLoop(false)
{

}

ParticleSystem::~ParticleSystem()
{
	for (auto& iter : mParticles)
	{
		if (iter.second == nullptr)
			continue;

		delete iter.second;
		iter.second = nullptr;
	}
}

void ParticleSystem::FixedUpdate()
{
	//==========================================================================
	if (mParticleEventFunC)
	{
		mParticleEventFunC();
		mParticleEventFunC = nullptr;
	}

	if (mCurParticle == nullptr)
		return;

	if (mCurParticle->IsRunning())
	{
		mCurParticle->Reset();

		if (!mbLoop)
		{
			mCurParticle = nullptr;
			return;
		}
	}

	//particleUpdate
	mCurParticle->Update();

	if (mCurParticle->GetAccType() == ParticleFormat::eAccessType::ComputShader)
	{
		// SetStructureData
		StructedBuffer* buffer = mCurParticle->GetDataBuffer();
		StructedBuffer* shaderBuffer = mCurParticle->GetShaderDataBuffer();

		if (buffer != nullptr && shaderBuffer != nullptr)
		{
			// SetCB_Data
			mCurParticle->CB_Bind(GetOwner()->GetTransform()->GetPhysicalPosition());

			// UAV Bind
			mCS->SetStrcutedBuffer(buffer);
			mCS->SetSharedStrutedBuffer(shaderBuffer);
			mCS->OnExcute();
		}
	}

	renderer::ParticleFunCArr.emplace_back(std::bind(&ParticleSystem::ParticleRender, this));
	//==========================================================================
}

void ParticleSystem::ParticleRender()
{
	if (mCurParticle == nullptr)
		return;

	GetOwner()->GetComponent<Transform>()->SetConstantBuffer();
	mCurParticle->Render();
}

void ParticleSystem::SetComputeShader(const std::wstring& shaderName)
{
	mCS = GETSINGLE(ResourceMgr)->Find<ParticleShader>(shaderName);
}

ParticleFormat* ParticleSystem::InsertParticle(const std::wstring& name, const std::wstring& modelname, UINT particleType, int count)
{
	const auto& iter = mParticles.find(name);
	if (iter != mParticles.end())
		return iter->second;
	if (particleType > 1)
		return nullptr;

	ParticleFormat* particle = nullptr;

	if (particleType == static_cast<UINT>(ParticleFormat::eParticleType::D2D))
	{
		particle = new ParticleFormat(count, static_cast<ParticleFormat::eParticleType>(particleType));
	}
	else if (particleType == static_cast<UINT>(ParticleFormat::eParticleType::D3D))
	{
		Model* model = GETSINGLE(ResourceMgr)->Find<Model>(modelname);
		if (model == nullptr)
			return nullptr;

		particle = new ParticleFormat(count, static_cast<ParticleFormat::eParticleType>(particleType));
		particle->SetModel(model);
	}

	if (particle == nullptr)
		return nullptr;

	particle->SetParticleSystem(this);
	mParticles.insert(std::pair(name, particle));

	return particle;
}

bool ParticleSystem::AddParticle(ParticleFormat* particle, const std::wstring& name)
{
	const auto& iter = mParticles.find(name);
	if (iter != mParticles.end())
		return false;

	particle->SetParticleSystem(this);
	mParticles.insert(std::pair(name, particle));
	return true;
}

ParticleFormat* ParticleSystem::Play(const std::wstring& name, int activeCount, bool loop)
{
	const auto& iter = mParticles.find(name);
	if (iter == mParticles.end())
		return nullptr;

	mParticleEventFunC = [this,iter, name, activeCount]()
		{
			mCurParticle = iter->second;
			mCurParticle->Reset();
			mCurParticle->SetActiveCount(activeCount);
		};

	mbLoop = loop;

	return iter->second;
}

void ParticleSystem::Stop()
{
	if (mCurParticle == nullptr)
		return;

	mParticleEventFunC = [this]()
		{
			mCurParticle->Reset();
			mCurParticle = nullptr;
		};

	mbLoop = false;
}

ParticleFormat* ParticleSystem::GetParticleFormat(const std::wstring& name)
{
	const auto& iter = mParticles.find(name);
	if (iter == mParticles.end())
		return nullptr;

	return iter->second;
}

