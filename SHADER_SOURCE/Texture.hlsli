// Material Textures
<<<<<<< HEAD
Texture2D colorTexture          : register(t0);
Texture2D normalTexture         : register(t1);
Texture2D metallicTexture       : register(t2);
Texture2D roughnessTexture      : register(t3);
Texture2D emissiveTexture       : register(t4);

// Merge Targets
Texture2D positionTarget        : register(t5);
Texture2D normalTarget          : register(t6);
Texture2D albedoTarget          : register(t7);
Texture2D specularTarget        : register(t8);

// Merge Light
Texture2D diffuseLightTarget    : register(t9);
Texture2D specularLightTarget   : register(t10);

//Atlas texture
Texture2D atlasTexture          : register(t12);
=======
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

>>>>>>> origin

