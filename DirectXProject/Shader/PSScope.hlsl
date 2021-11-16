struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : TEXCOORD1;
};

// CPU����n���ꂽ�e�N�X�`���𗘗p���邽�߂̐錾
Texture2D tex : register(t0);
Texture2D ShadowMap : register(t1);
Texture2D WorldTex : register(t9);
Texture2D ScopeMap : register(t10);

// �e�N�X�`���̌J��Ԃ��ݒ���K�v
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