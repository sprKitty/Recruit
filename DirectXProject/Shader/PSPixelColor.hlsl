struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float3 tangent : TEXCOORD2;
    float3 binormal : TEXCOORD3;
    float4 parallelLightPos : POSITION0;
    float4 wPos : POSITION3;
    float4 camPos : POSITION4;
};

struct LightInfo
{
    float4 pos;
    float3 dir;
    float uvOffset;
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


// CPUから渡されたテクスチャを利用するための宣言
Texture2D tex : register(t0);
// テクスチャの繰り返し設定も必要
SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_Target
{
	return 1.0f;
}