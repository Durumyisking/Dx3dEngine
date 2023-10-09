#pragma once
#include "Renderer.h"

class Model;
class Mesh;
class Material;
class StructedBuffer;
class ParticleSystem;
class ParticleFormat
{
public:
	enum class eParticleType
	{
		D2D,
		D3D,
	};

	enum class eAccessType
	{
		CPU,
		ComputShader,
	};

public:
	ParticleFormat(int maxCount, eParticleType type);
	virtual ~ParticleFormat();

	virtual void Update();
	virtual void Initalize();
	virtual void Render();
	void CB_Bind(Vector3 pos);

	const renderer::ParticleSystemCB& GetCB_Data() const {return mParticleCB;}
	void SetCB_Data(const renderer::ParticleSystemCB& data);

	void SetTexture(int slot, class Texture* texture, int xCount, int yCount);

	void Reset();

	// Cpu Acc 타입에면 구현하여 파티클의
	// 움직임을 직접 구현가능함
	virtual void Calculator() {};

public:
	StructedBuffer* GetDataBuffer() const;
	StructedBuffer* GetShaderDataBuffer() const;

	void SetModel(Model* model);
	void SetModel(const std::wstring& name);

	void SetParticleData(const Particle& particleData);
	void SetParticleData(const std::vector<Particle>& particleDatas);

	std::vector<Particle>& GetParticleData() { return mParticleData; }
	const renderer::ParticleSystemCB& GetParticleCB() const { return mParticleCB; }

	void SetActiveCount(int count) { mActiveCount = count; }

	bool IsRunning() {return mParticleCB.elapsedTime >= mParticleCB.maxLifeTime;}
	eAccessType GetAccType() { return mAccType; }

	void SetParticleSystem(ParticleSystem* system) { mParticleSystem = system; }
	
protected:
	// 파티클의 타입
	eParticleType mParticleType;

	eAccessType mAccType;

	// 파티클 최대 갯수
	int mParitlceMaxCount;

	// activeCount
	int mActiveCount;

	// 리소스
	Model* mModel;
	Mesh* mMesh;
	Material* mMaterial;

	StructedBuffer* mBuffer;
	StructedBuffer* mSharedBuffer;

	int mTexture_X_Count;
	int mTexture_Y_Count;

	// 파티클의 버퍼에 데이터를 읽고 씀
	std::vector<Particle> mParticleData;
	// 파티클의 상수 버퍼
	renderer::ParticleSystemCB mParticleCB;

	ParticleSystem* mParticleSystem;
};
