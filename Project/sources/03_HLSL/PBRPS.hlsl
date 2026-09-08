#include "Common.hlsl"
#include "CookTranceSub.hlsl"

Texture2D g_Texture : register(t0); // 0番テクスチャ
Texture2D g_TextureRoughness : register(t1); // 1番テクスチャ
Texture2D g_TextureMetalness : register(t2); // 2番テクスチャ

SamplerState g_SamplerState : register(s0); // 0番サンプラー

float CalculateDiffuseFromFresnel(float3 N, float3 L, float3 V);
float CalculateCookTranceSpecular(float3 L, float3 V, float3 N, float smooth, float metalic);

static const float PI = 3.1415926f;

void main(in PS_IN In, out float4 outDiffuse : SV_TARGET)
{
    // 法線計算
    float4 normal = normalize(In.Normal);
    
    // アルベドカラー
    float4 albedo = g_Texture.Sample(g_SamplerState, In.TexCoord);
    
    // スペキュラカラー
    float3 speColor = albedo.rgb;
    
    // 滑らかさを取得
    float smooth = g_TextureRoughness.Sample(g_SamplerState, In.TexCoord).r * 2.0f - 1.0f;
    smooth = saturate(smooth);
    
    // 金属度を取得
    float metalic = g_TextureMetalness.Sample(g_SamplerState, In.TexCoord).r * 2.0f - 1.0f;
    metalic = saturate(metalic);
    
    // カメラへのベクトルを計算
    float3 eyev = CameraPosition.xyz - In.WorldPosition.xyz;
    eyev = normalize(eyev);
    
    float3 lit = 0;
    for (int i = 0; i < Parameter.z; i++)
    {
        // 光源へのベクトル
        float4 lv = Light.Position - In.WorldPosition;

        // フレネル反射を考慮した拡散計算
        float diffuseFromFresnel = CalculateDiffuseFromFresnel(normal.xyz, lv.xyz, eyev);

        // 正規化ランバート拡散反射
        float nl = saturate(dot(normal.xyz, lv.xyz));
        float3 light = nl + Light.Diffuse.rgb / PI;
        
        // 最終的な拡散反射光を計算
        float3 diffuse = albedo.rgb * diffuseFromFresnel * Light.Diffuse.rgb * light;
        
        // 鏡面反射を計算
        float3 specular = CalculateCookTranceSpecular(lv.xyz, eyev, normal.xyz, smooth, metalic)
                                                        * Light.Diffuse.rgb * light;
        
        specular *= lerp(float3(1.0f, 1.0f, 1.0f), speColor, metalic);
        
        lit += diffuse * (1.0f - smooth) + specular;
    }
    
    lit += Light.Ambient.rgb * albedo.rgb;
    
    outDiffuse.rgb = lit;
    outDiffuse.a = albedo.a * In.Diffuse.a;
    
    return;
}

float CalculateDiffuseFromFresnel(float3 N, float3 L, float3 V)
{
    float nl = saturate(dot(N, L));
    float nv = saturate(dot(N, V));
    
    return (nl * nv);
}

float CalculateCookTranceSpecular(float3 L, float3 V, float3 N, float smooth, float metalic)
{
    float3 H = normalize(L + V);
    
    float nh = saturate(dot(N, H));
    float vh = saturate(dot(V, H));
    float nl = saturate(dot(N, L));
    float nv = saturate(dot(N, V));

    float D = CalculateBeckmann(smooth, nh);
    float F = CalculateFresnel(metalic, vh);
    float G = CalculateGeometricDamping(nh, nv, nl, vh);
        
    float m = PI * nv * nh;
    
    return max(F * D * G / m, 0.0f);
}