// Material Textures

Texture2D colorTexture      : register(t0);
Texture2D normalTexture     : register(t1);
Texture2D metalTexture      : register(t2);
Texture2D roughnessTexture  : register(t3);
Texture2D emissiveTexture   : register(t4);

// Merge Targets
Texture2D positionTarget        : register(t5);
Texture2D normalTarget          : register(t6);
Texture2D albedoTarget          : register(t7);
Texture2D specularTarget        : register(t8);

// Merge Light
Texture2D diffuseLightTarget    : register(t9);
Texture2D specularLightTarget   : register(t10);

// PBR Related Resources
TextureCube irradianceMap : register(t11);
TextureCube prefilteredMap : register(t12); // ¶óÀÌÆ®¸Ê Àû¿ë

Texture2D BRDF : register(t13);

//Atlas texture
Texture2D atlasTexture          : register(t29);

struct BoneMatrix
{
    float4x4 bMatrix;
};
StructuredBuffer<BoneMatrix>BonArray : register(t30);

