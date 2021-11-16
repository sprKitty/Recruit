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

cbuffer ConstantBuffer1 : register(b2)
{
    float2 scale;
    float2 offset;
    float4 multiply;
    int nWrap;
    float alpha;
    float2 dummy;
}

// CPUから渡されたテクスチャを利用するための宣言
Texture2D tex : register(t0);
Texture2D ShadowMap : register(t1);
Texture2D WorldTex : register(t9);
Texture2D ClipMap : register(t10);
Texture2D GrayScale : register(t12);

// テクスチャの繰り返し設定も必要
SamplerState wrap : register(s0);
SamplerState cramp : register(s1);
SamplerComparisonState samp2 : register(s2);

float4 main(PS_IN pin) : SV_Target
{
    float4 color = float4(0.0f, 0.0f, 0.0f, 1.0f);
    float4 grayScale = GrayScale.Sample(wrap, pin.uv);
    
    clip(grayScale.r - alpha);
    
    return color;
}