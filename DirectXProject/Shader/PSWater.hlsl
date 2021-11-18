struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : TEXCOORD1;
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

cbuffer ConstantBuffer2 : register(b2)
{
    float2 scale;
    float2 offset;
    float4 multiply;
    int nWrap;
    float alpha;
    float fTime;
    float fDummy;
}

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
Texture2D TEX_WATERDEPTH  : register(t14);
Texture2D TEX_TERRAINDEPTH : register(t15);
Texture2D TEX_WORLD : register(t17);

SamplerState WRAP : register(s0);
SamplerState CRAMP : register(s1);
SamplerState BORDER : register(s2);
SamplerComparisonState samp3 : register(s3);
SamplerState MIRROR : register(s4);


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
    float3 normal = float3(0.0, 0.7f, 0.0);
    normal.xz = -browDir * R * cos(th) / (7.0f / pi - Q * browDir * browDir * sin(th));

    localVtxPos += pos;
    localNormal += normalize(normal);
}

// フレネル反射率を算出
float fresnel(in float3 toCameraDirW, in float3 normalW, in float n1, in float n2)
{
    float A = n1 / n2;
    float B = dot(toCameraDirW, normalW);
    float C = sqrt(1.0 - A * A * (1.0 - B * B));
    float V1 = (A * B - C) / (A * B + C);
    float V2 = (A * C - B) / (A * C + B);
    return (V1 * V1 + V2 * V2) * 0.5;
}

float4 main(PS_IN pin) : SV_TARGET
{
    float2 mapUV;
    
    float4 color = float4(0.2f, 0.5f, 0.9f, 1.0f);
    mapUV.x = (1.0f + pin.camPos.x / pin.camPos.w) * 0.5f;
    mapUV.y = (1.0f - pin.camPos.y / pin.camPos.w) * 0.5f;
    float alpha = TEX_TERRAINDEPTH.Sample(BORDER, mapUV).z;
    alpha = 1.0f * alpha - 0.05f;
    //if(alpha > 1.0f)
        //alpha * 0.5f;
    float3 normalW = normalize(pin.normal);
    float3 toLightDirW = normalize(-light[2].dir);
    float3 waterColor = float3(0.5f, 0.8f, 0.9f);
    float3 fromVtxToCameraW = normalize(camera.pos.xyz - pin.wPos.xyz);
    float R = fresnel(fromVtxToCameraW, normalW, 1.000292f, 1.3334f);
    
    // 環境マップサンプル
    float3 reflectDirW = reflect(-fromVtxToCameraW, normalW);
    //float3 envColor = waterColor * 1.2f;
    float3 envColor = TEX_TERRAINDEPTH.Sample(BORDER, mapUV).z;
    
    // ディフューズ色
    // 水の色と環境マップを合成 → ライトとの角度で陰影
    float3 srcColor = waterColor * (1.0f - R) + envColor * R;
    float diffusePower = dot(normalW, toLightDirW);
    float3 diffuseColor = srcColor * diffusePower;
    
    // スペキュラ
    float3 halfDirW = normalize(toLightDirW + fromVtxToCameraW);
    float3 specularColor = pow(max(0.0f, dot(halfDirW, normalW)), 30.0f);
    float3 col = diffuseColor + specularColor * 0.75f;
    color.rgb *= col;
    color.a = alpha;
    return color;
}