struct VS_IN
{
    float3 pos : POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float3 binormal : BINORMAL0;
};
struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float4 lightvPos : POSITION0;
    float4 wPos : POSITION1;
};
cbuffer World : register(b0)
{
    float4x4 world;
};

cbuffer ViewProj : register(b1)
{
    float4x4 camView;
    float4x4 camProj;
    float4x4 sunView;
    float4x4 sunProj;
};

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;
    
    vout.pos = float4(vin.pos, 1);
    vout.pos = mul(vout.pos, world);
    vout.pos = mul(vout.pos, sunView);
    vout.pos = mul(vout.pos, sunProj);
    vout.lightvPos = vout.pos;
    vout.pos = float4(vin.pos, 1);

    vout.uv = vin.uv;
    vout.normal = mul(vin.normal, (float3x3) world);
    
    return vout;
}