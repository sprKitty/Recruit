struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float4 lightvPos : POSITION0;
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
    Light light;
};
Texture2D tex : register(t0);
Texture2D ShadowMap : register(t1);

SamplerState samp : register(s0);

float4 main(PS_IN pin) : SV_Target
{
    float4 color = tex.Sample(samp, pin.uv);
    
    float2 tex;
    tex.x = (1.0f + pin.lightvPos.x / pin.lightvPos.w) * 0.5f;
    tex.y = (1.0f - pin.lightvPos.y / pin.lightvPos.w) * 0.5f;
    float4 mapColor = ShadowMap.Sample(samp, tex);
    
    if (pow(pin.lightvPos.z / pin.lightvPos.w, light.OnOff.y) > mapColor.x + light.OnOff.z)
        color.rgb *= 0.5f;
    return color;
}