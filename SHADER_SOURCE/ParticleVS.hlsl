#include "global.hlsli"

struct VSIn
{
    float4 Pos : POSITION;
    uint Instance : SV_InstanceID; // drawindexedinstance 하면 자동으로 들어간다.
};

struct VSOut
{
    float4 Pos : SV_Position;
    uint Instance : SV_InstanceID;
};

VSOut main(VSIn vsIn)
{
    VSOut vsOut = (VSOut) 0.0f;
    
    vsOut.Pos = vsIn.Pos;
    vsOut.Instance = vsIn.Instance;
    
    return vsOut;
}

