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
};

cbuffer Animation : register(b2)
{
    float time;
    float3 dummy;
    float4x4 bone[250];
};

// ゲルストナー波
void gerstnerWave(in float3 localVtx, float t, float waveLen, float Q, float R, float2 browDir, inout float3 localVtxPos, inout float3 localNormal)
{

    browDir = normalize(browDir);
    const float pi = 3.1415926535f;
    const float grav = 9.8f;
    float A = waveLen / 14.0f;
    float _2pi_per_L = 2.0f * pi / waveLen;
    float d = dot(browDir, localVtx.xz);
    float th = _2pi_per_L * d + sqrt(grav / _2pi_per_L) * t;

    float3 pos = float3(0.0, R * A * sin(th), 0.0);
    pos.xz = Q * A * browDir * cos(th);

    // ゲルストナー波の法線
    float3 normal = float3(0.0, 1.0, 0.0);
    normal.xz = -browDir * R * cos(th) / (7.0f / pi - Q * browDir * browDir * sin(th));

    localVtxPos += pos;
    localNormal += normal;
}

VS_OUT main(VS_IN vin)
{
    VS_OUT vout;

    vout.pos = float4(vin.pos, 1);
    float3 oPosW = float3(0.0, 0.0, 0.0);
    float3 oNormalW = float3(0.0, 0.0, 0.0);
    
    vout.pos = float4(vin.pos, 1);
 
    gerstnerWave(vout.pos.xyz, time + 2.0, 0.3, 0.8, 0.5, float2(0.2, 0.3), oPosW, oNormalW);
    gerstnerWave(vout.pos.xyz, time,       0.7, 0.8, 0.5, float2(-0.4, 0.7), oPosW, oNormalW);
    gerstnerWave(vout.pos.xyz, time + 3.0, 0.8, 0.8, 0.5, float2(-0.4, -0.4), oPosW, oNormalW);
    gerstnerWave(vout.pos.xyz, time,       1.2, 0.8, 0.5, float2(-0.3, 0.6), oPosW, oNormalW);
 
    vout.pos.xyz += oPosW;
    vout.pos = mul(vout.pos, world);
    
    vout.wPos = vout.pos;
    vout.pos = mul(vout.pos, camView);
    vout.pos = mul(vout.pos, camProj);
    vout.camPos = vout.pos;
    
    vout.uv = vin.uv;
    vout.normal = mul(oNormalW, (float3x3) world);

    return vout;
}