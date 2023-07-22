#include "ParticleShader.h"
#include "Renderer.h"
#include "ConstantBuffer.h"
#include "TimeMgr.h"



ParticleShader::ParticleShader() // ��� ��ǻƮ���̴� ��ӹ�����
	: ComputeShader(128, 1, 1) // ������ 128�� ������
	, mBuffer(nullptr)
	, mSharedBuffer(nullptr) 
{
}
ParticleShader::~ParticleShader()
{
}
void ParticleShader::Bind()
{
	mBuffer->BindUAV(eShaderStage::CS, 0); 
	mSharedBuffer->BindUAV(eShaderStage::CS, 1);
	mGroupX = mBuffer->GetStride() / mThreadGroupCountX + 1;
	mGroupY = 1;
	mGroupZ = 1;
}
void ParticleShader::Clear()
{
	mBuffer->Clear();
	mSharedBuffer->Clear();
}


