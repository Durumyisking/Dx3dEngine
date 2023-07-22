#include "ParticleShader.h"
#include "Renderer.h"
#include "ConstantBuffer.h"
#include "TimeMgr.h"



ParticleShader::ParticleShader() // 얘는 컴퓨트세이더 상속받은애
	: ComputeShader(128, 1, 1) // 쓰레드 128개 쓸거임
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


