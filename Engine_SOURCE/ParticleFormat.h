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
	// ��ƼŬ�� Ÿ��
	eParticleType mParticleType;

	// ��ƼŬ �ִ� ����
	int mParitlceMaxCount;

	// ���ҽ�
	Model* mModel;
	Mesh* mMesh;
	Material* mMaterial;

	StructedBuffer* mBuffer;
	StructedBuffer* mSharedBuffer;

	// ��ƼŬ�� ���ۿ� �����͸� �а� ��
	std::vector<Particle> mParticleData;
	// ��ƼŬ�� ��� ����
	renderer::ParticleSystemCB mParticleCB;
};
