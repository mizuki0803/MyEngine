#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);	//0番スロットに設定されたテクスチャ
SamplerState smp : register(s0);		//0番スロットに設定されたサンプラー

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
	float d = distance(drawUV, pickUV);
	return exp(-(d * d) / (2 * sigma * sigma));
}

float4 GaussianBlur(float2 uv, float sigma, float stepWidth)
{
	float totalWeight = 0;
	float4 col = float4(0, 0, 0, 0);

	for (float py = -sigma * 2; py <= sigma * 2; py += stepWidth) {
		for (float px = -sigma * 2; px <= sigma * 2; px += stepWidth) {
			float2 pickUV = uv + float2(px, py);
			float weight = Gaussian(uv, pickUV, sigma);
			col += tex.Sample(smp, pickUV) * weight;
			totalWeight += weight;
		}
	}
	col.rgb = col.rgb / totalWeight;
	return col;
}

float4 main(VSOutput input) : SV_TARGET
{
	float4 texColor = tex.Sample(smp, input.uv);
	return float4(texColor.rgb, 1);

	////ガウシアンブラー
	//float _Sigma = 0.005, _StepWidth = 0.001;
	//return GaussianBlur(input.uv, _Sigma, _StepWidth);

	/*float _Depth = 0.05f;
	float _Width = 0.05f;

	float4 depth = tex.Sample(smp, input.uv);
	float4 pintColor = tex.Sample(smp, input.uv);
	float4 overColor = GaussianBlur(input.uv, 0.03f, 0.005f);
	float pint = smoothstep(0, _Width / 2, abs(depth.r - _Depth));
	return (1 - pint) * pintColor + pint * overColor;*/
}