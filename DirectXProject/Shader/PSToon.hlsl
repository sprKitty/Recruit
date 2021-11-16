struct PS_IN
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

struct Material
{
    float4 ambient;
};

struct Light
{
    float4 OnOff;
    float4 pos;
    float3 dir;
    float uvOffset;
    float4 decay;
    float4 color;
    Material mat;
};

struct Camera
{
    float4 pos;
    float4 look;
};

cbuffer ConstantBuffer : register(b0)
{
    Camera camera;
    Light light[3];
};

cbuffer ConstantBuffer1 : register(b1)
{
    float4 vColor;
    int nEnable;
    int nWidth;
    int nHeight;
    int nDummy;
};

Texture2D TEX_MAIN : register(t0);
Texture2D TEX_SCREEN : register(t1);
Texture2D TEX_NEARSHADOW : register(t2);
Texture2D TEX_MEDIUMSHADOW : register(t3);
Texture2D TEX_FARSHADOW : register(t4);
Texture2D TEX_TRILANAR_X : register(t5);
Texture2D TEX_TRILANAR_Y : register(t6);
Texture2D TEX_TRILANAR_Z : register(t7);
Texture2D TEX_BUMP_X : register(t8);
Texture2D TEX_BUMP_Y : register(t9);
Texture2D TEX_BUMP_Z : register(t10);
Texture2D TEX_CLIP : register(t11);
Texture2D TEX_BLUR : register(t12);
Texture2D TEX_GRAYSCALE : register(t13);
Texture2D TEX_WATERDEPTH : register(t14);
Texture2D TEX_TERRAINDEPTH : register(t15);
Texture2D TEX_TOONMAP : register(t16);

SamplerState WRAP : register(s0);
SamplerState CRAMP : register(s1);
SamplerState BORDER : register(s2);
SamplerComparisonState samp3 : register(s3);
SamplerState MIRROR : register(s4);


float4 main(PS_IN pin) : SV_TARGET
{
    float4 color = TEX_MAIN.Sample(WRAP, pin.uv);
    
    float3 dir = normalize(-light[0].dir.xyz);
    
    float p = dot(normalize(pin.normal), dir);
    p = p * 0.5f + 0.7f;
    p = p * p;
    float3 eyeVec = camera.pos.xyz - pin.wPos.xyz;
    float re = dot(normalize(pin.normal), normalize(eyeVec));
    re = re * 0.5f + 0.5f;
    re = re * re;
    float3 red = float3(0.0f, 0, 0);
    if(re < 0.38f)
    {
        color.rgb = red;
    }
    
    float4 col = TEX_TOONMAP.Sample(CRAMP, float2(p, 0.0f));
    color.rgb *= col.rgb;
    return color;
}