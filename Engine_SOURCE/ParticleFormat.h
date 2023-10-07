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
	void CB_Bind(Vector3 pos);

	void SetModel(Model* model);
	void SetModel(const std::wstring& name);

	const renderer::ParticleSystemCB& GetCB_Data() const {return mParticleCB;}
	void SetCB_Data(const renderer::ParticleSystemCB& data);

	void SetTexture(int slot, class Texture* texture, int xCount, int yCount);

	void Reset();

	StructedBuffer* GetDataBuffer() const { return mBuffer; }
	StructedBuffer* GetShaderDataBuffer() const;

	std::vector<Particle>& GetParticleData() { return mParticleData; }
	const renderer::ParticleSystemCB& GetParticleCB() const { return mParticleCB; }

	void SetActiveCount(int count) { mActiveCount = count; }

	bool IsRunning() {return mParticleCB.elapsedTime >= mParticleCB.maxLifeTime;}
private:
	// ��ƼŬ�� Ÿ��
	eParticleType mParticleType;

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
};
