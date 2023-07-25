#include "global.hlsli"

//struct BoneMatrix
//{
//    matrix bondemat;
//};
//StructuredBuffer<BoneMatrix> BoneArr : register(t10);


struct VSIn
{
    float4 Position : POSITION;
    float2 UV : TEXCOORD;
    float3 Tangent : TANGENT;
    float3 BiNormal : BINORMAL;
    float3 Normal : NORMAL;
    
    float4 BlendID : BLENDINDICES;
    float4 BlendWeight : BLENDWEIGHT;
};

struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    
    
    float3 ViewPos : POSITION;
    float3 ViewNormal : NORMAL;

    float Intensity : FOG;
};

VSOut main(VSIn vsIn)
{
    VSOut vsOut = (VSOut) 0.0f;
    
    float4 pos = (float4) 0.0f;
    float BlendWeightsArr[4] = (float[4])vsIn.BlendWeight;
    float BlendIDX[4] = (float[4]) vsIn.BlendID;
   
    for (int i = 0; i < 4; ++i)
    {
        float4 localPos = mul(vsIn.Position, BoneArr[BlendIDX[i]].bondeoffsetmat);
        float4 transPomation = mul(vsIn.Position, BoneArr[BlendIDX[i]].bondemat);
        
        pos += transPomation * BlendWeightsArr[i];

    }
    
    //float4 worldPosition = mul(pos, world);
    float4 viewPosition = mul(pos, view);
    float4 projPosition = mul(viewPosition, projection);
    
    vsOut.Position = projPosition;
    vsOut.UV = vsIn.UV;

    // 로컬 노말을 뷰변환
    float3 viewNormal = normalize(mul(float4(vsIn.Normal.xyz, 0.0f), world).xyz);
    viewNormal = normalize(mul(float4(viewNormal, 0.0f), view).xyz);
    
    vsOut.ViewPos = viewPosition.xyz;
    vsOut.ViewNormal = viewNormal.xyz;
    
    return vsOut;
}
