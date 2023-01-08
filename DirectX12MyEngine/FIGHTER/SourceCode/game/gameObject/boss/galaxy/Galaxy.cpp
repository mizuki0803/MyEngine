#include "Galaxy.h"
#include "BasePlayer.h"
#include "EnemyDefeatCounter.h"
#include "ParticleEmitter.h"

BasePlayer* Galaxy::player = nullptr;
const float Galaxy::waitModeTime = 300.0f;

Galaxy* Galaxy::Create(const Vector3& bornPos, const Vector3& basePos)
{
	//�M�����N�V�[�̃C���X�^���X�𐶐�
	Galaxy* galaxy = new Galaxy();
	if (galaxy == nullptr) {
		return nullptr;
	}

	// ������
	if (!galaxy->Initialize(bornPos, basePos)) {
		delete galaxy;
		assert(0);
		return nullptr;
	}

	return galaxy;
}

bool Galaxy::Initialize(const Vector3& bornPos, const Vector3& basePos)
{
	//�������W���Z�b�g
	this->bornPos = bornPos;
	//����W���Z�b�g
	this->basePos = basePos;

	//���̐���
	body.reset(GalaxyBody::Create(bornPos, basePos));
	//�D�񐶐�
	bow.reset(GalaxyBow::Create(body.get(), { 0, -0.5f, -1 }));
	//��C����
	std::vector<Vector3> cannonPos = {
		{1, -0.3f, -1},
		{1, -0.3f, 0},
		{1, -0.3f, 1},
	};
	for (int i = 0; i < cannonPos.size(); i++) {
		std::unique_ptr<GalaxyCannon> newCannon;
		newCannon.reset(GalaxyCannon::Create(body.get(), cannonPos[i]));
		cannons.push_back(std::move(newCannon));
	}

	//HP�Z�b�g
	int maxHP = GalaxyBow::GetMaxHP() + GalaxyCannon::GetMaxHP() * (int)cannons.size();
	HP = maxHP;

	//�r�w�C�r�A�c���[����
	behaviorTree.reset(GalaxyBehaviorTree::Create(this));

	return true;
}

void Galaxy::Update()
{
	//�X�V
	body->Update();//����
	bow->Update();//�D��
	for (const std::unique_ptr<GalaxyCannon>& cannon : cannons) {
		cannon->Update();//��C
	}

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
	bow->Draw();//�D��
	for (const std::unique_ptr<GalaxyCannon>& cannon : cannons) {
		cannon->Draw();//��C
	}
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

void Galaxy::OnCollisionBow(const int damageNum, const Vector3& collisionPos)
{
	//�D�񂪍U������p�[�c�łȂ���Δ�����
	if (!(attackPartPhase == AttackPartPhase::Front)) { return; }

	//�D��Ƀ_���[�W
	bow->Damage(damageNum, collisionPos);
	//���̂ɂ����o������
	body->Damage();

	//�M�����N�V�[�Ƀ_���[�W(���ۂɋ������_���[�W�ʂ��Z�b�g)
	const int mainBodyDamageNum = bow->GetDamageNum();
	Damage(mainBodyDamageNum);
}

void Galaxy::OnCollisionCannon(GalaxyCannon* cannon, const int damageNum, const Vector3& collisionPos)
{
	//��C���U������p�[�c�łȂ���Δ�����
	if (!(attackPartPhase == AttackPartPhase::Cannon)) { return; }

	//��C�����Ɏ��S���Ă����甲����
	if (cannon->GetIsDead()) { return; }

	//�Փ˂�����C�Ƀ_���[�W
	cannon->Damage(damageNum, collisionPos);

	//��C���S�ł������`�F�b�N
	CheckAllCannonDead();

	//�M�����N�V�[�S�̂Ƀ_���[�W(��C�����ۂɋ������_���[�W�ʂ��Z�b�g)
	const int avatarBodyDamageNum = cannon->GetDamageNum();
	Damage(avatarBodyDamageNum);
}

bool Galaxy::AppearMode()
{
	//�o���ԂłȂ���Δ�����
	if (!(phase == Phase::Appear)) { return false; }

	//���̂�o�ꂳ����
	body->Appear();

	//�{�X�̓o�ꂪ����������
	if (!body->GetIsAppear()) {
		//�U������p�[�c���C�ɐݒ�
		attackPartPhase = AttackPartPhase::Cannon;

		//���̃t�F�[�Y��
		phase = Phase::Attack;
		//�U���J�n�p�A�ϐ��Ȃǂ�������
		AttackModeStart();

		//�{�X���\��UI�͂����g�p���Ȃ��̂ŉ�����Ă���
		bossNameUI.reset();
	}

	return true;
}

bool Galaxy::AppearUICreate()
{
	//�o���ԂłȂ���Δ�����
	if (!(phase == Phase::Appear)) { return false; }

	//�{�X���W���w�肵�����C����肱���瑤�ɂȂ�����{�X���\��UI����
	const float bossNameUICreatePos = bornPos.z - 100;
	if (body->GetPosition().z <= bossNameUICreatePos && !bossNameUI) {
		bossNameUI.reset(BossNameUI::Create(1));
	}

	//��]���w�肵�����C���܂Ői�񂾂�HP�o�[����
	const float bossNameUICreateRota = 45;
	if (body->GetRotation().y >= bossNameUICreateRota && !hpUI) {
		const Vector2 hpUIPosition = { 30, 170 };
		hpUI.reset(BossHPUI::Create(hpUIPosition, HP));
	}

	return true;
}

bool Galaxy::AttackMode()
{
	//�U����ԂłȂ���Δ�����
	if (!(phase == Phase::Attack)) { return false; }

	//�U������p�[�c����C�̂Ƃ��͑�C�̍U�����I������������
	if (attackPartPhase == AttackPartPhase::Cannon) {
		for (const std::unique_ptr<GalaxyCannon>& cannon : cannons) {
			//��C�����Ɏ��S���Ă������΂�
			if (cannon->GetIsDead()) { continue; }
			//�U�����Ȃ甲����
			if (cannon->GetIsAttack()) { return true; }
		}
		//�U�����I�������̂őҋ@��Ԃɂ���
		phase = Phase::Wait;
	}
	//�U������p�[�c���D��̂Ƃ��͑D��̍U�����I������������
	else if (attackPartPhase == AttackPartPhase::Front) {
		//�U�����Ȃ甲����
		if (bow->GetIsAttack()) { return true; }

		//�U�����I�������̂őҋ@��Ԃɂ���
		phase = Phase::Wait;
	}

	return true;
}

bool Galaxy::UpdateBulletShotPos()
{
	//�U����ԂłȂ���Δ�����
	if (!(phase == Phase::Attack)) { return false; }

	//�U������p�[�c����C�̂Ƃ��͑�C�̔��ˍ��W���X�V
	if (attackPartPhase == AttackPartPhase::Cannon) {
		for (const std::unique_ptr<GalaxyCannon>& cannon : cannons) {
			//��C�����Ɏ��S���Ă������΂�
			if (cannon->GetIsDead()) { continue; }

			cannon->UpdateBulletShotPos();
		}
	}
	//�U������p�[�c���D��̂Ƃ��͑D��̔��ˍ��W���X�V
	else if (attackPartPhase == AttackPartPhase::Front) {
		bow->UpdateBulletShotPos();
	}

	return true;
}

bool Galaxy::AttackTypeSelectStart()
{
	//�U�����e�����Ɍ��܂��Ă�����true��Ԃ�
	if (!(attackType == AttackType::None)) { return true; }

	//�U�����e�ݒ���J�n���邽��false��Ԃ�
	return false;
}

bool Galaxy::AttackTypeRapidFireCannonSelect()
{
	//��C���U������p�[�c�łȂ���Δ�����
	if (!(attackPartPhase == AttackPartPhase::Cannon)) { return false; }

	//�O��̍U�����e������(��C)�������甲����
	if (preAttackType == AttackType::RapidFireCannon) { return false; }

	//�v���C���[���@����ʍ����ɂ����甲����
	if (player->GetPosition().x <= 0) { return false; }

	//��C�ɍU�����e:���˂̊J�n���ҋ@�t���[�������Z�b�g
	int rapidFireCannonStartWaitTime = 0;
	//��C�̔��ˊԊu
	const int rapidFireCannonInterval = 10;
	for (const std::unique_ptr<GalaxyCannon>& cannon : cannons) {
		cannon->SetRapidFireStartWaitTime(rapidFireCannonStartWaitTime);

		//���ˊԊu�̐������Z���Ă��炷
		rapidFireCannonStartWaitTime += rapidFireCannonInterval;
	}

	//�U�����e:����(��C)���Z�b�g
	attackType = AttackType::RapidFireCannon;
	//1�O�̍U�����e���X�V
	preAttackType = AttackType::RapidFireCannon;

	return true;
}

bool Galaxy::AttackTypeASelect()
{
	//�U�����e:a���Z�b�g
	attackType = AttackType::a;
	//1�O�̍U�����e���X�V
	preAttackType = AttackType::a;

	return true;
}

bool Galaxy::AttackTypeFlamethrowerBowSelect()
{
	//�D�񂪍U������p�[�c�łȂ���Δ�����
	if (!(attackPartPhase == AttackPartPhase::Front)) { return false; }

	//�O��̍U�����e���Ή�����(�D��)�������甲����
	if (preAttackType == AttackType::FlamethrowerBow) { return false; }

	//�v���C���[���@����ʍ����ɂ����甲����
	if (player->GetPosition().x <= 0) { return false; }


	//�U�����e:����(��C)���Z�b�g
	attackType = AttackType::FlamethrowerBow;
	//1�O�̍U�����e���X�V
	preAttackType = AttackType::FlamethrowerBow;

	return true;
}

bool Galaxy::AttackTypeRapidFireCannon()
{
	//�U�����e������(��C)�łȂ���Δ�����
	if (!(attackType == AttackType::RapidFireCannon)) { return false; }

	for (const std::unique_ptr<GalaxyCannon>& cannon : cannons) {
		//��C�����Ɏ��S���Ă������΂�
		if (cannon->GetIsDead()) { continue; }

		cannon->AttackTypeRapidFire();
	}

	return true;
}

bool Galaxy::AttackTypeA()
{
	//�U�����e��a�łȂ���Δ�����
	if (!(attackType == AttackType::a)) { return false; }



	return true;
}

bool Galaxy::AttackTypeFlamethrowerBow()
{
	//�U�����e���Ή�����(�D��)�łȂ���Δ�����
	if (!(attackType == AttackType::FlamethrowerBow)) { return false; }

	//�e���˂̕W�I���W���Z�b�g
	const Vector3 targetPos = player->GetWorldPos();
	bow->AttackTypeFlamethrower(targetPos);

	return true;
}

bool Galaxy::WaitMode()
{
	//�ҋ@��ԂłȂ���Δ�����
	if (!(phase == Phase::Wait)) { return false; }

	//�^�C�}�[���X�V
	waitModeTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (waitModeTimer >= waitModeTime) {
		phase = Phase::Attack;
		//�U���J�n�p�A�ϐ��Ȃǂ�������
		AttackModeStart();

		//���ɑҋ@��ԂɂȂ����Ƃ��̂��߂Ƀ^�C�}�[�����������Ă���
		waitModeTimer = 0;
	}

	return true;
}

bool Galaxy::AttackPartChangeMode()
{
	//�U������p�[�c�ύX��ԂłȂ���Δ�����
	if (!(phase == Phase::AttackPartChange)) { return false; }

	//���̂̍U������p�[�c�ύX�s��
	body->AttackPartChange();

	//���̂���]������s�����Ȃ甲����
	if (body->GetIsAttackPartChangeRota()) { return true; }

	//�U������p�[�c����C�̏ꍇ�A�U������p�[�c��D��ɕύX
	if (attackPartPhase == AttackPartPhase::Cannon) { attackPartPhase = AttackPartPhase::Front; }

	//�U������p�[�c�̕ύX�����������̂ŁA�U����Ԃɂ���
	phase = Phase::Attack;
	//�U���J�n�p�A�ϐ��Ȃǂ�������
	AttackModeStart();

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

void Galaxy::AttackModeStart()
{
	//�U������p�[�c����C�̂Ƃ��͑�C�̍U����Ԃ��J�n������
	if (attackPartPhase == AttackPartPhase::Cannon) {
		for (const std::unique_ptr<GalaxyCannon>& cannon : cannons) {
			//��C�����Ɏ��S���Ă������΂�
			if (cannon->GetIsDead()) { continue; }

			cannon->AttackModeStart();
		}
	}
	//�U������p�[�c���D��̂Ƃ��͑D��̍U����Ԃ��J�n������
	else if (attackPartPhase == AttackPartPhase::Front) {
		bow->AttackModeStart();
	}

	//�U�����e�𖢐ݒ�ɂ��Ă���
	attackType = AttackType::None;
}

void Galaxy::CheckAllCannonDead()
{
	//��̂ł������Ă����甲����
	for (const std::unique_ptr<GalaxyCannon>& cannon : cannons) {
		if (!cannon->GetIsDead()) {
			return;
		}
	}

	//�U������p�[�c��ύX�����]���J�n
	const float rotSpeed = -0.5f;
	const float changeRota = 0;
	body->AttackPartChangeRotaStart(rotSpeed, changeRota);

	//�U������p�[�c��ύX�����Ԃɂ���
	phase = Phase::AttackPartChange;
}
