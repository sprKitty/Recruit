struct VS_IN
{
    float3 pos : POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float3 binormal : BINORMAL0;
    float4 weight : WEIGHT0;
    uint4 index : INDEX0;
};
struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float3 tangent : TEXCOORD2;
    float3 binormal : TEXCOORD3;
    float4 nearLightPos : POSITION0;
    float4 mediumLightPos : POSITION1;
    float4 farLightPos : POSITION2;
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
    float4x4 nearLightView;
    float4x4 mediumLightView;
    float4x4 farLightView;
    float4x4 nearlightProj;
    float4x4 mediumlightProj;
    float4x4 farlightProj;
    float4 lightPos;
};

cbuffer Animation : register(b2)
{
    float time;
    float3 dummy;
    float4x4 bone[250];
};

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;

    float4x4 anime =
		bone[vin.index.x] * vin.weight.x +
		bone[vin.index.y] * vin.weight.y +
		bone[vin.index.z] * vin.weight.z +
		bone[vin.index.w] * vin.weight.w;
 
	vout.pos = float4(vin.pos, 1);
    vout.pos = mul(vout.pos, anime);
    vout.pos = mul(vout.pos, world);
    vout.pos = mul(vout.pos, nearLightView);
    vout.pos = mul(vout.pos, nearlightProj);
    vout.nearLightPos = vout.pos;
    
	vout.pos = float4(vin.pos, 1);
    vout.pos = mul(vout.pos, anime);
    vout.pos = mul(vout.pos, world);
    vout.pos = mul(vout.pos, mediumLightView);
    vout.pos = mul(vout.pos, mediumlightProj);
    vout.mediumLightPos = vout.pos;
    
	vout.pos = float4(vin.pos, 1);
    vout.pos = mul(vout.pos, anime);
    vout.pos = mul(vout.pos, world);
    vout.pos = mul(vout.pos, farLightView);
    vout.pos = mul(vout.pos, farlightProj);
    vout.farLightPos = vout.pos;
    
    vout.pos = float4(vin.pos, 1);
	vout.pos = mul(vout.pos, anime);
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