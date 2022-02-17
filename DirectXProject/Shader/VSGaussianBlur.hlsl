struct VS_IN
{
    float3 pos : POSITION0;
    float2 uv : TEXCOORD0;
    float3 normal : NORMAL0;
    float3 tangent : TANGENT0;
    float3 ambient : AMBIENT0;
    float3 diffuse : DIFFUSE0;
    float3 specular : SPECULAR0;
    uint specularIndex : BLENDINDICES0;
    uint inst : SV_InstanceID;
};
struct VS_OUT
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
};

struct VP
{
    float4x4 view;
    float4x4 proj;
};

struct LightInfo
{
    float4 pos;
    float4 dir;
    float4 decay;
    float4 color;
};

cbuffer ConstantBuffer0 : register(b0)
{
    float4x4 g_Worlds[1024];
};

cbuffer ConstantBuffer1 : register(b1)
{
    VP g_cameraVP;
};

cbuffer ConstantBuffer2 : register(b2)
{
    VP g_lightVP;
};

cbuffer ConstantBuffer3 : register(b3)
{
    LightInfo g_lightInfo;
};


VS_OUT main(VS_IN vin)
{
    VS_OUT vout;

    vout.pos = float4(vin.pos, 1);
    vout.pos = mul(vout.pos, g_Worlds[vin.inst]);
    vout.pos = mul(vout.pos, g_cameraVP.view);
    vout.pos = mul(vout.pos, g_cameraVP.proj);
    
    vout.uv = vin.uv;
    
    return vout;
}