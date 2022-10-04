#include "ParticleEmitter.h"

ParticleEmitter* ParticleEmitter::GetInstance()
{
	static ParticleEmitter instance;

	return &instance;
}

void ParticleEmitter::Initialize()
{
	//�p�[�e�B�N���}�l�[�W���[����
	particleManager.reset(ParticleManager::Create(1));
}

void ParticleEmitter::Update()
{
	//�p�[�e�B�N���}�l�[�W���[�X�V
	particleManager->Update();
}

void ParticleEmitter::DrawAll()
{
	//�p�[�e�B�N���}�l�[�W���[�`��
	particleManager->Draw();
}

void ParticleEmitter::DemoEffect()
{
	//�F
	const XMFLOAT4 purple = { 1.0f, 0.1f, 1.0f, 1.0f }; //��
	const XMFLOAT4 lightBlue = { 0.1f, 1.0f, 1.0f, 1.0f }; //���F

	for (int i = 0; i < 10; i++) {
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

		//�ǉ�
		particleManager->Add(life, pos, vel, acc, 1.0f, 0.0f, purple, lightBlue);
	}
}

void ParticleEmitter::DemoEffect2()
{
	//�F
	const XMFLOAT4 red = { 1.0f, 0.1f, 0.1f, 1.0f }; //��
	const XMFLOAT4 green = { 0.1f, 1.0f, 0.1f, 1.0f }; //���F

	for (int i = 0; i < 10; i++) {
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

		//�ǉ�
		particleManager->Add(life, pos, vel, acc, 1.0f, 0.0f, red, green);
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
		//�ǉ�
		particleManager->Add(5, pos, vel, acc, startScale, endScale, startColor, endColor);
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
		float endScale2 = 0;
		//�ǉ�
		particleManager->Add(5, pos, vel, acc, startScale, endScale2, startColor, endColor);
	}
}

void ParticleEmitter::ChargeShot(const Vector3& position, const float size)
{
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
		//�ǉ�
		particleManager->Add(2, position, vel, acc, startScale, endScale, startColor, endColor);
	}
}
