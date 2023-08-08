// Material Textures
Texture2D colorTexture          : register(t0);
Texture2D normalTexture         : register(t1);
Texture2D metallicTexture       : register(t2);
Texture2D roughnessTexture      : register(t3);
Texture2D emissiveTexture       : register(t4);

// Merge Targets
Texture2D positionTarget        : register(t5);
Texture2D albedoTarget          : register(t6);
Texture2D normalTarget          : register(t7);
Texture2D MRDTarget             : register(t8); // metallic roughness Depth target
Texture2D EmissiveTarget        : register(t9);

// Merge Light
Texture2D diffuseLightTarget    : register(t10);
Texture2D specularLightTarget   : register(t11);


TextureCube irradianceMap : register(t12);
TextureCube prefilteredMap : register(t13); // 라이트맵 적용

Texture2D BRDF : register(t14);

// t15 파티클 structured buffer
// Texture2D NoiseTexture : register(t16);
Texture2D CubeMapTexture : register(t17);

//StructuredBuffer<LightAttribute> lightAttributes : register(t22);

//Atlas texture
Texture2D atlasTexture : register(t29);

struct BoneMatrix
{
    float4x4 bMatrix;
};
StructuredBuffer<BoneMatrix>BonArray : register(t30);

