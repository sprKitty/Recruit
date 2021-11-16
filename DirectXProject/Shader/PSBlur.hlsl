struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : TEXCOORD1;
};

cbuffer ConstantBuffer1 : register(b1)
{
    float4 vColor;
    int nEnable;
    int nWidth;
    int nHeight;
    int nDummy;
};

// CPUから渡されたテクスチャを利用するための宣言
Texture2D map : register(t0);
Texture2D ShadowMap : register(t1);
Texture2D WorldTex : register(t9);
Texture2D ScopeMap : register(t10);
Texture2D BlurMap : register(t11);

// テクスチャの繰り返し設定も必要
SamplerState samp : register(s0);
SamplerState samp1 : register(s1);
SamplerComparisonState samp2 : register(s2);

float4 main(PS_IN pin) : SV_Target
{
    float2 offset = float2(4.0f / nWidth, 4.0f / nHeight);
    float4 color = BlurMap.Sample(samp, pin.uv);
    
    color += BlurMap.Sample(samp, pin.uv + float2(offset.x, 0) * nEnable);
    color += BlurMap.Sample(samp, pin.uv + float2(-offset.x, 0) * nEnable);
    color += BlurMap.Sample(samp, pin.uv + float2(0, offset.y) * nEnable);
    color += BlurMap.Sample(samp, pin.uv + float2(0, -offset.y) * nEnable);
    color += BlurMap.Sample(samp, pin.uv + float2(offset.x, offset.y) * nEnable);
    color += BlurMap.Sample(samp, pin.uv + float2(offset.x, -offset.y) * nEnable);
    color += BlurMap.Sample(samp, pin.uv + float2(-offset.x, offset.y) * nEnable);
    color += BlurMap.Sample(samp, pin.uv + float2(-offset.x, -offset.y) * nEnable);
    color += BlurMap.Sample(samp, pin.uv + float2(offset.x * 2, 0) * nEnable);
    color += BlurMap.Sample(samp, pin.uv + float2(-offset.x * 2, 0) * nEnable);
    color += BlurMap.Sample(samp, pin.uv + float2(0, offset.y * 2) * nEnable);
    color += BlurMap.Sample(samp, pin.uv + float2(0, -offset.y * 2) * nEnable);
    color += BlurMap.Sample(samp, pin.uv + float2(offset.x * 2, offset.y * 2) * nEnable);
    color += BlurMap.Sample(samp, pin.uv + float2(offset.x * 2, -offset.y * 2) * nEnable);
    color += BlurMap.Sample(samp, pin.uv + float2(-offset.x * 2, offset.y * 2) * nEnable);
    color += BlurMap.Sample(samp, pin.uv + float2(-offset.x * 2, -offset.y * 2) * nEnable);
	color += BlurMap.Sample(samp, pin.uv + float2(offset.x * 3, 0) * nEnable);
    color += BlurMap.Sample(samp, pin.uv + float2(-offset.x * 3, 0) * nEnable);
    color += BlurMap.Sample(samp, pin.uv + float2(0, offset.y * 3) * nEnable);
    color += BlurMap.Sample(samp, pin.uv + float2(0, -offset.y * 3) * nEnable);
    color += BlurMap.Sample(samp, pin.uv + float2(offset.x * 3, offset.y * 3) * nEnable);
    color += BlurMap.Sample(samp, pin.uv + float2(offset.x * 3, -offset.y * 3) * nEnable);
    color += BlurMap.Sample(samp, pin.uv + float2(-offset.x * 3, offset.y * 3) * nEnable);
    color += BlurMap.Sample(samp, pin.uv + float2(-offset.x * 3, -offset.y * 3) * nEnable);
    color /= 25;
    clip(color.a - 0.1f);
    
    return color;
}