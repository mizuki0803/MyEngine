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

	for (int i = 0; i < 10; i++)
	{
		int life = 0;
		life = (rand() % 200) + 10;
		//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z
		const float md_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
		const float md_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
		XMFLOAT3 acc{};
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

	for (int i = 0; i < 10; i++)
	{
		int life = 0;
		life = (rand() % 200) + 10;
		//X,Y,Z�S��[-5.0f, +5.0f]�Ń����_���ɕ��z
		const float md_pos = 10.0f;
		XMFLOAT3 pos{};
		pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		//X,Y,Z�S��[-0.05f, +0.05f]�Ń����_���ɕ��z
		const float md_vel = 0.1f;
		XMFLOAT3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//�d�͂Ɍ����Ă�Y�̂�[-0.001f, 0]�Ń����_���ɕ��z
		XMFLOAT3 acc{};
		const float md_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * md_acc;

		//�ǉ�
		particleManager->Add(life, pos, vel, acc, 1.0f, 0.0f, red, green);
	}
}
