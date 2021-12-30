struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float4 worldPos : TEXCOORD2;
    float4 lightPos : TEXCOORD3;
    float4 reflectionCam : TEXCOORD4;
    float3 texSpaceLight : TEXCOORD5;
};

struct LightInfo
{
    float4 pos;
    float4 dir;
    float4 decay;
    float4 color;
};

struct CameraInfo
{
    float4 pos;
    float4 vpSize;
};

struct TexSetting
{
    float2 tile;
    float2 offset;
    float4 multipray;
    int nWrap;
    float fAlpha;
    float fTime;
    float fGameStartCnt;
};

struct PostEffect
{
    int4 blur;
};
cbuffer ConstantBuffer0 : register(b0)
{
    CameraInfo g_cameraInfo;
};

cbuffer ConstantBuffer1 : register(b1)
{
    LightInfo g_lightInfo;
};

cbuffer ConstantBuffer2 : register(b2)
{
    TexSetting g_texSetting;
}

cbuffer ConstantBuffer3 : register(b3)
{
    PostEffect g_postEffect;
}

Texture2D TEX_MAIN : register(t0);
Texture2D TEX_WATER : register(t1);
Texture2D TEX_WATERHEIGHT : register(t2);
Texture2D TEX_WATERBUMP : register(t3);
Texture2D TEX_DOS : register(t4);
Texture2D TEX_DOF : register(t5);
Texture2D TEX_BUMP : register(t6);
Texture2D TEX_GRAYSCALE : register(t7);
Texture2D TEX_BLURX : register(t8);
Texture2D TEX_BLURY : register(t9);
Texture2D TEX_KAWASE1 : register(t10);
Texture2D TEX_KAWASE2 : register(t11);
Texture2D TEX_KAWASE3 : register(t12);
Texture2D TEX_KAWASE4 : register(t13);
Texture2D TEX_EFFECT : register(t14);

SamplerState WRAP : register(s0);
SamplerState CRAMP : register(s1);
SamplerState BORDER : register(s2);
SamplerComparisonState samp3 : register(s3);
SamplerState MIRROR : register(s4);


float4 main(PS_IN pin) : SV_Target
{
    float4 color = 1.5f;
    
    float3 dir = normalize(g_cameraInfo.pos.xyz - pin.worldPos.xyz);
    
    float2 uv = pin.uv;
    uv *= 1.f;
    uv.xy += g_texSetting.fGameStartCnt * .01f;
    float height = TEX_GRAYSCALE.Sample(WRAP, pin.uv).r;
    
    float2 texUV = uv + .08f * height * dir.xy;
    float3 N = TEX_WATERBUMP.Sample(WRAP, texUV).xyz * 2.f - 1.f;
    float3 L = normalize(g_lightInfo.pos.xyz - pin.worldPos.xyz);
    
    float3 R = 2.f * N * dot(N, L) - L;
    float I = pow(saturate(dot(R, dir)), 1) + .5f;
    I *= TEX_WATERHEIGHT.Sample(WRAP, uv).r;
    
    float2 mapUV;
    mapUV.x = (1.0f + pin.reflectionCam.x / pin.reflectionCam.w) * 0.5f;
    mapUV.y = (1.0f - pin.reflectionCam.y / pin.reflectionCam.w) * 0.5f;
    float3 mapColor = TEX_WATER.Sample(CRAMP, mapUV).rgb;
    color.rgb -= I;
    color *= float4(.0f / 255.f, 120.f / 255.f, 200.f / 255.f, 1);
    color.rgb *= mapColor;
    color.a = .8f;
    
    return color;
}
