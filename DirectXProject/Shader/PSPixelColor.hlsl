struct PS_IN
{
    float4 pos : SV_POSITION;
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

cbuffer ConstantBuffer : register(b0)
{
    CameraInfo cameraInfo;
    LightInfo parallelLightInfo;
};

cbuffer ConstantBuffer1 : register(b1)
{
    float4 vColor;
    int nEnable;
    int nWidth;
    int nHeight;
    int nDummy;
};

cbuffer ConstantBuffer2 : register(b2)
{
    float2 scale;
    float2 offset;
    float4 multiply;
    int nWrap;
    float fAlpha;
    float fTime;
    float fDummy;
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


// CPUから渡されたテクスチャを利用するための宣言
Texture2D tex : register(t0);
// テクスチャの繰り返し設定も必要
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_Target
{
    return float4(0, 0, 1, 0.4f);
}