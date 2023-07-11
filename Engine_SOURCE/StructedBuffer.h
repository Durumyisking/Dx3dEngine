#pragma once
#include "Entity.h"
#include "Graphics.h"

namespace dru::graphics
{
	class StructedBuffer : public GpuBuffer // ����ü ��� Ŭ����?
	{
	public:
		StructedBuffer();
		~StructedBuffer();

		bool Create(UINT size, UINT stride, graphics::eSRVType type, void* data, bool cpuAccess = false);
		void SetData(void* data, UINT stride);
		void GetData(void* data, UINT size = 0); 
		void BindSRV(graphics::eShaderStage stage, UINT slot);
		void BindUAV(eShaderStage stage, UINT slot);

		void Clear();

		UINT GetSize() { return mSize; } // ������ ũ�� (�Ѱ��� ��������? ũ��)
		UINT GetStride() { return mStride; } // ������ ����

	private:
		void setDiscription();
		bool createBuffer(void* data);
		bool createView();
		bool createRWBuffer();

	private:
		Microsoft::WRL::ComPtr<ID3D11Buffer> mWriteBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer> mReadBuffer; private:
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> mSRV;
		Microsoft::WRL::ComPtr<ID3D11UnorderedAccessView> mUAV;
		graphics::eSRVType mType;

		UINT mSize;
		UINT mStride;

		UINT mSRVSlot;
		UINT mUAVSlot;

	};
}