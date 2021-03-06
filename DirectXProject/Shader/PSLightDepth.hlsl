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


PS_OUT main(PS_IN pin)
{
    PS_OUT pout;
    int nCramp = 1 - g_texSetting.nWrap;
    float fAlpha = TEX_MAIN.Sample(WRAP, pin.uv * g_texSetting.tile + g_texSetting.offset).a * g_texSetting.nWrap;
    fAlpha += TEX_MAIN.Sample(CRAMP, pin.uv * g_texSetting.tile + g_texSetting.offset).a * nCramp;
    clip(fAlpha - 0.1f);
    
    float depth = pin.depth.z / pin.depth.w;
    float4 unpacked_depth = float4(0, 0, 256.0f, 256.0f);
    unpacked_depth.g = modf(depth * 256.0f, unpacked_depth.r);
    unpacked_depth.b *= modf(unpacked_depth.g * 256.0f, unpacked_depth.g);

    pout.target0 = unpacked_depth / 256.0f;
    return pout;
}