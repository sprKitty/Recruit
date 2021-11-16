struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float3 tangent : TEXCOORD2;
    float3 binormal : TEXCOORD3;
    float4 nearLightPos : POSITION0;
    float4 mediumLightPos : POSITION1;
    float4 farLightPos : POSITION2;
    float4 wPos : POSITION3;
    float4 camPos : POSITION4;
};

struct Material
{
    float4 ambient;
};

struct Light
{
    float4 OnOff;
    float4 pos;
    float3 dir;
    float uvOffset;
    float4 decay;
    float4 color;
    Material mat;
};

struct Camera
{
    float4 pos;
    float4 look;
};

cbuffer ConstantBuffer : register(b0)
{
    Camera camera;
    Light light[3];
};

cbuffer ConstantBuffer1 : register(b1)
{
    float4 vColor;
    int nEnable;
    int nWidth;
    int nHeight;
    int nDummy;
};

Texture2D TEX_MAIN : register(t0);
Texture2D TEX_SCREEN : register(t1);
Texture2D TEX_NEARSHADOW : register(t2);
Texture2D TEX_MEDIUMSHADOW : register(t3);
Texture2D TEX_FARSHADOW : register(t4);
Texture2D TEX_TRILANAR_X : register(t5);
Texture2D TEX_TRILANAR_Y : register(t6);
Texture2D TEX_TRILANAR_Z : register(t7);
Texture2D TEX_BUMP_X : register(t8);
Texture2D TEX_BUMP_Y : register(t9);
Texture2D TEX_BUMP_Z : register(t10);
Texture2D TEX_CLIP : register(t11);
Texture2D TEX_BLUR : register(t12);
Texture2D TEX_GRAYSCALE : register(t13);
Texture2D TEX_WATERDEPTH : register(t14);
Texture2D TEX_TERRAINDEPTH : register(t15);
Texture2D TEX_TOONMAP : register(t16);

SamplerState WRAP : register(s0);
SamplerState CRAMP : register(s1);
SamplerState BORDER : register(s2);
SamplerComparisonState samp3 : register(s3);
SamplerState MIRROR : register(s4);

static const float g_fLength = 0.5f;
static const float g_fScreenX = g_fLength / 1280.0f;
static const float g_fScreenY = g_fLength / 720.0f;
static const int g_nblur = 25;
static const float g_fUnpack = 256.0f;
static const int g_MultiBlur = 3;
static const float2 g_BlurInfo[g_nblur] =
{
    float2(0.0f, 0.0f),
    float2(sin(0) * g_fScreenX, cos(0) * g_fScreenY),
    float2(sin(45) * g_fScreenX, cos(45) * g_fScreenY),
    float2(sin(90) * g_fScreenX, cos(90) * g_fScreenY),
    float2(sin(135) * g_fScreenX, cos(135) * g_fScreenY),
    float2(sin(180) * g_fScreenX, cos(180) * g_fScreenY),
    float2(sin(225) * g_fScreenX, cos(225) * g_fScreenY),
    float2(sin(270) * g_fScreenX, cos(270) * g_fScreenY),
    float2(sin(315) * g_fScreenX, cos(315) * g_fScreenY),
    float2(sin(0) * g_fScreenX * g_MultiBlur, cos(0) * g_fScreenY * g_MultiBlur),
    float2(sin(22) * g_fScreenX * g_MultiBlur, cos(22) * g_fScreenY * g_MultiBlur),
    float2(sin(45) * g_fScreenX * g_MultiBlur, cos(45) * g_fScreenY * g_MultiBlur),
    float2(sin(67) * g_fScreenX * g_MultiBlur, cos(67) * g_fScreenY * g_MultiBlur),
    float2(sin(90) * g_fScreenX * g_MultiBlur, cos(90) * g_fScreenY * g_MultiBlur),
    float2(sin(112) * g_fScreenX * g_MultiBlur, cos(112) * g_fScreenY * g_MultiBlur),
    float2(sin(135) * g_fScreenX * g_MultiBlur, cos(135) * g_fScreenY * g_MultiBlur),
    float2(sin(157) * g_fScreenX * g_MultiBlur, cos(157) * g_fScreenY * g_MultiBlur),
    float2(sin(180) * g_fScreenX * g_MultiBlur, cos(180) * g_fScreenY * g_MultiBlur),
    float2(sin(202) * g_fScreenX * g_MultiBlur, cos(202) * g_fScreenY * g_MultiBlur),
    float2(sin(225) * g_fScreenX * g_MultiBlur, cos(225) * g_fScreenY * g_MultiBlur),
    float2(sin(247) * g_fScreenX * g_MultiBlur, cos(247) * g_fScreenY * g_MultiBlur),
    float2(sin(270) * g_fScreenX * g_MultiBlur, cos(270) * g_fScreenY * g_MultiBlur),
    float2(sin(292) * g_fScreenX * g_MultiBlur, cos(292) * g_fScreenY * g_MultiBlur),
    float2(sin(315) * g_fScreenX * g_MultiBlur, cos(315) * g_fScreenY * g_MultiBlur),
    float2(sin(337) * g_fScreenX * g_MultiBlur, cos(337) * g_fScreenY * g_MultiBlur),
};

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = 1;
    
    // Tri-Planarマッピング
    float3 blending = abs(pin.normal);
    blending = normalize(max(blending, 0.001f));
    if(blending.y < 0.7f)
        blending.y = 0.0f;
    float b = blending.x + blending.y + blending.z;
    blending /= b;
    
    float4 xaxis = TEX_TRILANAR_X.Sample(MIRROR, pin.wPos.zy * 0.04f);
    float4 yaxis = TEX_TRILANAR_Y.Sample(MIRROR, pin.wPos.xz * 0.005f);
    float4 zaxis = TEX_TRILANAR_Z.Sample(MIRROR, pin.wPos.xy * 0.04f);
    
    color.rgb = xaxis * blending.x + yaxis * blending.y + zaxis * blending.z;
    
    // バンプマッピング
    float4 xAxisBump = TEX_BUMP_X.Sample(MIRROR, pin.wPos.zy * 0.04f) * 2 - 1;
    float4 yAxisBump = TEX_BUMP_Y.Sample(MIRROR, pin.wPos.xz * 0.005f) * 2 - 1;
    float4 zAxisBump = TEX_BUMP_Z.Sample(MIRROR, pin.wPos.xy * 0.04f) * 2 - 1;
    
    float3 bumpNormalX = xAxisBump.x * pin.binormal.x + xAxisBump.y * pin.binormal.y + xAxisBump.z * pin.binormal.z;
    float3 bumpNormalY = yAxisBump.x * pin.binormal.x + yAxisBump.y * pin.binormal.y + yAxisBump.z * pin.binormal.z;
    float3 bumpNormalZ = zAxisBump.x * pin.binormal.x + zAxisBump.y * pin.binormal.y + zAxisBump.z * pin.binormal.z;
    float3 lightDir = normalize(-light[1].dir);
    float lightIntensityX = saturate(dot(bumpNormalX, lightDir));
    float lightIntensityY = saturate(dot(bumpNormalY, lightDir));
    float lightIntensityZ = saturate(dot(bumpNormalZ, lightDir));
    float3 lightColor = 1.0f;
    float3 bumpColor = max((saturate(lightColor * lightIntensityX * blending.x) + saturate(lightColor * lightIntensityY * blending.y) + saturate(lightColor * lightIntensityZ * blending.z)),0.2f);
    color.rgb *= (bumpColor * 3.0f);
    
    // シャドウマッピング
    float inLVP;
    float2 mapUV;
    float3 shadowColor = 0;
    float depthColor = 0;
    inLVP = pin.nearLightPos.z / pin.nearLightPos.w;
    mapUV.x = (1.0f + pin.nearLightPos.x / pin.nearLightPos.w) * 0.5f;
    mapUV.y = (1.0f - pin.nearLightPos.y / pin.nearLightPos.w) * 0.5f;
    if (mapUV.x > 0.0f && mapUV.x < 1.0f && mapUV.y > 0.0f && mapUV.y < 1.0f)
    {
        for (int i = 0; i < g_nblur; ++i)
        {
            float4 mapColor = TEX_NEARSHADOW.Sample(BORDER, mapUV + g_BlurInfo[i]);
            depthColor = mapColor.r;
            depthColor += mapColor.g / 256.0f;
            depthColor += mapColor.b / 256.0f / 256.0f;
            if (inLVP > depthColor)
            {
                shadowColor += (color.rgb * 0.3f);
            }
        }
        shadowColor /= g_nblur;
        color.rgb = color.rgb - shadowColor;
        return color;
    }
    
    shadowColor = 0;
    inLVP = pin.mediumLightPos.z / pin.mediumLightPos.w;
    mapUV.x = (1.0f + pin.mediumLightPos.x / pin.mediumLightPos.w) * 0.5f;
    mapUV.y = (1.0f - pin.mediumLightPos.y / pin.mediumLightPos.w) * 0.5f;
    if (mapUV.x > 0.0f && mapUV.x < 1.0f && mapUV.y > 0.0f && mapUV.y < 1.0f)
    {
        float4 mapColor = TEX_MEDIUMSHADOW.Sample(BORDER, mapUV);
        depthColor = mapColor.r;
        depthColor += mapColor.g / 256.0f;
        depthColor += mapColor.b / 256.0f / 256.0f;
        if (inLVP > depthColor)
        {
            shadowColor += (color.rgb * 0.3f);
        }
        color.rgb = color.rgb - shadowColor;
        return color;
    }
    
    shadowColor = 0;
    inLVP = pin.farLightPos.z / pin.farLightPos.w;
    mapUV.x = (1.0f + pin.farLightPos.x / pin.farLightPos.w) * 0.5f;
    mapUV.y = (1.0f - pin.farLightPos.y / pin.farLightPos.w) * 0.5f;
    if (mapUV.x > 0.0f && mapUV.x < 1.0f && mapUV.y > 0.0f && mapUV.y < 1.0f)
    {
        float4 mapColor = TEX_FARSHADOW.Sample(BORDER, mapUV);
        depthColor = mapColor.r;
        depthColor += mapColor.g / 256.0f;
        depthColor += mapColor.b / 256.0f / 256.0f;
        if (inLVP > depthColor)
        {
            shadowColor += (color.rgb * 0.3f);
        }
        color.rgb = color.rgb - shadowColor;
        return color;
    }
    
    return color;
}