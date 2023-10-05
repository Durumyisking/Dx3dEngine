#pragma once
#include "Engine.h"

class CustomPhysXMemory :
    public physx::PxOutputStream
{
public:
    CustomPhysXMemory(PxU32 initialSize = 0);
    virtual ~CustomPhysXMemory();

    virtual PxU32 write(const void* src, PxU32 count) override;

    PxU8* getData()  { return mBuffer.data(); }
    PxU32 getSize() const { return static_cast<PxU32>(mBuffer.size()); }
private:
    std::vector<PxU8> mBuffer;
};

