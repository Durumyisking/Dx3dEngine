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

		void SetTarget(std::shared_ptr<Texture> _target) { mTarget = _target; }
		std::shared_ptr<Texture> GetTarget() { return mTarget; }

	private:
		std::shared_ptr<Texture> mTarget;
    };

}