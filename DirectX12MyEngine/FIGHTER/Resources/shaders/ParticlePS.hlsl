#include "Particle.hlsli"

Texture2D<float4> tex : register(t0);	//0�ԃe�N�X�`��
SamplerState smp : register(s0);		//0�ԃT���v���[

float4 main(GSOutput input) : SV_TARGET
{
	//float3 light = normalize(float3(1, -1, 1));	//�E�����@�����̃��C�g
	//float diffuse = saturate(dot(-light, input.normal));	//diffuse��[0, 1]�͈̔͂�Clamp����
	//float brightness = diffuse + 0.3f;	//�A���r�G���g����0.3�Ƃ��Čv�Z
	//float4 texcolor = float4(tex.Sample(smp, input.uv));
	//return float4(texcolor.rgb * brightness, texcolor.a) * color;	//�P�x��RGB�ɑ�����ďo��
	float4 texcolor = tex.Sample(smp, input.uv) * input.color;
	return texcolor;
}