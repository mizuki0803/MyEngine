#include "ShadowMap.hlsli"

Texture2D<float> tex : register(t0);	//0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//0番スロットに設定されたサンプラー

float4 main(VSOutput input) : SV_TARGET
{
	float color = tex.Sample(smp, input.uv);

	return float4(color, color, color, 1);
}