#pragma once
#include "Script.h"
#include "Camera.h"



class GridScript :
    public Script
{
public:
    GridScript();
    virtual ~GridScript();

    virtual void Initialize() final;
    virtual void Update() final;
    virtual void FixedUpdate() final;
    virtual void Render() final;

    void SetCamera(Camera* camera) { mCamera = camera; }

private:
    Camera* mCamera;
    Vector2  mGridOffset;
    float mThickness;
};
