struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 depth : TEXCOORD2;
    float4 wPos : POSITION1;
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
    Light light[3];
};

float4 main(PS_IN pin) : SV_TARGET
{
    float4 color;
    color = pin.depth.z / pin.depth.w;
    return color;
}