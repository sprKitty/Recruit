struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float4 worldPos : TEXCOORD2;
    float4 camPos : TEXCOORD3;
    float4 reflectionCam : TEXCOORD4;
    float3 texSpaceLight : TEXCOORD5;
};

struct PS_OUT
{
    float4 main : SV_Target0;
    float4 emissive : SV_Target1;
    float4 depth : SV_Target2;
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
    float4 blur[8];
    float4 emissive;
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


PS_OUT main(PS_IN pin)
{
    PS_OUT pout;
    
    pout.emissive = g_postEffect.emissive;
    
    float4 color = 1.f;
    //float4 color = float4(0.8f, 0.8f, 0.8f, 1.f);
    
    float3 N[2];
    float height = TEX_WATERHEIGHT.Sample(WRAP, pin.uv).r;
    float3 L = pin.texSpaceLight * float3(1, 1, 1);
    float3 V = normalize(pin.worldPos.xyz - g_cameraInfo.pos.xyz);
    float2 texUV = pin.uv + .05f * height * V.xy;
    N[0] = normalize(TEX_WATERBUMP.Sample(WRAP, texUV + float2(-g_texSetting.fGameStartCnt * 0.0f, g_texSetting.fGameStartCnt * 0.5f)).rgb * 2.0f - 1.0f);
    N[1] = normalize(TEX_WATERBUMP.Sample(WRAP, texUV + float2(g_texSetting.fGameStartCnt * 0.0f, g_texSetting.fGameStartCnt * 0.2f)).rgb * 2.0f - 1.0f);
    float D = 1.f;
    for (int i = 0; i < 2; ++i)
    {
        float3 R = normalize(reflect(L, N[i]));
    
        float S = dot(V, R) * 0.5f + 0.5f;
        color.rgb *= pow(S, 64);

        float calcD = dot(N[i], L);
        calcD = (calcD + 1.0f) * 0.5f;
        D *= calcD;
    }
    
    float2 mapUV;
    mapUV.x = (1.0f + pin.reflectionCam.x / pin.reflectionCam.w) * 0.5f;
    mapUV.y = (1.0f - pin.reflectionCam.y / pin.reflectionCam.w) * 0.5f;
    float3 mapColor = TEX_WATER.Sample(CRAMP, mapUV).rgb;
    D = 1.f - D;
    pout.emissive = color;
    color.rgb += (float4(0.0f / 255.f, 100.f / 255.f, 150.f / 205.f, 1) * D);
    color.rgb += (mapColor);
    
    
    pout.main = color;
    pout.depth = pin.camPos.z / pin.camPos.w;
    
    return pout;
}
