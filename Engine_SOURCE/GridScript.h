#pragma once
#include "Script.h"
#include "Camera.h"

namespace dru
{
    class GridScript :
        public Script
    {
    public:
        GridScript();
        virtual ~GridScript();

        virtual void Initialize() override;
        virtual void update() override;
        virtual void fixedUpdate() override;
        virtual void render() override;

        void SetCamera(Camera* _Camera) { mCamera = _Camera; }

    private:
        Camera* mCamera;

    };
}

