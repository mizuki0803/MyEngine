#include "PostEffect.hlsli"

Texture2D<float4> tex : register(t0);	//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);		//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

float Gaussian(float2 drawUV, float2 pickUV, float sigma)
{
	float d = distance(drawUV, pickUV);
	return exp(-(d * d) / (2 * sigma * sigma));
}

float4 main(VSOutput input) : SV_TARGET
{
	//�ʏ�`��
	float4 texcolor = tex.Sample(smp, input.uv);
	return float4(texcolor.rgb, 1);

	//�K�E�V�A���u���[
	/*float totalWeight = 0, _Sigma = 0.005, _StepWidth = 0.001;
	float4 col = float4(0, 0, 0, 0);

	for (float py = -_Sigma * 2; py <= _Sigma * 2; py += _StepWidth)
	{
		for (float px = -_Sigma * 2; px <= _Sigma * 2; px += _StepWidth)
		{
			float2 pickUV = input.uv + float2(px, py);
			float weight = Gaussian(input.uv, pickUV, _Sigma);
			col += tex.Sample(smp, pickUV) * weight;
			totalWeight += weight;
		}
	}
	col.rgb = col.rgb / totalWeight;
	return col;*/
}