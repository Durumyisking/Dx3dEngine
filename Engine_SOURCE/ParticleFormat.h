#pragma once
#include "Renderer.h"

class Model;
class Mesh;
class Material;
class StructedBuffer;
class ParticleFormat
{
public:
	enum class eParticleType
	{
		D2D,
		D3D,
	};

public:
	ParticleFormat(int maxCount, eParticleType type);
	~ParticleFormat();

	void Update();
	void Render();
	void CB_Bind();

	void SetModel(Model* model);
	void SetModel(const std::wstring& name);
	void SetCB_Data(const renderer::ParticleSystemCB& data);

	void Reset();

	StructedBuffer* GetDataBuffer() { return mBuffer; }
	StructedBuffer* GetShaderDataBuffer() { return mSharedBuffer; }

	std::vector<Particle>& GetParticleData() { return mParticleData; }
	const renderer::ParticleSystemCB& GetParticleCB() const { return mParticleCB; }

	bool IsRunning() {return mParticleCB.elapsedTime >= mParticleCB.maxLifeTime;}
private:
	// 파티클의 타입
	eParticleType mParticleType;

	// 파티클 최대 갯수
	int mParitlceMaxCount;

	// 리소스
	Model* mModel;
	Mesh* mMesh;
	Material* mMaterial;

	StructedBuffer* mBuffer;
	StructedBuffer* mSharedBuffer;

	// 파티클의 버퍼에 데이터를 읽고 씀
	std::vector<Particle> mParticleData;
	// 파티클의 상수 버퍼
	renderer::ParticleSystemCB mParticleCB;
};
