#pragma once
#include "Graphics.h"

using namespace dru::enums;

namespace dru::graphics
{
	class ConstantBuffer : GpuBuffer
	{
		friend class Material;

	public:
		ConstantBuffer();
		ConstantBuffer(eCBType _Type);
		virtual ~ConstantBuffer();	

		bool Create(UINT _Size);
		void SetData(void* _Data);
		void Bind(eShaderStage _Stage);
		void Clear();


	private:
		const eCBType mType;
	};

}