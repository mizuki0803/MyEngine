#include "Obj.hlsli"

Texture2D<float4> tex : register(t0);	//0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//0番スロットに設定されたサンプラー

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
	float4 target2 : SV_TARGET2;
};

PSOutput main(VSOutput input)
{
	PSOutput output;


	////テクスチャマッピング
	//float4 texcolor = tex.Sample(smp, input.uv);
	float4 color = float4(1, 0, 0, 1);
	////Lambert反射
	float3 light = normalize(float3(0.2f, 0.3f, -1));
	//float diffuse = saturate(dot(-light, input.normal));
	//float brightness = diffuse + 0.3f;
	//float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
	////陰影とテクスチャの色を合成
	//return shadecolor * texcolor;

	//単色
	//return color;
	

	//ベクトル計算
	float3 eyeDir = normalize(cameraPos - input.worldpos.xyz);
	float3 halfVec = normalize(light + eyeDir);
	float intensity = saturate(dot(normalize(input.normal), light));

	//ADS合成
	float4 ambient = color * float4(0.3f, 0.3f, 0.3f, 1);
	float4 diffuse = saturate(dot(normalize(input.normal), light)) * color;
	float4 specular = pow(saturate(dot(normalize(input.normal), halfVec)), 50) * float4(1, 1, 1, 1);
	
	float4 ads = ambient + diffuse + specular;
	
	//return ads;


	//トゥーン
	float bright = smoothstep(0.5f, 0.6f, intensity);
	float dark = 1 - smoothstep(0.5f, 0.6f, intensity);
	
	float4 brightColor = color;
	float4 darkColor = color * float4(0.3f, 0.3f, 0.3f, 1);
	
	float4 toonColor = bright * brightColor + dark * darkColor;
	
	//return toonColor;

	output.target0 = color;
	output.target1 = ads;
	output.target2 = toonColor;
	return output;


	////テクスチャマッピング
	//float4 texcolor = tex.Sample(smp, input.uv);

	//// 光沢度
	//const float shininess = 4.0f;
	//// 頂点から視点への方向ベクトル
	//float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	//// 環境反射光
	//float3 ambient = m_ambient;

	//// シェーディングによる色
	//float4 shadecolor = float4(ambientColor * ambient, m_alpha);

	////平行光源
	//for (int i = 0; i < DIRLIGHT_NUM; i++) {
	//	if (dirLights[i].active) {
	//		// ライトに向かうベクトルと法線の内積
	//		float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
	//		// 反射光ベクトル
	//		float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
	//		// 拡散反射光
	//		float3 diffuse = dotlightnormal * m_diffuse;
	//		// 鏡面反射光
	//		float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	//		// 全て加算する
	//		shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
	//	}
	//}

	////点光源
	//for (int i = 0; i < POINTLIGHT_NUM; i++) {
	//	if (pointLights[i].active) {
	//		// ライトへのベクトル
	//		float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
	//		//ベクトルの長さ
	//		float d = length(lightv);
	//		//正規化し、単位ベクトルにする
	//		lightv = normalize(lightv);
	//		//距離減衰係数
	//		float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d + pointLights[i].lightatten.z * d * d);
	//		//ライトに向かうベクトルと法線の内積
	//		float3 dotlightnormal = dot(lightv, input.normal);
	//		//反射光ベクトル
	//		float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
	//		//拡散反射光
	//		float3 diffuse = dotlightnormal * m_diffuse;
	//		//鏡面反射光
	//		float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	//		//全て加算する
	//		shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;
	//	}
	//}

	////スポットライト
	//for (i = 0; i < SPOTLIGHT_NUM; i++) {
	//	if (spotLights[i].active) {
	//		//ライトへの方向ベクトル
	//		float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
	//		float d = length(lightv);
	//		lightv = normalize(lightv);
	//		//距離減衰係数
	//		float atten = saturate(1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d + spotLights[i].lightatten.z * d * d));
	//		//角度減衰
	//		float cos = dot(lightv, spotLights[i].lightv);
	//		//減衰開始角度から、減衰終了角度にかけて減衰
	//		//減衰開始角度の内側は1倍 減衰終了角度の外側は0倍の輝度
	//		float angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x, cos);
	//		//角度減衰を乗算
	//		atten *= angleatten;
	//		//ライトに向かうベクトルと法線の内積
	//		float3 dotlightnormal = dot(lightv, input.normal);
	//		//反射光ベクトル
	//		float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
	//		//拡散反射光
	//		float3 diffuse = dotlightnormal * m_diffuse;
	//		//鏡面反射光
	//		float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	//		//全て加算する
	//		shadecolor.rgb += atten * (diffuse + specular) * spotLights[i].lightcolor;
	//	}
	//}

	////丸影
	//for (int i = 0; i < CIRCLESHADOW_NUM; i++) {
	//	if (circleShadows[i].active) {
	//		//オブジェクト表面からキャスターへのベクトル
	//		float3 casterv = circleShadows[i].casterPos - input.worldpos.xyz;
	//		//投影方向での距離
	//		float d = dot(casterv, circleShadows[i].dir);
	//		//距離減衰係数
	//		float atten = saturate(1.0f / (circleShadows[i].atten.x + circleShadows[i].atten.y * d + circleShadows[i].atten.z * d * d));
	//		//距離がマイナスなら0にする
	//		atten *= step(0, d);
	//		//仮想ライトの座標
	//		float3 lightpos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight;
	//		//オブジェクト表面からライトへのベクトル(単位ベクトル)
	//		float3 lightv = normalize(lightpos - input.worldpos.xyz);
	//		//角度減衰
	//		float cos = dot(lightv, circleShadows[i].dir);
	//		//減衰開始角度から、減衰終了角度にかけて減衰
	//		//減衰開始角度の内側は1倍 減衰終了角度の外側は0倍の輝度
	//		float angleatten = smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);
	//		//角度減衰を乗算
	//		atten *= angleatten;

	//		//全て加算する
	//		shadecolor.rgb -= atten;
	//	}
	//}

	//// シェーディングによる色で描画
	//return shadecolor * texcolor;
}