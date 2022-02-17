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
    float3 normal : TEXCOORD1;
    float4 worldPos : TEXCOORD2;
    float4 camPos : TEXCOORD3;
    float4 reflectionCam : TEXCOORD4;
    float3 texSpaceLight : TEXCOORD5;
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
    VP g_cameraVP[2];
};

cbuffer ConstantBuffer2 : register(b2)
{
    VP g_lightVP[2];
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
    vout.worldPos = vout.pos;
    
    vout.pos = mul(vout.worldPos, g_cameraVP[1].view);
    vout.pos = mul(vout.pos, g_cameraVP[1].proj);
    vout.reflectionCam = vout.pos;
    
    vout.pos = mul(vout.worldPos, g_cameraVP[0].view);
    vout.pos = mul(vout.pos, g_cameraVP[0].proj);
    vout.camPos = vout.pos;
    
    vout.uv = vin.uv;
    
    float3 N = normalize(mul(vin.normal, (float3x3) g_Worlds[vin.inst]));
    vout.normal = N;
    float3 T = normalize(mul(vin.tangent, (float3x3) g_Worlds[vin.inst]));
    float3 B = normalize(mul(vin.binormal, (float3x3) g_Worlds[vin.inst]));
 
    float3x3 invTexToWorld = float3x3(
    T.x, B.x, N.x,
    T.y, B.y, N.y,
    T.z, B.z, N.z
    );
    
    float3 L = normalize(g_lightInfo.dir.xyz);
    vout.texSpaceLight = normalize(mul(-L, invTexToWorld));
    
    return vout;
}