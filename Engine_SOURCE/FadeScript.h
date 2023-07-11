#pragma once
#include "Script.h"

namespace dru
{
    class FadeScript : public Script
    {
    public:
        FadeScript();
        virtual ~FadeScript();

        virtual void Initialize() final;
        virtual void Update() final;
        virtual void FixedUpdate() final;
        virtual void Render() final;

        //  */
        void SetFadeType(int type) { mFadeType = type; }

        void SetFadeValue(float value) { mFadeValue = value; }
        void SetFadeTextureType(int value) { mFadeTextureType = value; }
        void SetFadeTime(float value) { mTime = value; }

        float GetElapsedTime() const { return mElapsedTime; }

        void restart(int fadeType);


    private:
        int   mFadeType;
        int   mFadeTextureType;
        float mFadeValue;
        float mTime;
        float mElapsedTime;

        bool  mStart;

    };
}


