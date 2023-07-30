// Material Textures
Texture2D colorTexture      : register(t0);
Texture2D normalTexture     : register(t1);
Texture2D metalTexture      : register(t2);
Texture2D roughnessTexture  : register(t3);
Texture2D emissiveTexture   : register(t4);


TextureCube irradianceMap : register(t9);
TextureCube prefilteredMap : register(t10); // ����Ʈ�� ����

Texture2D BRDF : register(t11);

//Atlas texture
Texture2D atlasTexture : register(t12);

struct BoneMatrix
{
    float4x4 bMatrix;
};
StructuredBuffer<BoneMatrix>BonArray : register(t30);


