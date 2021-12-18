struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float4 wPos : TEXCOORD2;
    float4 lightPos : TEXCOORD3;
    float4 camPos : TEXCOORD4;
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
};

struct TexSetting
{
    float2 tile;
    float2 offset;
    float4 multipray;
    int nWrap;
    float fAlpha;
    float fTime;
    float fDummy;
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

Texture2D TEX_MAIN : register(t0);
Texture2D TEX_SCREEN : register(t1);
Texture2D TEX_DOS : register(t2);
Texture2D TEX_DOF : register(t3);
Texture2D TEX_BUMP : register(t4);
Texture2D TEX_CLIP : register(t5);
Texture2D TEX_BLUR : register(t6);
Texture2D TEX_GRAYSCALE : register(t7);

SamplerState WRAP : register(s0);
SamplerState CRAMP : register(s1);
SamplerState BORDER : register(s2);
SamplerComparisonState samp3 : register(s3);
SamplerState MIRROR : register(s4);



float4 main(PS_IN pin) : SV_Target
{
    float4 color = 1.0f;
    float3 blending = abs(pin.normal);
    blending = normalize(max(blending, 0.001f));
    float b = blending.x + blending.y + blending.z;
    blending /= b;
    
    float3 xaxis = TEX_MAIN.Sample(WRAP, pin.wPos.zy * 0.2f).rgb;
    float3 yaxis = TEX_MAIN.Sample(WRAP, pin.wPos.xz * 0.2f).rgb;
    float3 zaxis = TEX_MAIN.Sample(WRAP, pin.wPos.xy * 0.2f).rgb;
    
    color.rgb = xaxis * blending.x + yaxis * blending.y + zaxis * blending.z;

    float3 xN = TEX_BUMP.Sample(WRAP, pin.wPos.zy * 0.2f).rgb * 2.0f - 1.0f;
    float3 yN = TEX_BUMP.Sample(WRAP, pin.wPos.xz * 0.2f).rgb * 2.0f - 1.0f;
    float3 zN = TEX_BUMP.Sample(WRAP, pin.wPos.xy * 0.2f).rgb * 2.0f - 1.0f;
    
    xN = (dot(xN, pin.texSpaceLight) + 1.0f) * 0.5f;
    yN = (dot(yN, pin.texSpaceLight) + 1.0f) * 0.5f;
    zN = (dot(zN, pin.texSpaceLight) + 1.0f) * 0.5f;
    
    color.rgb *= xN * blending.x + yN * blending.y + zN * blending.z;
    
    float3 L = -normalize(g_lightInfo.dir.xyz);
    float3 N = normalize(pin.normal.xyz);
    float I = dot(L, N);
    I += 1.0f;
    I *= 0.5f;
    color.rgb *= (I * g_lightInfo.color.rgb);
    
    float2 mapUV;
    float inLVP;
    float depth = 0;
    inLVP = pin.lightPos.z / pin.lightPos.w;
    mapUV.x = (1.0f + pin.lightPos.x / pin.lightPos.w) * 0.5f;
    mapUV.y = (1.0f - pin.lightPos.y / pin.lightPos.w) * 0.5f;
    float3 mapColor = TEX_DOS.Sample(BORDER, mapUV).rgb;
    depth = mapColor.r;
    depth += mapColor.g / 256.0f; 
    depth += mapColor.b / 256.0f / 256.0f;
    if (inLVP - 0.00015f > depth)
    {
        color.rgb *= 0.4f;
    }
    
    return color;
}
