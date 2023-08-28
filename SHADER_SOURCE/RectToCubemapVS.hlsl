#include "global.hlsli"

struct VSIn
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD;
};

struct VSOut
{
    float4 Position : SV_POSITION;
    float2 UV : TEXCOORD;
    float3 CUBEUV : TEXCOORD1;
};

VSOut main(VSIn vsIn)
{
    VSOut ouput = (VSOut) 0.0f;
    
    //float4 worldPosition = mul(float4(In.Pos, 1.0f), world);
    //float4 viewPosition = mul(worldPosition, view);
    //float4 ProjPosition = mul(viewPosition, projection);
    
    float4 viewPos = mul(float4(vsIn.Position.xyz, 0.0f), view);
    float4 projPos = mul(float4(viewPos.xyz, 1.0f), projection);
    projPos.z = projPos.w;
    
    ouput.Position = projPos;
    ouput.UV = vsIn.UV;
    ouput.CUBEUV = normalize(vsIn.Position);
    
    return ouput;

}


//struct VSIn
//{
//    float4 Position : Position;
//};

//struct VSOut
//{
//    float4 Position : SV_Position;
//    float3 WorldPos : Position;
//};

//VSOut main(VSIn vsIn)
//{
//    VSOut vsOut = (VSOut) 0.f;
    
//    vsIn.Position.w = 1.f;
    
//    float4 worldPosition = mul(vsIn.Position, world);
//    float4 viewPosition = mul(worldPosition, view);
//    float4 projPosition = mul(viewPosition, projection); // cubemap proj °öÇØ¾ßÇÒµí?
    
//    vsOut.WorldPos = vsIn.Position;
//    vsOut.Position = mul(float4(vsIn.Position.xyz, 1.0), cubemapMat);
//    vsOut.Position.z = vsOut.Position.w;
    
//    return vsOut;
//}
