struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float4 wPos : POSITION0;
    float4 pPos : POSITION1;
};

struct Material
{
    float4 ambient;
};

struct Light
{
    float4 OnOff;
    float4 pos;
    float4 dir;
    float4 decay;
    float4 color;
    Material mat;
};

cbuffer ConstantBuffer : register(b0)
{
    float4 view;
    Light light;
};

cbuffer ConstantBuffer1 : register(b1)
{
    float4 vColor;
    int nEnable;
    int nWidth;
    int nHeight;
    int nDummy;
};

Texture2D tex : register(t0);
SamplerState samp : register(s0);
SamplerState samp1 : register(s1);

float4 main(PS_IN pin) : SV_TARGET
{
    return vColor;
}