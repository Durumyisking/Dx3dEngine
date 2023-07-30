// Material Textures
Texture2D colorTexture      : register(t0);
Texture2D normalTexture     : register(t1);
Texture2D MetalTexture      : register(t2);
Texture2D RoughnessTexture  : register(t3);
Texture2D EmissiveTexture   : register(t4);


TextureCube irradianceMap : register(t9);
TextureCube prefilteredMap : register(t10); // 라이트맵 적용

Texture2D BRDF : register(t11);

//Atlas texture
Texture2D atlasTexture : register(t12);

struct BoneMatrix
{
    float4x4 bMatrix;
};
StructuredBuffer<BoneMatrix>BonArray : register(t30);


