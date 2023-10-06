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

TextureCube CubeMapTexture : register(t12);
TextureCube irradianceMap : register(t13);
TextureCube prefilteredMap : register(t14); // 라이트맵 적용

Texture2D BRDF : register(t15);

//  파티클 structured buffer t16
// Texture2D NoiseTexture : register(t17

Texture2D SkySphere : register(t18);

Texture2D ShadowMap : register(t19);

// postProcess
Texture2D postProcessTexture : register(t20);


//StructuredBuffer<LightAttribute> lightAttributes : register(t22);



//Atlas texture
Texture2D atlasTexture : register(t29);



struct BoneMatrix
{
    float4x4 bMatrix;
};

StructuredBuffer<BoneMatrix>BonArray : register(t30);

