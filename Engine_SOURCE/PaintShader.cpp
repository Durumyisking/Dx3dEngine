#include "PaintShader.h"
#include "ConstantBuffer.h"
#include "Renderer.h"
#include "TimeMgr.h"

namespace dru::graphics
{
	PaintShader::PaintShader()
		: mTarget(nullptr)
	{
	}

	PaintShader::~PaintShader()
	{
	}
	void PaintShader::Bind()
	{
		mTarget->BindUnorderedAccessview(0);
		mGroupX = mTarget->GetWidth() / mThreadGroupCountX + 1;
		mGroupY = mTarget->GetHeight() / mThreadGroupCountY + 1;
		mGroupZ = 1;
	}

	void PaintShader::Clear()
	{
		mTarget->ClearUnorderedAccessview(0);
	}


}