#include "global.hlsli"
#include "BRDF.hlsli"

<<<<<<< HEAD

=======
>>>>>>> origin
struct VSOut
{
    float4 Position : SV_Position;
    float2 UV : TEXCOORD;
    float3 ViewPos : POSITION; // viewspace fragmentPos
  
    float3 ViewTangent : TANGENT; // view Space tangent
    float3 ViewNormal : NORMAL; // view Space normal
    float3 ViewBiNormal : BINORMAL; // view Space binormal
      
};

<<<<<<< HEAD
static const float Epsilon = 0.00001;
static const float3 Fdielectric = 0.04;

float4 main(VSOut vsIn) : SV_Target
{
    float4 outColor = (float4) 0.f;
    outColor.w = 1.f;
=======
static const float Epsilon = 0.00001f;
static const float3 Fdielectric = 0.04f;

float4 main(VSOut vsIn) : SV_Target
{
    float3 outColor = (float3) 0.f;
    
    float3 directLighting = (float3) 0.f;
    float3 ambientLighting = (float3) 0.f;

>>>>>>> origin
    float4  albedo = float4(0.5f, 0.5f, 0.5f, 1.f);
    float3  normal = vsIn.ViewNormal;    
    float   metallic = 0.01f;
    float   roughness = 0.5f;
    float3  A0 = (float3) 1.f;
    
<<<<<<< HEAD
    
    if (cbtextureExistence == 1)
=======
    int textureCounts = cbtextureExistence;
        
    if (0 == cbtextureExistence)
>>>>>>> origin
    {
        normal.xyz = vsIn.ViewNormal;
    }
    if (bAlbedo == 1)
    {
        albedo = TextureMapping_albedo(vsIn.UV);
<<<<<<< HEAD
=======
        normal.xyz = vsIn.ViewNormal;
>>>>>>> origin
    }
    if (bNormal == 1)
    {
<<<<<<< HEAD
        normal = TextureMapping_normal(vsIn.UV, vsIn.ViewTangent, vsIn.ViewNormal, vsIn.ViewBiNormal);
    }
    if (bMetallic == 1) 
    {
        //metallic = TextureMapping_metallic(vsIn.UV);
    }
    if (cbroughness == 1)
    {
        //roughness = TextureMapping_roughness(vsIn.UV);
    }
    if (cbemissive == 1)// emissive √ﬂ∞°« ø‰
    {
=======
        albedo = TextureMapping_albedo(vsIn.UV);
        normal = TextureMapping_normal(vsIn.UV, vsIn.ViewTangent, vsIn.ViewNormal, vsIn.ViewBiNormal);
    }
    else if (3 == cbtextureExistence) 
    {
        albedo = TextureMapping_albedo(vsIn.UV);
        normal = TextureMapping_normal(vsIn.UV, vsIn.ViewTangent, vsIn.ViewNormal, vsIn.ViewBiNormal);
        metallic = TextureMapping_metallic(vsIn.UV);
    }
    else if (4 == cbtextureExistence)
    {
        albedo = TextureMapping_albedo(vsIn.UV);
        normal = TextureMapping_normal(vsIn.UV, vsIn.ViewTangent, vsIn.ViewNormal, vsIn.ViewBiNormal);
        metallic = TextureMapping_metallic(vsIn.UV);
        roughness = TextureMapping_roughness(vsIn.UV);
    }
    else if (5 == cbtextureExistence)// emissive Ï∂îÍ∞ÄÌïÑÏöî
    {
        albedo = TextureMapping_albedo(vsIn.UV);
        normal = TextureMapping_normal(vsIn.UV, vsIn.ViewTangent, vsIn.ViewNormal, vsIn.ViewBiNormal);
>>>>>>> origin
    }
    normal.xyz = vsIn.ViewNormal;
    // PBR     
    float3 V = normalize(-vsIn.ViewPos); // Î∑∞Í≥µÍ∞Ñ pinPoint(0,0,0)Î∂ÄÌÑ∞ ÌîΩÏÖÄÎ°ú Ìñ•ÌïòÎäî Î≤°ÌÑ∞
    float3 N = normal; // Ï†ïÏ†ê/ÌÖçÏä§Ï≤ò ÎÖ∏Îßê Î∑∞Î≥ÄÌôò ÏôÑÎ£å
    float3 R = reflect(-V, N);
    float NDotV = saturate(dot(N, V)); // ÎÖ∏Î©Ä to Îàà Î∞òÏÇ¨Í∞Å 

<<<<<<< HEAD
 
    // PBR     
    float3 V = normalize(-vsIn.ViewPos); // ∫‰∞¯∞£ pinPoint(0,0,0)∫Œ≈Õ «»ºø∑Œ «‚«œ¥¬ ∫§≈Õ
    float3 N = normal; // ¡§¡°/≈ÿΩ∫√≥ ≥Î∏ª ∫‰∫Ø»Ø øœ∑·
    float NDotV = saturate(dot(N, V)); // ≥Î∏÷ to ¥´ π›ªÁ∞¢ 

    float3 F0 = lerp(Fdielectric, albedo.xyz, metallic); // ±›º”º∫¿Ã ∞≠«“ºˆ∑œ albedo∏¶ ªÁøÎ«œ∞Ì æ∆¥œ∏È 0.04ªÁøÎ (¿Á¡˙ ∞™)

    
    for (uint i = 0; i < lightCount; ++i)
    {
        float3 Lradiance = lightAttributes[i].color.diffuse.xyz;
        float3 L = -normalize(mul(float4(lightAttributes[i].direction.xyz, 0.f), view)).xyz; // ∫˚ ∞¢µµ

        float3 H = normalize(L + V); // «œ«¡∫§≈Õ : ¥´, ∫˚ π›ªÁ∫§≈Õ
  
        float NDotL = saturate(dot(N, L)); // «•∏È π›ªÁ ∞¢µµ
        float NDotH = saturate(dot(N, H)); // ≥Î∏÷ «œ«¡∫§≈Õ ∞¢µµ («œ«¡∫§≈ÕøÕ ¿œƒ°«“ºˆ∑œ 1¿Ã ≥™ø¬¥Ÿ¥¬∞Õø°º≠ ¿«πÃ¿÷¥¬ ∞™¿Ã¥Ÿ)
        float VDotH = saturate(dot(V, H)); // ¥´ «œ«¡∫§≈Õ ∞¢µµ
        float3 F = FresnelSchlick(F0, VDotH); // ∫˚ π›ªÁ¿≤  metallic¿Ã ≥Ù¿ªºˆ∑œ ∫˚¿ª ∏π¿Ã π›ªÁ«—¥Ÿ.
        float D = NormalDistributionGGXTR(NDotH, roughness); // «•∏È¿« ∞≈ƒß ∫–∆˜∏¶ ∞ËªÍ«‘, πÃºº«•∏È¿« ∞≈ƒ•±‚¥¬ øÏ∏Æ∞° ¡§»Æ»˜ æÀ ºˆ æ¯±‚∂ßπÆø° √ﬂ¡§ƒ°∏¶ ±∏«—¥Ÿ. roughness∞° ≥∑¿ªºˆ∑œ ∫˚¿ª ºº∞‘ π›ªÁ«—¥Ÿ.
        float G = GeometrySmith(NDotL, NDotV, roughness); // ≈◊µŒ∏Æ ∞≈ƒßµµø° ±‚ø©«—¥Ÿ. metallic«— π∞¡˙µÈ¿∫ viewøÕ ≥Î∏ª∫§≈Õ∞° ¿Ã∑Á¥¬ ∞¢µµ∞° ≈¨ºˆ∑œ ∫˚¿ª »Ìºˆ«œ±‚∫∏¥Ÿ¥¬ π›ªÁ«—¥Ÿ.
 
 
        // ∫˚¿∫ π∞¡˙ø° ¥Í¿∏∏È ø©∑Øπ¯ ±º¿˝µ«æÓ »ÆªÍ ªÍ∂ı¿Ã ¿œæÓ≥≠¥Ÿ.
        // ±›º”º∫¿Ã ≥Ù¿ªºˆ∑œ ø°≥ ¡ˆ∏¶ π›ªÁ«œ∞≈≥™ »Ìºˆ«œ±‚∂ßπÆø° »ÆªÍ±‚ø©¥¬ ≥∑¥Ÿ        
        float3 kd = lerp((float3) 1.f - F, (float3) 0.f, metallic); // f∏¶ sub «œ∏È ∞°øÓµ• ¡°∞∞¿∫∞≈ ª˝±Ë

        // Lambert diffuse BRDF.
        float3 diffuseBRDF = kd * albedo.xyz;
        
        // Cook-Torrance specular microfacet BRDF.
        float3 specularBRDF = ((F * G) / max(Epsilon, 4.0f * NDotL * NDotV));

        outColor.xyz += saturate((diffuseBRDF + specularBRDF) * Lradiance * NDotL); // NDotL¿ª ∞ˆ«œ¡ˆ æ ¿∏∏È diffuse∞° ∏µÁ ∏Èø° ¿€øÎ«—¥Ÿ.

    }
    
    if (outColor.w == 0)
        discard;
=======
    float3 F0 = lerp(Fdielectric, albedo.xyz, metallic); // Í∏àÏÜçÏÑ±Ïù¥ Í∞ïÌï†ÏàòÎ°ù albedoÎ•º ÏÇ¨Ïö©ÌïòÍ≥† ÏïÑÎãàÎ©¥ 0.04ÏÇ¨Ïö© (Ïû¨Ïßà Í∞í)

    float3 L = -normalize(mul(float4(lightAttributes[0].direction.xyz, 0.f), view)).xyz; // Îπõ Í∞ÅÎèÑ            

    float NdotL = max(dot(N, L), 0.f);
    
    float3 F = fresnelSchlickRoughness(max(dot(N, V), 0.f), F0, roughness);
    
    float3 kd = lerp((float3) 1.f - F, (float3) 0.f, metallic);

    //float3 irradiance = irradianceMap.Sample(linearSampler, N).rgb;
    //float3 diffuse = irradiance * kd * albedo.xyz;
    float3 diffuse = kd * albedo.xyz;
    
    //const float MAX_REFLECTION_LOD = 4.f;
    //float3 prefilteredColor = prefilteredMap.SampleLevel(linearSampler, R, roughness * MAX_REFLECTION_LOD).rgb;
    float2 envBRDF = BRDF.Sample(linearSampler, float2(max(dot(N, V), 0.f), roughness)).rg;
    //float3 specular = prefilteredColor * (F * envBRDF.x + envBRDF.y);
    float3 specular = (F * envBRDF.x + envBRDF.y);
    
    float3 ambient = (diffuse + specular) * NdotL;
    //float3 ambient = (diffuse + specular); // ÎßàÎîßÏÑ∏Ïù¥Îäî Ï†ÑÎ∞©Ìñ• Îπõ ÎπÑÏ∂îÎäîÎìØ?
>>>>>>> origin
    
    outColor.xyz = ambient;   
        
<<<<<<< HEAD
    return outColor;
    
}
=======
    return float4(outColor, 1.f);;
    
}











//    for (uint i = 0; i < lightCount; ++i)
//    {
//        float3 Lradiance = lightAttributes[i].color.diffuse.xyz;
//        float3 L = (float3) 0.f;

//        if (0 == lightAttributes[i].type)
//        {
//            L = -normalize(mul(float4(lightAttributes[i].direction.xyz, 0.f), view)).xyz; // Îπõ Í∞ÅÎèÑ            
//        }
//        else if (1 == lightAttributes[i].type)
//        {
//            float3 lightViewPos = mul(float4(lightAttributes[i].position.xyz, 1.f), view).xyz;
//            L = vsIn.ViewPos - lightViewPos;
//        }
        
        
//        float3 H = normalize(V + L); // ÌïòÌîÑÎ≤°ÌÑ∞ : Îàà, Îπõ Î∞òÏÇ¨Î≤°ÌÑ∞
        
//        float3 F = fresnelSchlick(max(dot(H, V), 0.f), F0); // Îπõ Î∞òÏÇ¨Ïú®  metallicÏù¥ ÎÜíÏùÑÏàòÎ°ù ÎπõÏùÑ ÎßéÏù¥ Î∞òÏÇ¨ÌïúÎã§.
//        float D = DistributionGGX(N, H, roughness); // ÌëúÎ©¥Ïùò Í±∞Ïπ® Î∂ÑÌè¨Î•º Í≥ÑÏÇ∞Ìï®, ÎØ∏ÏÑ∏ÌëúÎ©¥Ïùò Í±∞Ïπ†Í∏∞Îäî Ïö∞Î¶¨Í∞Ä Ï†ïÌôïÌûà Ïïå Ïàò ÏóÜÍ∏∞ÎïåÎ¨∏Ïóê Ï∂îÏ†ïÏπòÎ•º Íµ¨ÌïúÎã§. roughnessÍ∞Ä ÎÇÆÏùÑÏàòÎ°ù ÎπõÏùÑ ÏÑ∏Í≤å Î∞òÏÇ¨ÌïúÎã§.
//        float G = GeometrySmith(N, V, L, roughness); // ÌÖåÎëêÎ¶¨ Í±∞Ïπ®ÎèÑÏóê Í∏∞Ïó¨ÌïúÎã§. metallicÌïú Î¨ºÏßàÎì§ÏùÄ viewÏôÄ ÎÖ∏ÎßêÎ≤°ÌÑ∞Í∞Ä Ïù¥Î£®Îäî Í∞ÅÎèÑÍ∞Ä ÌÅ¥ÏàòÎ°ù ÎπõÏùÑ Ìù°ÏàòÌïòÍ∏∞Î≥¥Îã§Îäî Î∞òÏÇ¨ÌïúÎã§.
 
 
//        // ÎπõÏùÄ Î¨ºÏßàÏóê ÎãøÏúºÎ©¥ Ïó¨Îü¨Î≤à Íµ¥Ï†àÎêòÏñ¥ ÌôïÏÇ∞ ÏÇ∞ÎûÄÏù¥ ÏùºÏñ¥ÎÇúÎã§.
//        // Í∏àÏÜçÏÑ±Ïù¥ ÎÜíÏùÑÏàòÎ°ù ÏóêÎÑàÏßÄÎ•º Î∞òÏÇ¨ÌïòÍ±∞ÎÇò Ìù°ÏàòÌïòÍ∏∞ÎïåÎ¨∏Ïóê ÌôïÏÇ∞Í∏∞Ïó¨Îäî ÎÇÆÎã§        
//        float3 kd = lerp((float3) 1.f - F, (float3) 0.f, metallic);
//        // Lambert diffuse BRDF.
//        float3 diffuseBRDF = kd * albedo.xyz;
        
//        // Cook-Torrance specular microfacet BRDF.
////        float3 specularBRDF = ((F * D * G) / max(Epsilon, 4.f * NDotL * NDotV));
//        float3 specNumerator = D * G * F;
//        float specDenominator = 4.f * max(dot(N, V), 0.f) * max(dot(N, L), 0.f) + 0.0001f;
//        float3 specularBRDF = specNumerator / specDenominator;
//        float NdotL = max(dot(N, L), 0.f);
        
//        directLighting += saturate((diffuseBRDF + specularBRDF) * Lradiance * NdotL); // NDotLÏùÑ Í≥±ÌïòÏßÄ ÏïäÏúºÎ©¥ diffuseÍ∞Ä Î™®Îì† Î©¥Ïóê ÏûëÏö©ÌïúÎã§.

//    }
>>>>>>> origin
