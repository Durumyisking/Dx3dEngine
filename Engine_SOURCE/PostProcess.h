#pragma once
#include "GameObj.h"


namespace dru
{
<<<<<<< Updated upstream
	class CPostProcessRenderer;
	class CPostProcess :
		public CGameObj
=======
	class PostProcessRenderer;
	class PostProcess : public GameObj
>>>>>>> Stashed changes
	{
	public:
		CPostProcess();
		virtual ~CPostProcess();

		virtual void Initialize();
		virtual void update();
		virtual void fixedUpdate();
		virtual void render();

		virtual void Bind();
		void Clear();

		void SetMaterial(std::wstring _Key);

	protected:
		std::shared_ptr<CMaterial> mPostProcessMaterial;
		PostProcessCB mConstantBuffer;
		CPostProcessRenderer* mRenderer;
	};

}