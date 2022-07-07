#include "Obj.hlsli"

Texture2D<float4> tex : register(t0);	//0�ԃX���b�g�ɐݒ肳�ꂽ�e�N�X�`��
SamplerState smp : register(s0);		//0�ԃX���b�g�ɐݒ肳�ꂽ�T���v���[

struct PSOutput
{
	float4 target0 : SV_TARGET0;
	float4 target1 : SV_TARGET1;
	float4 target2 : SV_TARGET2;
};

PSOutput main(VSOutput input)
{
	PSOutput output;


	////�e�N�X�`���}�b�s���O
	//float4 texcolor = tex.Sample(smp, input.uv);
	float4 color = float4(1, 0, 0, 1);
	////Lambert����
	float3 light = normalize(float3(0.2f, 0.3f, -1));
	//float diffuse = saturate(dot(-light, input.normal));
	//float brightness = diffuse + 0.3f;
	//float4 shadecolor = float4(brightness, brightness, brightness, 1.0f);
	////�A�e�ƃe�N�X�`���̐F������
	//return shadecolor * texcolor;

	//�P�F
	//return color;
	

	//�x�N�g���v�Z
	float3 eyeDir = normalize(cameraPos - input.worldpos.xyz);
	float3 halfVec = normalize(light + eyeDir);
	float intensity = saturate(dot(normalize(input.normal), light));

	//ADS����
	float4 ambient = color * float4(0.3f, 0.3f, 0.3f, 1);
	float4 diffuse = saturate(dot(normalize(input.normal), light)) * color;
	float4 specular = pow(saturate(dot(normalize(input.normal), halfVec)), 50) * float4(1, 1, 1, 1);
	
	float4 ads = ambient + diffuse + specular;
	
	//return ads;


	//�g�D�[��
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


	////�e�N�X�`���}�b�s���O
	//float4 texcolor = tex.Sample(smp, input.uv);

	//// ����x
	//const float shininess = 4.0f;
	//// ���_���王�_�ւ̕����x�N�g��
	//float3 eyedir = normalize(cameraPos - input.worldpos.xyz);

	//// �����ˌ�
	//float3 ambient = m_ambient;

	//// �V�F�[�f�B���O�ɂ��F
	//float4 shadecolor = float4(ambientColor * ambient, m_alpha);

	////���s����
	//for (int i = 0; i < DIRLIGHT_NUM; i++) {
	//	if (dirLights[i].active) {
	//		// ���C�g�Ɍ������x�N�g���Ɩ@���̓���
	//		float3 dotlightnormal = dot(dirLights[i].lightv, input.normal);
	//		// ���ˌ��x�N�g��
	//		float3 reflect = normalize(-dirLights[i].lightv + 2 * dotlightnormal * input.normal);
	//		// �g�U���ˌ�
	//		float3 diffuse = dotlightnormal * m_diffuse;
	//		// ���ʔ��ˌ�
	//		float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;

	//		// �S�ĉ��Z����
	//		shadecolor.rgb += (diffuse + specular) * dirLights[i].lightcolor;
	//	}
	//}

	////�_����
	//for (int i = 0; i < POINTLIGHT_NUM; i++) {
	//	if (pointLights[i].active) {
	//		// ���C�g�ւ̃x�N�g��
	//		float3 lightv = pointLights[i].lightpos - input.worldpos.xyz;
	//		//�x�N�g���̒���
	//		float d = length(lightv);
	//		//���K�����A�P�ʃx�N�g���ɂ���
	//		lightv = normalize(lightv);
	//		//���������W��
	//		float atten = 1.0f / (pointLights[i].lightatten.x + pointLights[i].lightatten.y * d + pointLights[i].lightatten.z * d * d);
	//		//���C�g�Ɍ������x�N�g���Ɩ@���̓���
	//		float3 dotlightnormal = dot(lightv, input.normal);
	//		//���ˌ��x�N�g��
	//		float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
	//		//�g�U���ˌ�
	//		float3 diffuse = dotlightnormal * m_diffuse;
	//		//���ʔ��ˌ�
	//		float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	//		//�S�ĉ��Z����
	//		shadecolor.rgb += atten * (diffuse + specular) * pointLights[i].lightcolor;
	//	}
	//}

	////�X�|�b�g���C�g
	//for (i = 0; i < SPOTLIGHT_NUM; i++) {
	//	if (spotLights[i].active) {
	//		//���C�g�ւ̕����x�N�g��
	//		float3 lightv = spotLights[i].lightpos - input.worldpos.xyz;
	//		float d = length(lightv);
	//		lightv = normalize(lightv);
	//		//���������W��
	//		float atten = saturate(1.0f / (spotLights[i].lightatten.x + spotLights[i].lightatten.y * d + spotLights[i].lightatten.z * d * d));
	//		//�p�x����
	//		float cos = dot(lightv, spotLights[i].lightv);
	//		//�����J�n�p�x����A�����I���p�x�ɂ����Č���
	//		//�����J�n�p�x�̓�����1�{ �����I���p�x�̊O����0�{�̋P�x
	//		float angleatten = smoothstep(spotLights[i].lightfactoranglecos.y, spotLights[i].lightfactoranglecos.x, cos);
	//		//�p�x��������Z
	//		atten *= angleatten;
	//		//���C�g�Ɍ������x�N�g���Ɩ@���̓���
	//		float3 dotlightnormal = dot(lightv, input.normal);
	//		//���ˌ��x�N�g��
	//		float3 reflect = normalize(-lightv + 2 * dotlightnormal * input.normal);
	//		//�g�U���ˌ�
	//		float3 diffuse = dotlightnormal * m_diffuse;
	//		//���ʔ��ˌ�
	//		float3 specular = pow(saturate(dot(reflect, eyedir)), shininess) * m_specular;
	//		//�S�ĉ��Z����
	//		shadecolor.rgb += atten * (diffuse + specular) * spotLights[i].lightcolor;
	//	}
	//}

	////�ۉe
	//for (int i = 0; i < CIRCLESHADOW_NUM; i++) {
	//	if (circleShadows[i].active) {
	//		//�I�u�W�F�N�g�\�ʂ���L���X�^�[�ւ̃x�N�g��
	//		float3 casterv = circleShadows[i].casterPos - input.worldpos.xyz;
	//		//���e�����ł̋���
	//		float d = dot(casterv, circleShadows[i].dir);
	//		//���������W��
	//		float atten = saturate(1.0f / (circleShadows[i].atten.x + circleShadows[i].atten.y * d + circleShadows[i].atten.z * d * d));
	//		//�������}�C�i�X�Ȃ�0�ɂ���
	//		atten *= step(0, d);
	//		//���z���C�g�̍��W
	//		float3 lightpos = circleShadows[i].casterPos + circleShadows[i].dir * circleShadows[i].distanceCasterLight;
	//		//�I�u�W�F�N�g�\�ʂ��烉�C�g�ւ̃x�N�g��(�P�ʃx�N�g��)
	//		float3 lightv = normalize(lightpos - input.worldpos.xyz);
	//		//�p�x����
	//		float cos = dot(lightv, circleShadows[i].dir);
	//		//�����J�n�p�x����A�����I���p�x�ɂ����Č���
	//		//�����J�n�p�x�̓�����1�{ �����I���p�x�̊O����0�{�̋P�x
	//		float angleatten = smoothstep(circleShadows[i].factorAngleCos.y, circleShadows[i].factorAngleCos.x, cos);
	//		//�p�x��������Z
	//		atten *= angleatten;

	//		//�S�ĉ��Z����
	//		shadecolor.rgb -= atten;
	//	}
	//}

	//// �V�F�[�f�B���O�ɂ��F�ŕ`��
	//return shadecolor * texcolor;
}