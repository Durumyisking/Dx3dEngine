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

	// Cpu Acc Ÿ�Կ��� �����Ͽ� ��ƼŬ��
	// �������� ���� ����������
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
	// ��ƼŬ�� Ÿ��
	eParticleType mParticleType;

	eAccessType mAccType;

	// ��ƼŬ �ִ� ����
	int mParitlceMaxCount;

	// activeCount
	int mActiveCount;

	// ���ҽ�
	Model* mModel;
	Mesh* mMesh;
	Material* mMaterial;

	StructedBuffer* mBuffer;
	StructedBuffer* mSharedBuffer;

	int mTexture_X_Count;
	int mTexture_Y_Count;

	// ��ƼŬ�� ���ۿ� �����͸� �а� ��
	std::vector<Particle> mParticleData;
	// ��ƼŬ�� ��� ����
	renderer::ParticleSystemCB mParticleCB;

	ParticleSystem* mParticleSystem;
};
