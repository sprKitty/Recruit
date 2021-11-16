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
    float4 depth : TEXCOORD2;
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
    float4x4 nearLightView;
    float4x4 mediumLightView;
    float4x4 farLightView;
    float4x4 nearlightProj;
    float4x4 mediumlightProj;
    float4x4 farlightProj;
};

//cbuffer Sun : register(b2)
//{
//    float4x4 Sunview;
//};

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;

    vout.pos = float4(vin.pos, 1);
    
    vout.pos = mul(vout.pos, world);
    vout.wPos = vout.pos;
    
    vout.pos = mul(vout.pos, camView);
    
    vout.pos = mul(vout.pos, camProj);
    vout.depth = vout.pos;

    return vout;
}