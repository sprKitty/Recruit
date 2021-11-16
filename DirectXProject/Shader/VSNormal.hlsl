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
    float3 tangent : TEXCOORD2;
    float3 binormal : TEXCOORD3;
    float4 parallelLightPos : POSITION0;
    float4 wPos : POSITION3;
    float4 camPos : POSITION4;
};

cbuffer World : register(b0)
{
	float4x4 world;
};

cbuffer ViewProj : register(b1)
{
	float4x4 camView;
	float4x4 camProj;
    float4x4 parallelLightView;
    float4x4 parallellightProj;
};


VS_OUT main(VS_IN vin)
{
	VS_OUT vout;

    vout.pos = float4(vin.pos, 1);
    vout.pos = mul(vout.pos, world);
    vout.pos = mul(vout.pos, parallelLightView);
    vout.pos = mul(vout.pos, parallellightProj);
    vout.parallelLightPos = vout.pos;
    
    vout.pos = float4(vin.pos, 1);
    vout.pos = mul(vout.pos, world);
    vout.wPos = vout.pos;
    vout.pos = mul(vout.pos, camView);
    vout.pos = mul(vout.pos, camProj);
    vout.camPos = vout.pos;
    
	vout.uv = vin.uv;
    
    vout.normal = mul(vin.normal, (float3x3) world);
    vout.normal = normalize(vout.normal);
    
    vout.tangent = mul(vin.tangent, (float3x3) world);
    vout.tangent = normalize(vout.tangent);
    
    vout.binormal = mul(vin.binormal, (float3x3) world);
    vout.binormal = normalize(vout.binormal);
    
	return vout;
}