struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float4 wPos : TEXCOORD2;
    float4 lightPos : TEXCOORD3;
    float4 camPos : TEXCOORD4;
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
};

struct TexSetting
{
    float2 tile;
    float2 offset;
    float4 multipray;
    int nWrap;
    float fAlpha;
    float fTime;
    float fDummy;
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

Texture2D TEX_MAIN : register(t0);
Texture2D TEX_SCREEN : register(t1);
Texture2D TEX_DOS : register(t2);
Texture2D TEX_DOF : register(t3);
Texture2D TEX_BUMP : register(t4);
Texture2D TEX_CLIP : register(t5);
Texture2D TEX_BLUR : register(t6);
Texture2D TEX_GRAYSCALE : register(t7);

SamplerState WRAP : register(s0);
SamplerState CRAMP : register(s1);
SamplerState BORDER : register(s2);
SamplerComparisonState samp3 : register(s3);
SamplerState MIRROR : register(s4);



float4 main(PS_IN pin) : SV_Target
{
    int nCramp = 1 - g_texSetting.nWrap;
    float4 color = TEX_MAIN.Sample(WRAP, pin.uv * g_texSetting.tile + g_texSetting.offset) * g_texSetting.nWrap;
    color += TEX_MAIN.Sample(CRAMP, pin.uv * g_texSetting.tile + g_texSetting.offset) * nCramp;
    color *= g_texSetting.multipray;
    clip(color.a - 0.1f);
    
    
    return color;
}
