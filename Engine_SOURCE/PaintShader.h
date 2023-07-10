#pragma once
#include "ComputeShader.h"
#include "Texture.h"

namespace dru::graphics
{
    class PaintShader :
        public ComputeShader
    {
	public:
		PaintShader();
		~PaintShader();

		virtual void Bind();
		virtual void Clear();

		void SetTarget(Texture* _target) { mTarget = _target; }
		Texture* GetTarget() { return mTarget; }

	private:
		Texture* mTarget;
    };

}