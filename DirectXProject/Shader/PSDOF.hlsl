struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 depth : TEXCOORD1;
};

struct PS_OUT
{
    float4 target0 : SV_Target0;
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


float4 main(PS_IN pin) : SV_Target0
{
    int nCramp = 1 - g_texSetting.nWrap;
    float fAlpha = TEX_MAIN.Sample(WRAP, pin.uv * g_texSetting.tile + g_texSetting.offset).a * g_texSetting.nWrap;
    fAlpha += TEX_MAIN.Sample(CRAMP, pin.uv * g_texSetting.tile + g_texSetting.offset).a * nCramp;
    clip(fAlpha - 0.1f);
    
    float depth = pin.depth.z / 80.0f;
    float4 color = { 0, 0, 0, 1 };
    color.rgb = 2 * (0.95f - depth);
    depth = max(0, depth);
    depth = min(1, depth);
    color = pow(color, 10);
    
    //color.rgb = 2.0f * (depth + 0.23f);
    //depth = max(0, depth);
    //depth = min(1, depth);
    //color = pow(color, 10);
    return color;
}