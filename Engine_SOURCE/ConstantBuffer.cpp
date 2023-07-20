#include "druMath.h"
#include "ConstantBuffer.h"
#include "GraphicDevice.h"

using namespace dru::math;

namespace dru
{
	ConstantBuffer::ConstantBuffer()
		: mType(eCBType::End)
	{
	}

	ConstantBuffer::ConstantBuffer(eCBType type)
		: mType(type)
	{
	}

	ConstantBuffer::~ConstantBuffer()
	{
	}

	bool ConstantBuffer::Create(UINT size)
	{
		desc.ByteWidth = size; // 들고있을 데이터 크기만큼 (일단은 위치 정보만)
		desc.BindFlags = D3D11_BIND_FLAG::D3D11_BIND_CONSTANT_BUFFER;
		desc.Usage = D3D11_USAGE::D3D11_USAGE_DYNAMIC;
		desc.CPUAccessFlags = D3D11_CPU_ACCESS_FLAG::D3D11_CPU_ACCESS_WRITE;
		desc.MiscFlags = 0;
		desc.StructureByteStride = 0;

		if (!GetDevice()->CreateBuffer(&desc, nullptr, buffer.GetAddressOf()))
			return false;

		return true;
	}

	void ConstantBuffer::SetData(void* data)
	{
		GetDevice()->BindBuffer(buffer.Get(), data, desc.ByteWidth);
	}

	void ConstantBuffer::Bind(eShaderStage stage)
	{
		if (stage == eShaderStage::All)
		{
			GetDevice()->BindConstantBuffer(eShaderStage::VS, mType, buffer.Get());
			GetDevice()->BindConstantBuffer(eShaderStage::HS, mType, buffer.Get());
			GetDevice()->BindConstantBuffer(eShaderStage::DS, mType, buffer.Get());
			GetDevice()->BindConstantBuffer(eShaderStage::GS, mType, buffer.Get());
			GetDevice()->BindConstantBuffer(eShaderStage::PS, mType, buffer.Get());
			GetDevice()->BindConstantBuffer(eShaderStage::CS, mType, buffer.Get());
		}
		else
		{
			GetDevice()->BindConstantBuffer(stage, mType, buffer.Get());
		}
	}

	void ConstantBuffer::Clear()
	{
		GetDevice()->ClearConstantBuffer(buffer.Get(), desc.ByteWidth);

		UINT stageCount = static_cast<UINT>(eShaderStage::End);
		for (UINT stage = 0; stage < stageCount; ++stage)
		{
			GetDevice()->BindConstantBuffer(static_cast<eShaderStage>(stage), mType, buffer.Get());
		}
	}
}