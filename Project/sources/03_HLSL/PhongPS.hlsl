/*------------------------------------------------------------
	フォンシェーディングピクセルシェーダー
------------------------------------------------------------*/
#include "Common.hlsl"

Texture2D gTexture : register(t0); // 0番テクスチャ
SamplerState gSamplerState : register(s0); // 0番サンプラー

void main(in PS_IN In, out float4 outDiffuse : SV_Target)
{
    // 法線を正規化
    float4 normal = normalize(In.Normal);
    
    // 光源計算
    float light = 0.5 - 0.5 * dot(Light.Direction.xyz, normal.xyz);
    light = saturate(light);
    
    // テクスチャが存在する場合はテクスチャカラー、無ければマテリアルカラーを取得
    if (Material.TextureEnable)
    {
        outDiffuse = gTexture.Sample(gSamplerState, In.TexCoord);
    }
    else
    {
        outDiffuse = Material.Diffuse;
    }
    
    float3 diffuse = light * (outDiffuse.rgb * In.Diffuse.rgb);
    float3 ambient = Light.Ambient.rgb * Light.Diffuse.rgb * outDiffuse.rgb;
    
    outDiffuse.rgb = diffuse + ambient; // 頂点の明るさを合成
    outDiffuse.a = In.Diffuse.a; // 元の頂点のアルファ値のまま出力
    
    // 視線ベクトル作成
    float3 eyev = In.WorldPosition.xyz - CameraPosition.xyz;
    eyev = normalize(eyev);

    // 光の反射ベクトルを計算
    float3 refv = reflect(Light.Direction.xyz, normal.xyz);
    refv = normalize(refv);
    
    // 鏡面反射を計算
    float speculer = -dot(eyev, refv);
    speculer = saturate(speculer);
    speculer = pow(speculer, 30.0f);
    
    outDiffuse.rgb += speculer;
}