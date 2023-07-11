#pragma once
#include "BaseRenderer.h"

namespace dru
{
	class PostProcess;
    class PostProcessRenderer :
        public BaseRenderer
    {
	public:
		PostProcessRenderer();
		virtual ~PostProcessRenderer();

		virtual void Initialize() final;
		virtual void update() final;
		virtual void fixedUpdate() final;
		virtual void render() final;

		void SetPostProcessOwner(PostProcess* postProcess);

	private:
		PostProcess* mPostProcess;
    };
}
