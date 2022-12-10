#include "ParticleEmitter.h"
#include "Easing.h"
#include <functional>
#include <d3dx12.h>

ParticleEmitter* ParticleEmitter::GetInstance()
{
	static ParticleEmitter instance;

	return &instance;
}

void ParticleEmitter::Initialize()
{
	//�e�N�X�`���S�ǂݍ���
	LoadTexture();

	//�p�[�e�B�N���}�l�[�W���[����
	circleParticle.reset(ParticleManager::Create(1));
	explosionParticle.reset(ParticleManager::Create(2));
	blackSmokeParticle.reset(ParticleManager::Create(2));
	shineParticle.reset(ParticleManager::Create(3));
}

void ParticleEmitter::Update()
{
	//�p�[�e�B�N���}�l�[�W���[�X�V
	circleParticle->Update();
	explosionParticle->Update();
	blackSmokeParticle->Update();
	shineParticle->Update();
}

void ParticleEmitter::DrawAll()
{
	//���Z�����`��O����
	ParticleManager::DrawPrevAddBlend();

	//�p�[�e�B�N���}�l�[�W���[�`��
	circleParticle->Draw();
	explosionParticle->Draw();
	shineParticle->Draw();

	//���Z�����`��O����
	ParticleManager::DrawPrevSubBlend();

	//�p�[�e�B�N���}�l�[�W���[�`��
	blackSmokeParticle->Draw();
}

void ParticleEmitter::DemoEffect()
{
	//�F
	const XMFLOAT4 purple = { 1.0f, 0.1f, 1.0f, 1.0f }; //��
	const XMFLOAT4 lightBlue = { 0.1f, 1.0f, 1.0f, 1.0f }; //���F

	for (int i = 0; i < 100; i++) {
		int life = 0;
		life = (rand() % 200) + 10;
		//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z
		const float mdPos = 10.0f;
		Vector3 pos{};
		pos.x = (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
		const float mdVel = 0.1f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * mdVel - mdVel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * mdVel - mdVel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * mdVel - mdVel / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
		Vector3 acc{};
		const float mdAcc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * mdAcc;

		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�ǉ�
		circleParticle->Add(life, pos, vel, acc, 1.0f, 0.0f, lerp, purple, lightBlue);
	}
}

void ParticleEmitter::DemoEffect2()
{
	//�F
	const XMFLOAT4 red = { 1.0f, 0.1f, 0.1f, 1.0f }; //��
	const XMFLOAT4 green = { 0.1f, 1.0f, 0.1f, 1.0f }; //���F

	for (int i = 0; i < 100; i++) {
		int life = 0;
		life = (rand() % 200) + 10;
		//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z
		const float mdPos = 10.0f;
		Vector3 pos{};
		pos.x = (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
		const float mdVel = 0.1f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * mdVel - mdVel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * mdVel - mdVel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * mdVel - mdVel / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
		Vector3 acc{};
		const float mdAcc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * mdAcc;

		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�ǉ�
		explosionParticle->Add(life, pos, vel, acc, 1.0f, 0.0f, lerp, red, green);
	}
}

void ParticleEmitter::PlayerJet(const XMMATRIX& playerMatWorld, const int playerSpeedPhase)
{
	//���@�̒��S���W����̋���
	const Vector3 distancePos = { 0, -0.25f, -1.2f };
	//���s�ړ��s��̌v�Z
	XMMATRIX matTrans = XMMatrixTranslation(distancePos.x, distancePos.y, distancePos.z);
	//���[���h�s��̍���
	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();	//�ό`�����Z�b�g
	matWorld *= matTrans;	//���[���h�s��ɕ��s�ړ��𔽉f
	//���@�I�u�W�F�N�g�̃��[���h�s���������
	matWorld *= playerMatWorld;
	//�p�[�e�B�N���������W���擾
	Vector3 pos = { matWorld.r[3].m128_f32[0], matWorld.r[3].m128_f32[1], matWorld.r[3].m128_f32[2] };

	//�p�[�e�B�N��(��)����
	{
		//�F
		const XMFLOAT4 startColor = { 0.6f, 0.6f, 0.6f, 1.0f }; //�Z����
		const XMFLOAT4 endColor = { 0.4f, 0.4f, 0.4f, 1.0f }; //������
		Vector3 vel{};
		Vector3 acc{};
		float startScale = 0;
		float endScale = 0;

		//�ړ����x�t�F�[�Y���Ƃɒ���
		//�ʏ�ړ����
		if (playerSpeedPhase == 0) {
			vel.z = (float)rand() / RAND_MAX * -0.1f - 0.1f;
			const float mdAcc = 0.005f;
			acc.z = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.1f + 0.5f;
		}
		//�����ړ����
		else if (playerSpeedPhase == 1) {
			vel.z = (float)rand() / RAND_MAX * -0.2f - 0.2f;
			const float mdAcc = 0.01f;
			acc.z = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.1f + 1.0f;
		}
		//�����ړ����
		else if (playerSpeedPhase == 2) {
			vel.z = (float)rand() / RAND_MAX * -0.001f - 0.001f;
			const float mdAcc = 0.001f;
			acc.z = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.05f + 0.25f;
		}
		//�ʏ�ړ��ɖ߂����
		else if (playerSpeedPhase == 3) {
			vel.z = (float)rand() / RAND_MAX * -0.002f - 0.002f;
			const float mdAcc = 0.0025f;
			acc.z = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.05f + 0.3f;
		}

		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		//�ǉ�
		circleParticle->Add(5, pos, vel, acc, startScale, endScale, lerp, startColor, endColor);
	}

	//�p�[�e�B�N��(��)����
	{
		//�F
		const XMFLOAT4 startColor = { 1.0f, 0.21f, 0.21f, 1.0f }; //������
		const XMFLOAT4 endColor = { 0.3f, 0.01f, 0.01f, 1.0f }; //�Z����
		Vector3 vel{};
		Vector3 acc{};
		float startScale = 0;
		float endScale = 0;

		//�ړ����x�t�F�[�Y���Ƃɒ���
		//�ʏ�ړ����
		if (playerSpeedPhase == 0) {
			vel.z = (float)rand() / RAND_MAX * -0.1f - 0.1f;
			const float mdAcc = 0.008f;
			acc.z = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.2f + 2.0f;
		}
		//�����ړ����
		else if (playerSpeedPhase == 1) {
			vel.z = (float)rand() / RAND_MAX * -0.2f - 0.2f;
			const float mdAcc = 0.0016f;
			acc.z = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.2f + 4.0f;
		}
		//�����ړ����
		else if (playerSpeedPhase == 2) {
			vel.z = (float)rand() / RAND_MAX * -0.04f - 0.04f;
			const float mdAcc = 0.004f;
			acc.z = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.1f + 1.0f;
		}
		//�ʏ�ړ��ɖ߂����
		else if (playerSpeedPhase == 3) {
			vel.z = (float)rand() / RAND_MAX * -0.06f - 0.06f;
			const float mdAcc = 0.005f;
			acc.z = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.15f + 1.2f;
		}

		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		//�ǉ�
		circleParticle->Add(5, pos, vel, acc, startScale, endScale, lerp, startColor, endColor);
	}
}

void ParticleEmitter::PlayerBlackSmokeJet(const XMMATRIX& playerMatWorld)
{
	//���@�̒��S���W����̋���
	const Vector3 distancePos = { 0, -0.25f, -1.2f };
	//���s�ړ��s��̌v�Z
	XMMATRIX matTrans = XMMatrixTranslation(distancePos.x, distancePos.y, distancePos.z);
	//���[���h�s��̍���
	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();	//�ό`�����Z�b�g
	matWorld *= matTrans;	//���[���h�s��ɕ��s�ړ��𔽉f
	//���@�I�u�W�F�N�g�̃��[���h�s���������
	matWorld *= playerMatWorld;
	//�p�[�e�B�N���������W���擾
	Vector3 pos = { matWorld.r[3].m128_f32[0], matWorld.r[3].m128_f32[1], matWorld.r[3].m128_f32[2] };

	//���W�����ɍ����G�t�F�N�g���쐬
	for (int i = 0; i < 2; i++) {
		//��������
		int life = (rand() % 30) + 60;

		//X,Y,Z�����_���ɕ��z
		const Vector3 mdVel = { 0.01f, 0.05f, 0.01f };
		Vector3 vel{};
		vel.x = ((float)rand() / RAND_MAX * mdVel.x - mdVel.x / 2.0f);
		vel.y = ((float)rand() / RAND_MAX * mdVel.y);
		vel.z = ((float)rand() / RAND_MAX * mdVel.z - mdVel.z / 2.0f);
		Vector3 acc{};
		const float mdAcc = 0.001f;
		acc.y = (float)rand() / RAND_MAX * mdAcc;

		//�傫�����Z�b�g
		const float mdScale = 2.5f;
		const float randScale = ((float)rand() / RAND_MAX * mdScale);
		const float startScale = randScale;
		const float endScale = randScale * 5.0f;
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerpFloat =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�F
		const XMFLOAT4 startColor = { 0.15f, 0.15f, 0.15f, 1 }; //������
		const XMFLOAT4 endColor = { 0.01f, 0.01f, 0.01f, 1 }; //����������

		//�ǉ�
		blackSmokeParticle->Add(life, pos, vel, acc, startScale, endScale, lerpFloat, startColor, endColor);
	}
}

void ParticleEmitter::Shot(const Vector3& position)
{
	//��������
	const int life = 3;
	//���x�A�����x��0
	const Vector3 vel{};
	const Vector3 acc{};
	//�傫���ύX�̃C�[�W���O
	std::function<float(const float, const float, const float) > lerp =
		std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	//�F
	const XMFLOAT4 green = { 0.1f, 0.3f, 0.1f, 1.0f }; //������
	for (int i = 0; i < 3; i++) {
		//�傫��
		const float scale = 2.5f;

		//�ǉ�
		circleParticle->Add(life, position, vel, acc, scale, scale, lerp, green, green);
	}

	//�F
	const XMFLOAT4 white = { 0.1f, 0.1f, 0.1f, 1.0f }; //������
	for (int i = 0; i < 3; i++) {
		//�傫��
		const float scale = 1.25f;

		//�ǉ�
		circleParticle->Add(life, position, vel, acc, scale, scale, lerp, white, white);
	}
}

void ParticleEmitter::ShotDead(const Vector3& position, const float size)
{
	//�F
	const XMFLOAT4 startColor = { 0.2f, 0.3f, 0.4f, 1.0f }; //������
	const XMFLOAT4 endColor = { 0, 0, 0, 1.0f }; //���F

	//���x�A�����x��0
	Vector3 vel{};
	Vector3 acc{};

	for (int i = 0; i < 3; i++) {
		//��������
		const int life = (rand() % 10) + 15;
		//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z
		const float mdPos = 0.5f;
		Vector3 pos = position;
		pos.x += (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		pos.z += (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		//�傫���������_���ɕ��z
		const float mdScale = 2.0f;
		const float startScale = (float)rand() / RAND_MAX * mdScale + (size * 2);
		const float endScale = startScale;
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > outQuad =
			std::bind(&Easing::OutCubic, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�ǉ�
		circleParticle->Add(life, pos, vel, acc, startScale, endScale, outQuad, startColor, endColor);
	}
}

void ParticleEmitter::ChargeShot(const Vector3& position, const float size)
{
	//��������
	const int life = 2;
	//�F
	const XMFLOAT4 startColor = { 0.1f, 1.0f, 0.05f, 1.0f }; //�Z����
	const XMFLOAT4 endColor = { 0.1f, 0.4f, 0.05f, 1.0f }; //������
	//���x�A�����x��0
	Vector3 vel{};
	Vector3 acc{};

	for (int i = 0; i < 10; i++) {
		//�傫���������_���ɕ��z
		const float mdScale = 2.8f;
		float startScale = (float)rand() / RAND_MAX * mdScale + (size * 2 / 7) - mdScale / 2;
		float endScale = (float)rand() / RAND_MAX * mdScale + (size * 2) - mdScale / 2;
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�ǉ�
		circleParticle->Add(life, position, vel, acc, startScale, endScale, lerp, startColor, endColor);
	}
}

void ParticleEmitter::ChargeShotDead(const Vector3& position, const float size)
{
	//�F
	const XMFLOAT4 startColor = { 0.04f, 0.1f, 0.04f, 1.0f }; //�Z����
	const XMFLOAT4 endColor = { 0, 0, 0, 1.0f }; //���F

	//���x�A�����x��0
	Vector3 vel{};
	Vector3 acc{};

	for (int i = 0; i < 20; i++) {
		//��������
		int life = (rand() % 20) + 30;
		//�傫���������_���ɕ��z
		const float mdScale = 10.0f;
		const float scale = size * 3 / 5;
		float startScale = 0;
		float endScale = (float)rand() / RAND_MAX * mdScale + (size * 2);
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > outQuad =
			std::bind(&Easing::OutCubic, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�ǉ�
		circleParticle->Add(life, position, vel, acc, startScale, endScale, outQuad, startColor, endColor);
	}

	for (int i = 0; i < 10; i++) {
		//��������
		int life = (rand() % 20) + 60;
		//�傫���������_���ɕ��z
		const float mdScale = 10.0f;
		float startScale = 0;
		float endScale = (float)rand() / RAND_MAX * mdScale + (size * 2);
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > outQuad =
			std::bind(&Easing::OutCubic, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�ǉ�
		circleParticle->Add(life, position, vel, acc, startScale, endScale, outQuad, startColor, endColor);
	}
}

void ParticleEmitter::BossCharge(const Vector3& position, const float size)
{
	//��������
	const int life = 2;
	//�F
	const XMFLOAT4 startColor = { 1.0f, 0.1f, 0.1f, 1.0f }; //�Z����
	const XMFLOAT4 endColor = { 0.4f, 0.01f, 0.01f, 1.0f }; //������
	//���x�A�����x��0
	Vector3 vel{};
	Vector3 acc{};

	for (int i = 0; i < 20; i++) {
		//�傫���������_���ɕ��z
		const float mdScale = 1.5f;
		float startScale = (float)rand() / RAND_MAX * mdScale + (size * 2 / 7) - mdScale / 2;
		float endScale = (float)rand() / RAND_MAX * mdScale + (size * 2) - mdScale / 2;
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�ǉ�
		circleParticle->Add(life, position, vel, acc, startScale, endScale, lerp, startColor, endColor);
	}
}

void ParticleEmitter::BossChargeShot(const Vector3& position, const float size)
{
	//��������
	const int life = 2;
	//�F
	const XMFLOAT4 startColor = { 1.0f, 0.1f, 0.1f, 1.0f }; //�Z����
	const XMFLOAT4 endColor = { 0.4f, 0.01f, 0.01f, 1.0f }; //������
	//���x�A�����x��0
	Vector3 vel{};
	Vector3 acc{};

	for (int i = 0; i < 20; i++) {
		//�傫���������_���ɕ��z
		const float mdScale = 1.0f;
		float startScale = (float)rand() / RAND_MAX * mdScale + (size * 2 / 4) - mdScale / 2;
		float endScale = (float)rand() / RAND_MAX * mdScale + (size * 2) - mdScale / 2;
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�ǉ�
		circleParticle->Add(life, position, vel, acc, startScale, endScale, lerp, startColor, endColor);
	}
}

void ParticleEmitter::BossChargeShotDead(const Vector3& position, const float size)
{
	//�F
	const XMFLOAT4 startColor = { 0.1f, 0.04f, 0.04f, 1.0f }; //�Z����
	const XMFLOAT4 endColor = { 0, 0, 0, 1.0f }; //���F

	//���x�A�����x��0
	Vector3 vel{};
	Vector3 acc{};

	for (int i = 0; i < 20; i++) {
		//��������
		int life = (rand() % 10) + 15;
		//�傫���������_���ɕ��z
		const float mdScale = 10.0f;
		const float scale = size * 3 / 5;
		float startScale = 0;
		float endScale = (float)rand() / RAND_MAX * mdScale + (size * 2);
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > outQuad =
			std::bind(&Easing::OutCubic, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�ǉ�
		circleParticle->Add(life, position, vel, acc, startScale, endScale, outQuad, startColor, endColor);
	}

	for (int i = 0; i < 10; i++) {
		//��������
		int life = (rand() % 20) + 60;
		//�傫���������_���ɕ��z
		const float mdScale = 10.0f;
		float startScale = 0;
		float endScale = (float)rand() / RAND_MAX * mdScale + (size * 2);
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > outQuad =
			std::bind(&Easing::OutCubic, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�ǉ�
		circleParticle->Add(life, position, vel, acc, startScale, endScale, outQuad, startColor, endColor);
	}
}

void ParticleEmitter::Explosion(const Vector3& position, const float size, const int time)
{
	for (int j = 0; j < 5; j++) {
		//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z
		const float mdPos = 2.0f;
		Vector3 pos = position;
		pos.x += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f) * size;
		pos.y += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f) * size;
		pos.z += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f) * size;

		for (int i = 0; i < 25; i++) {
			//��������
			int life = (rand() % 30) + time;

			//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z
			const float mdPos2 = 0.8f;
			pos.x += ((float)rand() / RAND_MAX * mdPos2 - mdPos2 / 2.0f) * size;
			pos.y += ((float)rand() / RAND_MAX * mdPos2 - mdPos2 / 2.0f) * size;
			pos.z += ((float)rand() / RAND_MAX * mdPos2 - mdPos2 / 2.0f) * size;

			//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
			const float mdVel = 0.05f;
			Vector3 vel{};
			vel.x = ((float)rand() / RAND_MAX * mdVel - mdVel / 2.0f) * size;
			vel.y = ((float)rand() / RAND_MAX * mdVel - mdVel / 2.0f) * size;
			vel.z = ((float)rand() / RAND_MAX * mdVel - mdVel / 2.0f) * size;
			Vector3 acc{};

			const float mdScale = 3.0f;
			float startScale = 0.3f * size;
			float endScale = (4.0f + (float)rand() / RAND_MAX * mdScale - mdScale / 2.0f) * size;
			//�傫���ύX�̃C�[�W���O
			std::function<float(const float, const float, const float) > outQuart =
				std::bind(&Easing::OutQuart, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
			//�F
			const float mdGreen = 0.2f;
			const float green = 0.2f + (float)rand() / RAND_MAX * mdGreen;
			const XMFLOAT4 startColor = { 0.9f, green, 0.1f, 1.0f }; //�Z����
			const XMFLOAT4 endColor = { 0, 0, 0, 1.0f }; //���F

			//�ǉ�
			explosionParticle->Add(life, pos, vel, acc, startScale, endScale, outQuart, startColor, endColor);
		}
	}
}

void ParticleEmitter::BossDeadExplosion(const Vector3& position)
{
	for (int j = 0; j < 20; j++) {
		//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z
		const float mdPos = 2.5f;
		Vector3 pos = position;
		pos.x += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f);
		pos.y += ((float)rand() / RAND_MAX * mdPos);
		pos.z += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f);

		for (int i = 0; i < 30; i++) {
			//��������
			int life = (rand() % 30) + 40;

			//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z
			const float mdPos2 = 2.5f;
			pos.x += ((float)rand() / RAND_MAX * mdPos2 - mdPos2 / 2.0f);
			pos.z += ((float)rand() / RAND_MAX * mdPos2 - mdPos2 / 2.0f);

			//X,Y,Z�S��[-0.25f, +0.25f]�Ń����_���ɕ��z
			const float mdVel = 0.75f;
			Vector3 vel{};
			vel.x = ((float)rand() / RAND_MAX * mdVel - mdVel / 2.0f);
			vel.y = ((float)rand() / RAND_MAX * mdVel);
			vel.z = ((float)rand() / RAND_MAX * mdVel - mdVel / 2.0f);
			Vector3 acc{};
			const float mdAcc = 0.01f;
			acc.y = -(float)rand() / RAND_MAX * mdAcc;

			const float mdScale = 5.0f;
			float startScale = 3.0f;
			float endScale = (20.0f + (float)rand() / RAND_MAX * mdScale - mdScale / 2.0f);
			//�傫���ύX�̃C�[�W���O
			std::function<float(const float, const float, const float) > outQuart =
				std::bind(&Easing::OutQuart, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
			//�F
			const float mdGreen = 0.2f;
			const float green = 0.2f + (float)rand() / RAND_MAX * mdGreen;
			const XMFLOAT4 startColor = { 0.9f, green, 0.1f, 1.0f }; //�Z����
			const XMFLOAT4 endColor = { 0, 0, 0, 1.0f }; //���F

			//�ǉ�
			explosionParticle->Add(life, pos, vel, acc, startScale, endScale, outQuart, startColor, endColor);
		}
	}
}

void ParticleEmitter::BlackSmoke(const Vector3& position, const float size)
{
	for (int i = 0; i < 2; i++) {
		//��������
		int life = (rand() % 30) + 30;

		//X,Y,Z���ƂɂŃ����_���ɕ��z
		const float mdPos = 0.1f;
		Vector3 pos = position;
		pos.x += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f);
		pos.y += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f) + 1.0f;
		pos.z += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f);

		//X,Y,Z�S��[-0.25f, +0.25f]�Ń����_���ɕ��z
		const Vector3 mdVel = { 0.01f, 0.05f, 0.01f };
		Vector3 vel{};
		vel.x = ((float)rand() / RAND_MAX * mdVel.x - mdVel.x / 2.0f);
		vel.y = ((float)rand() / RAND_MAX * mdVel.y);
		vel.z = ((float)rand() / RAND_MAX * mdVel.z - mdVel.z / 2.0f);
		Vector3 acc{};
		const float mdAcc = 0.003f;
		acc.y = (float)rand() / RAND_MAX * mdAcc;

		const float mdScale = 1.3f;
		const float randScale = ((float)rand() / RAND_MAX * mdScale);
		const float startScale = randScale * size;
		const float endScale = randScale * size * 2.0f;
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerpFloat =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�F
		const XMFLOAT4 startColor = { 0.15f, 0.15f, 0.15f, 1 }; //������
		const XMFLOAT4 endColor = { 0.01f, 0.01f, 0.01f, 1 }; //����������

		//�ǉ�
		blackSmokeParticle->Add(life, pos, vel, acc, startScale, endScale, lerpFloat, startColor, endColor);
	}
}

void ParticleEmitter::ItemShine(const Vector3& position, const float size, const int num)
{
	//��������
	for (int i = 0; i < num; i++) {
		int life = (rand() % 30) + 30;

		//X,Y,Z�ɂŃ����_���ɕ��z
		const float mdPos = 4.0f;
		Vector3 pos = position;
		pos.x += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f);
		pos.y += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f);
		pos.z += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f);

		//�ړ����Ȃ��̂ő��x�͕ύX�Ȃ�
		Vector3 vel{};
		Vector3 acc{};
		const float mdScale = 2.0f * size;
		const float randScale = ((float)rand() / RAND_MAX * mdScale) + (1.0f * size);
		const float scale = randScale;
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerpFloat =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�F
		const XMFLOAT4 startColor = { 0.8f, 0.8f, 0.6f, 1 }; //���F
		const XMFLOAT4 endColor = { 0.4f, 0.4f, 0.3f, 1 }; //�������F

		//��]���x
		const float mdRotSpeed = 5.0f;
		const float baseSpeed = 10.0f * ((float)num / 2);
		const float rotSpeed = ((float)rand() / RAND_MAX * mdRotSpeed) + baseSpeed;

		//�ǉ�
		shineParticle->Add(life, pos, vel, acc, scale, scale, lerpFloat, startColor, endColor, rotSpeed);
	}
}

void ParticleEmitter::AllDelete()
{
	//�S�p�[�e�B�N���̍폜
	circleParticle->AllDelete();
	explosionParticle->AllDelete();
}

void ParticleEmitter::LoadTexture()
{
	//�e�N�X�`���S�ǂݍ���
	ParticleManager::LoadTexture(1, "effect1.png");
	ParticleManager::LoadTexture(2, "effect2.png");
	ParticleManager::LoadTexture(3, "effect3.png");
}
