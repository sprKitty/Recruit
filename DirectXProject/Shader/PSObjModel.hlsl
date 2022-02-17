struct PS_IN
{
    float4 pos : SV_POSITION;
    float2 uv : TEXCOORD0;
    float3 normal : TEXCOORD1;
    float4 worldPos : TEXCOORD2;
    float4 lightPos : TEXCOORD3;
    float4 camPos : TEXCOORD4;
    float3 texSpaceLight : TEXCOORD5;
    float4 ambient : COLOR0;
    float4 diffuse : COLOR1;
    float4 specular : COLOR2;
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
    float4 vp;
    float4 dir;
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

inline float V_SmithGGXCorrelated(float ndotl, float ndotv, float alpha)
{
    float lambdaV = ndotl * (ndotv * (1 - alpha) + alpha);
    float lambdaL = ndotv * (ndotl * (1 - alpha) + alpha);
    return 0.5f / (lambdaV + lambdaL + 0.0001);
}

inline float3 F_Schlick(float3 f0, float cos,int specularIndex)
{
    return f0 + (1 - f0) * pow(1 - cos, specularIndex);
}

inline float D_GGX(half perceptualRoughness, half ndoth, half3 normal, half3 halfDir)
{
    half3 ncrossh = cross(normal, halfDir);
    half a = ndoth * perceptualRoughness;
    half k = perceptualRoughness / (dot(ncrossh, ncrossh) + a * a);
    half d = k * k * 1.f / 3.141592653589f;
    return min(d, 65504.0);
}

inline half Fd_Burley(half ndotv, half ndotl, half ldoth, half roughness)
{
    half fd90 = 0.5 + 2 * ldoth * ldoth * roughness;
    half lightScatter = (1 + (fd90 - 1) * pow(1 - ndotl, 5));
    half viewScatter = (1 + (fd90 - 1) * pow(1 - ndotv, 5));

    half diffuse = lightScatter * viewScatter;
    // 本来はこのDiffuseをπで割るべきだけどUnityではレガシーなライティングとの互換性を保つため割らない
    //diffuse /= UNITY_PI;
    return diffuse;
}

PS_OUT main(PS_IN pin)
{
    PS_OUT pout;
    pout.main = pin.ambient;
    
    // 物理ベースレンダリング DiffuseとSpecular反射
    // カメラのベクトル
    float3 V = normalize(-g_cameraInfo.dir.xyz);
    float3 L = normalize(-g_lightInfo.dir.xyz);
    float3 N = normalize(pin.normal);
    float3 H = normalize(L + V);
    
    float metalic = 0.1f;
    float perceptualRoughness = 1.f;
    float NV = abs(dot(N, V)); // 法線と視線ベクトルの内積
    float NL = max(0, dot(N, L)); // 法線とライト方向の内積
    float NH = max(0, dot(N, H)); // 法線をハーフベクトルの内積
    float LH = max(0, dot(L, H)); // ハーフベクトルとライト方向の内積
    float reflect = lerp(0.04f, 1, metalic);
    float3 f0 = lerp(0.04f, pin.diffuse.rgb, metalic);
    
    half diffuseTerm = Fd_Burley(NV, NL, LH, perceptualRoughness) * NL;
    half3 diffuse = pin.diffuse.rgb * (1 - reflect) * g_lightInfo.color.rgb * diffuseTerm;
    
    float alpha = perceptualRoughness * perceptualRoughness;
    V = V_SmithGGXCorrelated(NL, NV, alpha);
    float D = D_GGX(perceptualRoughness, NV, N, H);
    float3 F = F_Schlick(f0, LH, pin.specular.w);
    float3 specular = V * D * F * NL * pin.specular.rgb * g_lightInfo.color.rgb;
    specular *= 3.141592653589f;
    specular = max(0, specular);
    
    pout.main.rgb = diffuse + specular;
    
    // シャドウマッピング
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
    
    pout.main.rgb = (inLVP > depth + g_lightInfo.pos.w) ? pout.main.rgb * 0.5f : pout.main.rgb;
    pout.emissive = g_postEffect.emissive;
    pout.depth = pin.camPos.z / pin.camPos.w;
    
    //pout.main = pin.diffuse;
    return pout;
}
