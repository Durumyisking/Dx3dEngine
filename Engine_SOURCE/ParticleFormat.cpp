#include "ParticleFormat.h"
#include "ResourceMgr.h"

#include "Model.h"
#include "Material.h"
#include "Mesh.h"

#include "TimeMgr.h"

ParticleFormat::ParticleFormat(int maxCount, eParticleType type)
	: mParticleType(type)
	, mParitlceMaxCount(1)
	, mModel(nullptr)
	, mMesh(nullptr)
	, mMaterial(nullptr)
	, mParticleData {}
	, mParticleCB {}
	, mBuffer(nullptr)
	, mSharedBuffer(nullptr)
	, mTexture_X_Count(1)
	, mTexture_Y_Count(1)
	, mActiveCount(1)
{
	if (type == eParticleType::D2D)
	{
		Material* mater = GETSINGLE(ResourceMgr)->Find<Material>(L"ParticleMaterial");
		Mesh* mesh = GETSINGLE(ResourceMgr)->Find<Mesh>(L"PointMesh");

		mMaterial = mater;
		mMesh = mesh;
	}

	// CB √ ±‚»≠
	renderer::ParticleSystemCB info = {};
	info.deltaTime = 0.0f;
	info.elapsedTime = 0.0f;

	info.maxLifeTime = 10.0f;
	info.minLifeTime = 0.0f;

	info.startAngle = Vector4::Zero;
	info.endAngle = Vector4(0.0f, 180.f,0.0f,1.0f);

	info.worldPosition = Vector4::Zero;
	info.startSize = Vector4(0.01f, 0.01f, 0.01f, 1.0f);
	info.endSize = Vector4(1.0f, 1.0f, 1.0f, 1.0f);

	info.startColor = Vector4(1.0f, 0.f, 1.0f, 1.0f);
	info.endColor = Vector4(1.0f, 0.f, 1.0f, 1.0f);

	info.maxParticles = maxCount;
	info.simulationSpace = 1; // world

	info.radius = 1.0f;
	info.startSpeed = 10.f;
	info.endSpeed = 10.f;

	info.gravity = -9.8f;
	info.force = 0.0f;

	SetCB_Data(info);

	// InitParticleData
	mParticleData.resize(mParitlceMaxCount);
	for (size_t i = 0; i < mParitlceMaxCount; i++)
	{
		mParticleData[i].position = Vector4::Zero;
		mParticleData[i].direction.Normalize();
		mParticleData[i].lifeTime = info.maxLifeTime;
		mParticleData[i].speed = info.startSpeed;
		mParticleData[i].radian = info.radius;
		mParticleData[i].active = 0;
		mParticleData[i].particleWorld = math::Matrix::Identity;
	}

	// StructureBuffer
	mBuffer = new StructedBuffer();
	mBuffer->Create(sizeof(Particle), maxCount, eSRVType::UAV, &mParticleData, true);

	mSharedBuffer = new StructedBuffer();
	mSharedBuffer->Create(sizeof(ParticleShared), 1, eSRVType::UAV, nullptr, true);
}

ParticleFormat::~ParticleFormat()
{
	if (mBuffer)
	{
		mBuffer->Clear();

		delete mBuffer;
		mBuffer = nullptr;
	}

	if (mSharedBuffer)
	{
		mSharedBuffer->Clear();

		delete mSharedBuffer;
		mSharedBuffer = nullptr;
	}

}

void ParticleFormat::Update()
{
	if (mParticleCB.elapsedTime >= mParticleCB.maxLifeTime)
		return;

	mParticleCB.deltaTime = DT;

	mParticleCB.elapsedTime += DT;
	mParticleCB.gravity += -DT;
}

void ParticleFormat::CB_Bind()
{
	ConstantBuffer* cb = renderer::constantBuffers[static_cast<UINT>(eCBType::ParticleSystem)];
	cb->SetData(&mParticleCB);
	cb->Bind(eShaderStage::CS);
}

void ParticleFormat::Render()
{
	if (mMaterial == nullptr || mMesh == nullptr)
		return;

	Vector2 textureSize(static_cast<float>(mTexture_X_Count), static_cast<float>(mTexture_Y_Count));
	mMaterial->SetData(eGPUParam::Float_2, &textureSize);

	mBuffer->BindSRV(eShaderStage::VS, 16);
	mBuffer->BindSRV(eShaderStage::PS, 16);

	mMaterial->Bind();

	mMesh->BindBuffer();
	mMesh->RenderInstanced(mParitlceMaxCount);

	mMaterial->Clear();

	mBuffer->Clear();
}

void ParticleFormat::SetModel(Model* model)
{
	Mesh* mesh = model->GetMesh(static_cast<UINT>(0));
	Material* mater = GETSINGLE(ResourceMgr)->Find<Material>(L"Particle3DMaterial");

	mMesh = mesh;
	mMaterial = mater;
	mModel = model;
}

void ParticleFormat::SetModel(const std::wstring& name)
{
	Model* model = GETSINGLE(ResourceMgr)->Find<Model>(name);
	if (model == nullptr)
		return;

	SetModel(model);
}

void ParticleFormat::SetCB_Data(const renderer::ParticleSystemCB& data)
{
	mParticleCB = data;
}

void ParticleFormat::SetTexture(int slot, Texture* texture, int xCount, int yCount)
{
	if (mMaterial == nullptr)
		return;

	mMaterial->SetTexture(static_cast<eTextureSlot>(slot), texture);

	mTexture_X_Count = xCount;
	mTexture_Y_Count = yCount;
}

void ParticleFormat::Reset()
{
	mParticleCB.elapsedTime = 0.0f;
	mParticleCB.gravity = 0.0f;

	mParticleCB.force = 0.0f;

	mActiveCount = 1;
}

StructedBuffer* ParticleFormat::GetShaderDataBuffer() const
{
	ParticleShared info = {};
	info.activeCount = mActiveCount;
	mSharedBuffer->SetData(&info,1);

	return mSharedBuffer;
}
