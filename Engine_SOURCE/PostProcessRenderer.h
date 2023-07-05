#pragma once
#include "BaseRenderer.h"

namespace dru
{
    class PostProcessRenderer :
        public BaseRenderer
    {
	public:
		PostProcessRenderer();
		virtual ~PostProcessRenderer();

		virtual void Initialize() override;
		virtual void update() override;
		virtual void fixedUpdate() override;
		virtual void render() override;

		void SetPostProcessOwner(PostProcess* _PostProcess);

	private:
		PostProcess* mPostProcess;
    };
}
