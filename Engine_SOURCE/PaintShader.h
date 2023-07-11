#pragma once
#include "ComputeShader.h"
#include "Texture.h"

namespace dru
{
    class PaintShader :
        public ComputeShader
    {
	public:
		PaintShader();
		~PaintShader();

		virtual void Bind();
		virtual void Clear();

		void SetTarget(Texture* target) { mTarget = target; }
		Texture* GetTarget() const { return mTarget; }

	private:
		Texture* mTarget;
    };

}