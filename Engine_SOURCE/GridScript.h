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

        virtual void Initialize() final;
        virtual void update() final;
        virtual void fixedUpdate() final;
        virtual void render() final;

        void SetCamera(Camera* camera) { mCamera = camera; }

    private:
        Camera* mCamera;
        Vector2  mGridOffset;
        float mThickness;
    };
}

