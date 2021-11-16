struct PS_IN
{
    float4 pos : SV_POSITION;
    float4 depth : POSITION0;
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

float4 main(PS_IN pin):SV_Target0
{
    float depth = pin.depth.z / pin.depth.w;
    float4 unpacked_depth = float4(0, 0, 256.0f, 256.0f);
    unpacked_depth.g = modf(depth * 256.0f, unpacked_depth.r);
    unpacked_depth.b *= modf(unpacked_depth.g * 256.0f, unpacked_depth.g);

    return unpacked_depth / 256.0f; // ïWèÄâª
}