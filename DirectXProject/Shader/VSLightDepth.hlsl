struct VS_IN
{
    float3 pos : POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float3 binormal : BINORMAL0;
    uint inst : SV_InstanceID;
};
struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float4 depth : TEXCOORD1;
};

struct VP
{
    float4x4 view;
    float4x4 proj;
};

cbuffer ConstantBuffer0 : register(b0)
{
    float4x4 g_Worlds[1024];
};

cbuffer ConstantBuffer1 : register(b1)
{
    VP g_cameraVP[2];
};

cbuffer ConstantBuffer2 : register(b2)
{
    VP g_lightVP[2];
};

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;

    vout.pos = float4(vin.pos, 1);
    vout.pos = mul(vout.pos, g_Worlds[vin.inst]);
    vout.pos = mul(vout.pos, g_lightVP[0].view);
    vout.pos = mul(vout.pos, g_lightVP[0].proj);
    vout.depth = vout.pos;
    vout.uv = vin.uv;
    return vout;
}