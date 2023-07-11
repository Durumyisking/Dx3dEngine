#pragma once
#include "GameObj.h"


namespace dru
{
	class PostProcessRenderer;
	class PostProcess :
		public GameObj
	{
	public:
		PostProcess();
		virtual ~PostProcess();

		virtual void Initialize();
		virtual void update();
		virtual void fixedUpdate();
		virtual void render();

		virtual void Bind();
		void Clear();

		void SetMaterial(std::wstring key);

	protected:
		Material* mPostProcessMaterial;
		PostProcessCB mConstantBuffer;
		PostProcessRenderer* mRenderer;
	};

}