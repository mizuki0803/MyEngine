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
}

void ParticleEmitter::Update()
{
	//�p�[�e�B�N���}�l�[�W���[�X�V
	circleParticle->Update();
	explosionParticle->Update();
}

void ParticleEmitter::DrawAll()
{
	//�p�[�e�B�N���}�l�[�W���[�`��
	circleParticle->Draw();
	explosionParticle->Draw();
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
		const float md_pos = 10.0f;
		Vector3 pos{};
		pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
		const float md_vel = 0.1f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
		Vector3 acc{};
		const float md_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * md_acc;

		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1 , std::placeholders::_2, std::placeholders::_3);

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
		const float md_pos = 10.0f;
		Vector3 pos{};
		pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
		const float md_vel = 0.1f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
		Vector3 acc{};
		const float md_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * md_acc;

		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�ǉ�
		explosionParticle->Add(life, pos, vel, acc, 1.0f, 0.0f, lerp, red, green);
	}
}

void ParticleEmitter::PlayerJet(const XMMATRIX& playerMatWorld)
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
		vel.z = (float)rand() / RAND_MAX * -0.1f - 0.1f;
		Vector3 acc{};
		const float md_acc = 0.005f;
		acc.z = -(float)rand() / RAND_MAX * md_acc;
		float startScale = (float)rand() / RAND_MAX * 0.1f + 0.5f;
		float endScale = 0;
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
		vel.z = (float)rand() / RAND_MAX * -0.1f - 0.1f;
		Vector3 acc{};
		const float md_acc = 0.008f;
		acc.z = -(float)rand() / RAND_MAX * md_acc;
		float startScale = (float)rand() / RAND_MAX * 0.2f + 2.0f;
		float endScale = 0;
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		//�ǉ�
		circleParticle->Add(5, pos, vel, acc, startScale, endScale, lerp,startColor, endColor);
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
		const float md_pos = 0.5f;
		Vector3 pos = position;
		pos.x += (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.z += (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		//�傫���������_���ɕ��z
		const float md_scale = 2.0f;
		const float startScale = (float)rand() / RAND_MAX * md_scale + (size * 2);
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
		const float md_scale = 2.8f;
		float startScale = (float)rand() / RAND_MAX * md_scale + (size * 2 / 7) - md_scale / 2;
		float endScale = (float)rand() / RAND_MAX * md_scale + (size * 2) - md_scale / 2;
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
		const float md_scale = 10.0f;
		const float scale = size * 3 / 5;
		float startScale = 0;
		float endScale = (float)rand() / RAND_MAX * md_scale + (size * 2);
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
		const float md_scale = 10.0f;
		float startScale = 0;
		float endScale = (float)rand() / RAND_MAX * md_scale + (size * 2);
		//�傫���ύX�̃C�[�W���O
		std::function<float(const float, const float, const float) > outQuad =
			std::bind(&Easing::OutCubic, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//�ǉ�
		circleParticle->Add(life, position, vel, acc, startScale, endScale, outQuad, startColor, endColor);
	}
}

void ParticleEmitter::Explosion(const Vector3& position)
{
	for (int j = 0; j < 5; j++) {
		//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z
		const float md_pos = 2.0f;
		Vector3 pos = position;
		pos.x += (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.y += (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.z += (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;

		for (int i = 0; i < 25; i++) {
			//��������
			int life = (rand() % 30) + 10;

			//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z
			const float md_pos2 = 0.8f;
			pos.x += (float)rand() / RAND_MAX * md_pos2 - md_pos2 / 2.0f;
			pos.y += (float)rand() / RAND_MAX * md_pos2 - md_pos2 / 2.0f;
			pos.z += (float)rand() / RAND_MAX * md_pos2 - md_pos2 / 2.0f;

			//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
			const float md_vel = 0.05f;
			Vector3 vel{};
			vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			Vector3 acc{};

			const float md_scale = 3.0f;
			float startScale = 0.3f;
			float endScale = 4.0f + (float)rand() / RAND_MAX * md_scale - md_scale / 2.0f;
			//�傫���ύX�̃C�[�W���O
			std::function<float(const float, const float, const float) > outQuart =
				std::bind(&Easing::OutQuart, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
			//�F
			const float md_green = 0.2f;
			const float green = 0.2f + (float)rand() / RAND_MAX * md_green;
			const XMFLOAT4 startColor = { 0.9f, green, 0.1f, 1.0f }; //�Z����
			const XMFLOAT4 endColor = { 0, 0, 0, 1.0f }; //���F

			//�ǉ�
			explosionParticle->Add(life, pos, vel, acc, startScale, endScale, outQuart, startColor, endColor);
		}
	}
}

void ParticleEmitter::BlackSmoke(const Vector3& position)
{
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
}
