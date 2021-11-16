struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : TEXCOORD1;
};

// CPUから渡されたテクスチャを利用するための宣言
Texture2D tex : register(t0);
Texture2D ShadowMap : register(t1);
Texture2D WorldTex : register(t9);
Texture2D ScopeMap : register(t10);

// テクスチャの繰り返し設定も必要
SamplerState samp : register(s0);
SamplerState samp1 : register(s1);
SamplerComparisonState samp2 : register(s2);

float4 main(PS_IN pin) : SV_Target
{
    float4 color = WorldTex.Sample(samp, pin.uv);
    float4 colorS = ScopeMap.Sample(samp, pin.uv);
    
    color.a = colorS.a;
    clip(color.a - 0.1f);
    return color;
}