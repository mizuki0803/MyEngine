#include "Galaxy.h"
#include "BasePlayer.h"
#include "EnemyDefeatCounter.h"
#include "ParticleEmitter.h"

BasePlayer* Galaxy::player = nullptr;
const float Galaxy::appearModeTime = 600.0f;
const float Galaxy::waitModeTime = 500.0f;

Galaxy* Galaxy::Create(const Vector3& position)
{
	//�M�����N�V�[�̃C���X�^���X�𐶐�
	Galaxy* galaxy = new Galaxy();
	if (galaxy == nullptr) {
		return nullptr;
	}

	// ������
	if (!galaxy->Initialize(position)) {
		delete galaxy;
		assert(0);
		return nullptr;
	}

	return galaxy;
}

bool Galaxy::Initialize(const Vector3& position)
{
	//���̐���
	body.reset(GalaxyBody::Create(position));

	//HP�Z�b�g
	int maxHP = 1;
	HP = maxHP;

	//�r�w�C�r�A�c���[����
	behaviorTree.reset(GalaxyBehaviorTree::Create(this));

	return true;
}

void Galaxy::Update()
{
	//�X�V
	body->Update();//����

	//�r�w�C�r�A�c���[�ɂ��s���J��
	behaviorTree->Root();

	//HPUI�X�V
	if (hpUI) {
		hpUI->Update();
	}
	//�{�X���\��UI�X�V
	if (bossNameUI) {
		bossNameUI->Update();
	}
}

void Galaxy::Draw()
{
	//�`��
	body->Draw();//����
}

void Galaxy::DrawUI()
{
	//HPUI�`��
	if (hpUI) {
		hpUI->Draw();
	}
	//�{�X���\��UI�`��
	if (bossNameUI) {
		bossNameUI->Draw();
	}
}

void Galaxy::OnCollisionBody(const int damageNum, const Vector3& collisionPos, const Vector3& subjectVel)
{
	//���̂Ƀ_���[�W
	body->Damage(damageNum, collisionPos, subjectVel);

	//�M�����N�V�[�Ƀ_���[�W(���ۂɋ������_���[�W�ʂ��Z�b�g)
	const int mainBodyDamageNum = body->GetDamageNum();
	Damage(mainBodyDamageNum);
}

bool Galaxy::AppearModeCount()
{
	//�o���ԂłȂ���Δ�����
	if (!(phase == Phase::Appear)) { return false; }

	//�^�C�}�[���X�V
	appearModeTimer++;

	//�w�肵�����ԂɂȂ�����{�X���\��UI����
	const float bossNameUICreateTime = 160;
	if (appearModeTimer >= bossNameUICreateTime && !bossNameUI) {
		bossNameUI.reset(BossNameUI::Create(1));
	}

	//�w�肵�����ԂɂȂ�����HP�o�[����
	const float hpUICreateTime = appearModeTime - 100;
	if (appearModeTimer >= hpUICreateTime && !hpUI) {
		const Vector2 hpUIPosition = { 30, 170 };
		hpUI.reset(BossHPUI::Create(hpUIPosition, HP));
	}

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (appearModeTimer >= appearModeTime) {
		phase = Phase::Wait;

		//�{�X���\��UI�͂����g�p���Ȃ��̂ŉ�����Ă���
		bossNameUI.reset();
	}

	return true;
}

bool Galaxy::AppearFall()
{
	//�~���ɂ����鎞��
	const float fallTime = 400;
	//���ɍ~���ɂ����鎞�Ԉȏ�Ȃ甲����
	if (appearModeTimer > fallTime) { return false; }

	//�C�[�W���O�p��0�`1�̒l���Z�o����
	const float time = appearModeTimer / fallTime;

	//���̂��~��������
	body->Fall(time);

	return true;
}

bool Galaxy::WaitMode()
{
	//�ҋ@��ԂłȂ���Δ�����
	if (!(phase == Phase::Wait)) { return false; }

	return true;
}

bool Galaxy::DeadExplosion()
{
	//���S��ԂłȂ���Δ�����
	if (!(phase == Phase::Dead)) { return false; }

	//���S��Ԃ̎���
	const int deadModeTime = 480;
	//�^�C�}�[�X�V
	deadModeTimer++;

	//���̂̎��S�s��
	body->Dead();

	//���Ԋu�Ŕ���
	const int explosionInterval = 20;
	if ((deadModeTimer % (int)explosionInterval) == 0) {
		//�������o�p�p�[�e�B�N������
		Vector3 particlePos = body->GetWorldPos();
		const float distance = 8.0f;
		particlePos.x += (float)rand() / RAND_MAX * distance - distance / 2.0f;
		particlePos.y += (float)rand() / RAND_MAX * distance - distance / 2.0f;
		particlePos.z += (float)rand() / RAND_MAX * distance - distance / 2.0f;
		const float size = 2.0f;
		ParticleEmitter::GetInstance()->Explosion(particlePos, size);
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����폜
	if (deadModeTimer >= deadModeTime) {
		isDelete = true;
	}

	return true;
}

void Galaxy::Damage(const int damageNum)
{
	//�����̐����̕��_���[�W����炤
	HP -= damageNum;

	//HP��0�ȉ��ɂȂ����玀�S��Ԃɂ���
	if (HP <= 0) {
		HP = 0;

		phase = Phase::Dead;
		isDead = true;
	}

	//�_���[�W���������̂�HP�o�[�̒�����ύX����
	if (hpUI) {
		hpUI->Damage(HP);
	}
}
