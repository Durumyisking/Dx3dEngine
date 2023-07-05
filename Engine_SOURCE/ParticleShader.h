#pragma once
#include "ComputeShader.h"
#include "StructedBuffer.h"

namespace dru::graphics
{
    class ParticleShader :
        public ComputeShader
    {

	public:
		ParticleShader();
		~ParticleShader();

		virtual void Bind() override;
		virtual void Clear() override;

		void SetStrcutedBuffer(StructedBuffer* buffer) { mBuffer = buffer; }
		void SetSharedStrutedBuffer(StructedBuffer* buffer) { mSharedBuffer = buffer; }

	private:
		StructedBuffer* mBuffer;
		StructedBuffer* mSharedBuffer;
	};
}


