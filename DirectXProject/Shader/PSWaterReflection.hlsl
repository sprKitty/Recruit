struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float4 worldPos : TEXCOORD2;
    float4 lightPos : TEXCOORD3;
    float4 reflectionCam : TEXCOORD4;
    float3 texSpaceLight : TEXCOORD5;
};

struct LightInfo
{
    float4 pos;
    float4 dir;
    float4 decay;
    float4 color;
};

struct CameraInfo
{
    float4 pos;
    float4 vpSize;
};

struct TexSetting
{
    float2 tile;
    float2 offset;
    float4 multipray;
    int nWrap;
    float fAlpha;
    float fTime;
    float fGameStartCnt;
};

struct PostEffect
{
    int4 blur;
};
cbuffer ConstantBuffer0 : register(b0)
{
    CameraInfo g_cameraInfo;
};

cbuffer ConstantBuffer1 : register(b1)
{
    LightInfo g_lightInfo;
};

cbuffer ConstantBuffer2 : register(b2)
{
    TexSetting g_texSetting;
}

cbuffer ConstantBuffer3 : register(b3)
{
    PostEffect g_postEffect;
}

Texture2D TEX_MAIN : register(t0);
Texture2D TEX_WATER : register(t1);
Texture2D TEX_WATERHEIGHT : register(t2);
Texture2D TEX_WATERBUMP : register(t3);
Texture2D TEX_DOS : register(t4);
Texture2D TEX_DOF : register(t5);
Texture2D TEX_BUMP : register(t6);
Texture2D TEX_GRAYSCALE : register(t7);
Texture2D TEX_BLURX : register(t8);
Texture2D TEX_BLURY : register(t9);
Texture2D TEX_KAWASE1 : register(t10);
Texture2D TEX_KAWASE2 : register(t11);
Texture2D TEX_KAWASE3 : register(t12);
Texture2D TEX_KAWASE4 : register(t13);
Texture2D TEX_EFFECT : register(t14);

SamplerState WRAP : register(s0);
SamplerState CRAMP : register(s1);
SamplerState BORDER : register(s2);
SamplerComparisonState samp3 : register(s3);
SamplerState MIRROR : register(s4);


float4 main(PS_IN pin) : SV_Target
{
    float4 color = 1;
    
    float3 EyeVec = normalize(pin.worldPos.xyz - g_cameraInfo.pos.xyz);
    float3 LightVec = normalize(pin.worldPos.xyz - g_lightInfo.pos.xyz); //光線ベクトル
    float height = TEX_WATERHEIGHT.Sample(WRAP, pin.uv).r;
    float2 texUV = pin.uv + .2f * height * EyeVec.xy + g_texSetting.fGameStartCnt * 0.1f;
    float3 bumpTex = TEX_WATERBUMP.Sample(WRAP, texUV).rgb * 2.0f - 1.0f;
    float3 N = (dot(bumpTex, pin.texSpaceLight) + 1.0f) * 0.5f;
    float3 R = reflect(EyeVec, N);
    float3 RL = max(dot(R, LightVec),0.f);
    float3 lightColor = g_lightInfo.color.rgb;
    float waterTopAddPower = 1.0f;
    float waterTopAddColor = lightColor * pow(1.f - abs(dot(EyeVec, N)), 10.f) * waterTopAddPower;

    float3 spe = { 2.0f, 1.8f, 1.2f }; // 反射色
    
    float2 mapUV;
    mapUV.x = (1.0f + pin.reflectionCam.x / pin.reflectionCam.w) * 0.5f;
    mapUV.y = (1.0f - pin.reflectionCam.y / pin.reflectionCam.w) * 0.5f;
    float3 mapColor = TEX_WATER.Sample(CRAMP, mapUV).rgb;
    color.rgb = mapColor;
    color.rgb += (waterTopAddColor * TEX_WATERHEIGHT.Sample(WRAP, texUV).r);
    color.rgb += (spe * pow(RL, 64));
    color.rgb *= N;
    color.a = 0.85f;
    
    return color;
}
