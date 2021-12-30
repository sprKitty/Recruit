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
    float fDummy;
};

struct PostEffect
{
    float4 blur[8];
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
    float4 color = 0;
    float2 size = g_cameraInfo.vpSize.xy;
    
    
    color += g_postEffect.blur[0].z * TEX_DOF.Sample(CRAMP, pin.uv + float2(g_postEffect.blur[0].x, g_postEffect.blur[0].y));
    color += g_postEffect.blur[1].z * TEX_DOF.Sample(CRAMP, pin.uv + float2(g_postEffect.blur[1].x, g_postEffect.blur[1].y));
    color += g_postEffect.blur[2].z * TEX_DOF.Sample(CRAMP, pin.uv + float2(g_postEffect.blur[2].x, g_postEffect.blur[2].y));
    color += g_postEffect.blur[3].z * TEX_DOF.Sample(CRAMP, pin.uv + float2(g_postEffect.blur[3].x, g_postEffect.blur[3].y));
    color += g_postEffect.blur[4].z * TEX_DOF.Sample(CRAMP, pin.uv + float2(g_postEffect.blur[4].x, g_postEffect.blur[4].y));
    color += g_postEffect.blur[5].z * TEX_DOF.Sample(CRAMP, pin.uv + float2(g_postEffect.blur[5].x, g_postEffect.blur[5].y));
    color += g_postEffect.blur[6].z * TEX_DOF.Sample(CRAMP, pin.uv + float2(g_postEffect.blur[6].x, g_postEffect.blur[6].y));
    color += g_postEffect.blur[7].z * TEX_DOF.Sample(CRAMP, pin.uv + float2(g_postEffect.blur[7].x, g_postEffect.blur[7].y));
    color += g_postEffect.blur[1].z * TEX_DOF.Sample(CRAMP, pin.uv - float2(g_postEffect.blur[1].x, g_postEffect.blur[1].y));
    color += g_postEffect.blur[2].z * TEX_DOF.Sample(CRAMP, pin.uv - float2(g_postEffect.blur[2].x, g_postEffect.blur[2].y));
    color += g_postEffect.blur[3].z * TEX_DOF.Sample(CRAMP, pin.uv - float2(g_postEffect.blur[3].x, g_postEffect.blur[3].y));
    color += g_postEffect.blur[4].z * TEX_DOF.Sample(CRAMP, pin.uv - float2(g_postEffect.blur[4].x, g_postEffect.blur[4].y));
    color += g_postEffect.blur[5].z * TEX_DOF.Sample(CRAMP, pin.uv - float2(g_postEffect.blur[5].x, g_postEffect.blur[5].y));
    color += g_postEffect.blur[6].z * TEX_DOF.Sample(CRAMP, pin.uv - float2(g_postEffect.blur[6].x, g_postEffect.blur[6].y));
    color += g_postEffect.blur[7].z * TEX_DOF.Sample(CRAMP, pin.uv - float2(g_postEffect.blur[7].x, g_postEffect.blur[7].y));
    
    clip(color.a - 0.1f);
    color.a = 1;
    return color;
}
