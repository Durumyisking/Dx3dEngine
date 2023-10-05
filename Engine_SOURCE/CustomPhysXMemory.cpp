#include "CustomPhysXMemory.h"

CustomPhysXMemory::CustomPhysXMemory(PxU32 initialSize)
{
    if (initialSize > 0) {
        mBuffer.reserve(initialSize);
    }
}

CustomPhysXMemory::~CustomPhysXMemory()
{
    mBuffer.clear();
}

PxU32 CustomPhysXMemory::write(const void* src, PxU32 count)
{
    const PxU8* srcBytes = static_cast<const PxU8*>(src);
    mBuffer.insert(mBuffer.end(), srcBytes, srcBytes + count);
    return count;
}