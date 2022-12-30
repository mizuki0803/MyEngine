#include "Medaman.h"
#include "Input.h"
#include "SpriteTextureLoader.h"
#include "BasePlayer.h"
#include "MedamanAvatarType01.h"
#include "MedamanAvatarType02.h"
#include "MedamanAvatarType03.h"
#include "MedamanAvatarType04.h"
#include "EnemyDefeatCounter.h"
#include "ParticleEmitter.h"

BasePlayer* Medaman::player = nullptr;
const float Medaman::appearModeTime = 600.0f;
const float Medaman::attackModeTime = 600.0f;
const float Medaman::waitModeTime = 500.0f;
const float Medaman::changeModeTime = 60.0f;
const float Medaman::returnBasePositionTime = 180.0f;

Medaman* Medaman::Create(const Vector3& position)
{
	//���_�}�[���̃C���X�^���X�𐶐�
	Medaman* medaman = new Medaman();
	if (medaman == nullptr) {
		return nullptr;
	}

	// ������
	if (!medaman->Initialize(position)) {
		delete medaman;
		assert(0);
		return nullptr;
	}

	return medaman;
}

bool Medaman::Initialize(const Vector3& position)
{
	//�{�̐���
	mainBody.reset(MedamanMainBody::Create(position));
	//���_�}�[�����g����
	std::unique_ptr<MedamanAvatarType01> newAvatarType01;
	newAvatarType01.reset(MedamanAvatarType01::Create(mainBody.get(), { 2, 0, 0 }));
	avatars.push_back(std::move(newAvatarType01));
	std::unique_ptr<MedamanAvatarType02> newAvatarType02;
	newAvatarType02.reset(MedamanAvatarType02::Create(mainBody.get(), { -2, 0, 0 }));
	avatars.push_back(std::move(newAvatarType02));
	std::unique_ptr<MedamanAvatarType03> newAvatarType03;
	newAvatarType03.reset(MedamanAvatarType03::Create(mainBody.get(), { 0, 2, 0 }));
	avatars.push_back(std::move(newAvatarType03));
	std::unique_ptr<MedamanAvatarType04> newAvatarType04;
	newAvatarType04.reset(MedamanAvatarType04::Create(mainBody.get(), { 0, -2, 0 }));
	avatars.push_back(std::move(newAvatarType04));

	//HP��{�̂ƕ��g�̍��v�ŎZ�o����
	int maxHP = MedamanMainBody::GetMaxHP() + MedamanAvatar::GetMaxHP() * (int)avatars.size();
	HP = maxHP;

	//�r�w�C�r�A�c���[����
	behaviorTree.reset(MedamanBehaviorTree::Create(this));

	return true;
}

void Medaman::Update()
{
	//���S�������g�̍폜
	avatars.remove_if([](std::unique_ptr<MedamanAvatar>& avatar) {
		return avatar->GetIsDelete();
		});

	//�X�V
	mainBody->Update();//�{��
	for (const std::unique_ptr<MedamanAvatar>& avatar : avatars) {
		avatar->Update();//���g
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

void Medaman::Draw()
{
	//�`��
	mainBody->Draw();//�{��
	for (const std::unique_ptr<MedamanAvatar>& avatar : avatars) {
		avatar->Draw();//���g
	}
}

void Medaman::DrawLightCameraView()
{
	//�e�p�������C�g���猩�����_�ł̕`��
	mainBody->DrawLightCameraView();//�{��
	for (const std::unique_ptr<MedamanAvatar>& avatar : avatars) {
		avatar->DrawLightCameraView();//���g
	}
}

void Medaman::DrawTopLightCameraView()
{
	//���ォ��̉e�p�������C�g���猩�����_�ł̕`��
	mainBody->DrawTopLightCameraView();//�{��
	for (const std::unique_ptr<MedamanAvatar>& avatar : avatars) {
		avatar->DrawTopLightCameraView();//���g
	}
}

void Medaman::DrawUI()
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

void Medaman::OnCollisionMainBody(const int damageNum, const Vector3& collisionPos, const Vector3& subjectVel)
{
	//�ҋ@��ԂȂ�_���[�W����炤
	const bool isWaitMode = (phase == Phase::Wait);
	//y���p�x45�x�ȉ��Ȃ�_���[�W����炤
	const bool isDamageRota = (mainBody->GetRotation().y <= 45.0f);

	//�{�̂��U����ԂȂ�_���[�W����炤��ǉ����A�ǂꂩ��ł����ڂ�B�����Ă��Ȃ���΃_���[�W���󂯂Ȃ��Ŕ�����
	if (!(isMainBodyAttackMode && isWaitMode && isDamageRota)) { return; }

	//�{�̂Ƀ_���[�W
	mainBody->Damage(damageNum, collisionPos, subjectVel);

	//���_�}�[���S�̂Ƀ_���[�W(���_�}�[���{�̂����ۂɋ������_���[�W�ʂ��Z�b�g)
	const int mainBodyDamageNum = mainBody->GetDamageNum();
	Damage(mainBodyDamageNum);
}

void Medaman::OnCollisionAvatar(MedamanAvatar* avatar, const int damageNum, const Vector3& collisionPos, const Vector3& subjectVel)
{
	//���g�����Ɏ��S���Ă����甲����
	if (avatar->GetIsDead()) { return; }

	//�ҋ@��ԂȂ�_���[�W����炤
	const bool isWaitMode = (phase == Phase::Wait);
	//y���p�x45�x�ȉ��Ȃ�_���[�W����炤
	const bool isDamageRota = (avatar->GetRotation().y <= 45.0f);

	//�ǂ��炩��ł����ڂ�B�����Ă��Ȃ���΃_���[�W���󂯂Ȃ��Ŕ�����
	if (!(isWaitMode && isDamageRota)) { return; }

	//�Փ˂������g�Ƀ_���[�W
	avatar->Damage(damageNum, collisionPos, subjectVel);

	//���g���S�ł������`�F�b�N
	CheckAllAvatarDead();

	//���_�}�[���S�̂Ƀ_���[�W(���_�}�[�����g�����ۂɋ������_���[�W�ʂ��Z�b�g)
	const int avatarBodyDamageNum = avatar->GetDamageNum();
	Damage(avatarBodyDamageNum);
}

bool Medaman::AppearModeCount()
{
	//�o���ԂłȂ���Δ�����
	if (!(phase == Phase::Appear)) { return false; }

	//�^�C�}�[���X�V
	appearModeTimer++;

	//�w�肵�����ԂɂȂ�����{�X���\��UI����
	const float bossNameUICreateTime = 160;
	if (appearModeTimer >= bossNameUICreateTime && !bossNameUI) {
		bossNameUI.reset(BossNameUI::Create(0));
	}

	//�w�肵�����ԂɂȂ�����HP�o�[����
	const float hpUICreateTime = appearModeTime - 100;
	if (appearModeTimer >= hpUICreateTime && !hpUI) {
		const Vector2 hpUIPosition = { 30, 170 };
		hpUI.reset(BossHPUI::Create(hpUIPosition, HP));
	}

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (appearModeTimer >= appearModeTime) {
		phase = Phase::Attack;

		//���g�̃��f�����N���Ă����Ԃ̃��f���ɕύX����
		for (const std::unique_ptr<MedamanAvatar>& avatar : avatars) {
			avatar->ChangeModel();
		}

		//����W�ɖ߂�����Z�b�g
		SetReturnBasePosition();

		//�{�X���\��UI�͂����g�p���Ȃ��̂ŉ�����Ă���
		bossNameUI.reset();
	}

	return true;
}

bool Medaman::AppearFall()
{
	//�~���ɂ����鎞��
	const float fallTime = 400;
	//���ɍ~���ɂ����鎞�Ԉȏ�Ȃ甲����
	if (appearModeTimer > fallTime) { return false; }

	//�C�[�W���O�p��0�`1�̒l���Z�o����
	const float time = appearModeTimer / fallTime;

	//���_�}�[���{�̂��~��������
	mainBody->Fall(time);

	return true;
}

bool Medaman::AttackModeCount()
{
	//�U����ԂłȂ���Δ�����
	if (!(phase == Phase::Attack)) { return false; }

	//�^�C�}�[���X�V
	attackModeTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (attackModeTimer >= attackModeTime) {
		phase = Phase::Wait;

		//���ɍU����ԂɂȂ����Ƃ��̂��߂Ƀ^�C�}�[�����������Ă���
		attackModeTimer = 0;
		//�U�����e�𖢐ݒ�ɂ��Ă���
		attackType = AttackType::None;

		//�U����ԏI������̂ŕK�v�ȏ����Z�b�g
		AttackEnd();

		//�U����Ԃ��I����̂�false��Ԃ�
		return false;
	}

	return true;
}

bool Medaman::UpdateBulletShotPos()
{
	//�U����ԂłȂ���Δ�����
	if (!(phase == Phase::Attack)) { return false; }

	//�{�̂ƕ��g�̔��ˍ��W���X�V
	mainBody->UpdateBulletShotPos();
	for (const std::unique_ptr<MedamanAvatar>& avatar : avatars) {
		//���g�����Ɏ��S���Ă������΂�
		if (avatar->GetIsDead()) { continue; }

		avatar->UpdateBulletShotPos();
	}

	return true;
}

bool Medaman::AttackTypeSelectStart()
{
	//�U�����e�����Ɍ��܂��Ă�����true��Ԃ�
	if (!(attackType == AttackType::None)) { return true; }

	//�U�����e�ݒ���J�n���邽��false��Ԃ�
	return false;
}

bool Medaman::AttackTypeTrackingSelect()
{
	//�{�̂��U����ԂłȂ���Δ�����
	if (!isMainBodyAttackMode) { return false; }

	//�O��̍U�����e���Ǐ]�������甲����
	if (preAttackType == AttackType::Tracking) { return false; }

	//�v���C���[���@����ʍ����ɂ����甲����
	if (player->GetPosition().x <= 0) { return false; }

	//�U�����e:�Ǐ]���Z�b�g
	attackType = AttackType::Tracking;
	//1�O�̍U�����e���X�V
	preAttackType = AttackType::Tracking;

	return true;
}

bool Medaman::AttackTypeSuperGiantBulletSelect()
{
	//�{�̂��U����ԂłȂ���Δ�����
	if (!isMainBodyAttackMode) { return false; }

	//�O��̍U�����e��������e�������甲����
	if (preAttackType == AttackType::SuperGiantBullet) { return false; }

	//�U�����e:������e���Z�b�g
	attackType = AttackType::SuperGiantBullet;
	//1�O�̍U�����e���X�V
	preAttackType = AttackType::SuperGiantBullet;

	return true;
}

bool Medaman::AttackTypeRotateSelect()
{
	//�{�̂��U����ԂłȂ���Δ�����
	if (!isMainBodyAttackMode) { return false; }

	//�O��̍U�����e����]�������甲����
	if (preAttackType == AttackType::Rotate) { return false; }

	//�U�����e:��]���Z�b�g
	attackType = AttackType::Rotate;
	//1�O�̍U�����e���X�V
	preAttackType = AttackType::Rotate;

	return true;
}

bool Medaman::AttackTypeAvatarBodyBlowSelect()
{
	//�{�̂��U����ԂłȂ甲����
	if (isMainBodyAttackMode) { return false; }

	//�O��̍U�����e�����g�̓����肾�����甲����
	if (preAttackType == AttackType::AvatarBodyBlow) { return false; }

	//�v���C���[���@����ʉE���ɂ����甲����
	if (player->GetPosition().x <= 0) { return false; }

	//�U�����e:���g�̓�������Z�b�g
	attackType = AttackType::AvatarBodyBlow;
	//1�O�̍U�����e���X�V
	preAttackType = AttackType::AvatarBodyBlow;

	return true;
}

bool Medaman::AttackTypeAvatarGatlingSelect()
{
	//�{�̂��U����ԂłȂ甲����
	if (isMainBodyAttackMode) { return false; }

	//�O��̍U�����e�����g�K�g�����O�C�������甲����
	if (preAttackType == AttackType::AvatarGatling) { return false; }

	//�U�����e:���g�K�g�����O�C���Z�b�g
	attackType = AttackType::AvatarGatling;
	//1�O�̍U�����e���X�V
	preAttackType = AttackType::AvatarGatling;

	return true;
}

bool Medaman::AttackTypeAvatarGiantBulletSelect()
{
	//�{�̂��U����ԂłȂ甲����
	if (isMainBodyAttackMode) { return false; }

	//�O��̍U�����e�����g����e�������甲����
	if (preAttackType == AttackType::AvatarGiantBullet) { return false; }

	//�U�����e:���g����e���Z�b�g
	attackType = AttackType::AvatarGiantBullet;
	//1�O�̍U�����e���X�V
	preAttackType = AttackType::AvatarGiantBullet;

	return true;
}

bool Medaman::AttackTypeTracking()
{
	//�U�����e���Ǐ]�łȂ���Δ�����
	if (!(attackType == AttackType::Tracking)) { return false; }

	mainBody->AttackTypeTracking(player->GetWorldPos());

	return true;
}

bool Medaman::AttackTypeSuperGiantBullet()
{
	//�U�����e��������e�łȂ���Δ�����
	if (!(attackType == AttackType::SuperGiantBullet)) { return false; }

	mainBody->AttackTypeSuperGiantBullet();

	return true;
}

bool Medaman::AttackTypeRotate()
{
	//�U�����e����]�łȂ���Δ�����
	if (!(attackType == AttackType::Rotate)) { return false; }

	mainBody->AttackTypeRotate();

	return true;
}

bool Medaman::AttackTypeAvatarBodyBlow()
{
	//�U�����e�����g�̓�����łȂ���Δ�����
	if (!(attackType == AttackType::AvatarBodyBlow)) { return false; }

	//�{�̂ƕ��g���U�����e:���g�̓�����œ�����
	mainBody->AttackTypeAvatarBodyBlow();
	for (const std::unique_ptr<MedamanAvatar>& avatar : avatars) {
		//���g�����Ɏ��S���Ă������΂�
		if (avatar->GetIsDead()) { continue; }

		avatar->AttackTypeAvatarBodyBlow(player->GetWorldPos());
	}

	return true;
}

bool Medaman::AttackTypeAvatarGatling()
{
	//�U�����e�����g�K�g�����O�C�łȂ���Δ�����
	if (!(attackType == AttackType::AvatarGatling)) { return false; }

	//�{�̂ƕ��g���U�����e:���g�K�g�����O�C�œ�����
	mainBody->AttackTypeAvatarGatling(player->GetWorldPos());
	for (const std::unique_ptr<MedamanAvatar>& avatar : avatars) {
		//���g�����Ɏ��S���Ă������΂�
		if (avatar->GetIsDead()) { continue; }

		avatar->AttackTypeAvatarGatling();
	}

	return true;
}

bool Medaman::AttackTypeAvatarGiantBullet()
{
	//�U�����e�����g����e�łȂ���Δ�����
	if (!(attackType == AttackType::AvatarGiantBullet)) { return false; }

	//�{�̂ƕ��g���U�����e:���g����e�œ�����
	for (const std::unique_ptr<MedamanAvatar>& avatar : avatars) {
		//���g�����Ɏ��S���Ă������΂�
		if (avatar->GetIsDead()) { continue; }

		avatar->AttackTypeAvatarGiantBullet();
	}

	return true;
}

bool Medaman::AttackModeMainBodyRota()
{
	//�{�̂��U����ԂłȂ���Δ�����
	if (!isMainBodyAttackMode) { return false; }

	//�^�C�}�[�����[�h�`�F���W���Ԉȉ��łȂ��Ȃ甲����
	if (!(attackModeTimer < changeModeTime)) { return false; }

	//�C�[�W���O�p��0�`1�̒l���Z�o����
	const float time = attackModeTimer / changeModeTime;

	//�U����Ԃɂ��邽�ߖ{�̂���]������
	mainBody->ChangeAttackMode(time);

	//�m�b�N�o�b�N�ɂ���Ă��ꂽ���W������W�Ɉړ�������
	mainBody->ReturnBasePosition(time);

	return true;
}

bool Medaman::AttackModeAvatarRota()
{
	//�{�̂��U����ԂłȂ甲����
	if (isMainBodyAttackMode) { return false; }

	//�^�C�}�[�����[�h�`�F���W���Ԉȉ��łȂ��Ȃ甲����
	if (!(attackModeTimer < changeModeTime)) { return false; }

	//�C�[�W���O�p��0�`1�̒l���Z�o����
	const float time = attackModeTimer / changeModeTime;

	//�U����Ԃɂ��邽�ߕ��g����]������
	for (const std::unique_ptr<MedamanAvatar>& avatar : avatars) {
		//���g�����Ɏ��S���Ă������΂�
		if (avatar->GetIsDead()) { continue; }

		avatar->ChangeAttackMode(time);

		//�m�b�N�o�b�N�ɂ���Ă��ꂽ���W������W�Ɉړ�������
		avatar->ReturnBasePosition(time);
	}

	return true;
}

bool Medaman::WaitModeCount()
{
	//�ҋ@��ԂłȂ���Δ�����
	if (!(phase == Phase::Wait)) { return false; }

	//�^�C�}�[���X�V
	waitModeTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (waitModeTimer >= waitModeTime) {
		phase = Phase::Attack;

		//���ɑҋ@��ԂɂȂ����Ƃ��̂��߂Ƀ^�C�}�[�����������Ă���
		waitModeTimer = 0;

		//�ҋ@��ԏI������̂Ŋ���W�ɖ߂�����Z�b�g
		SetReturnBasePosition();

		//�ҋ@��Ԃ��I����̂�false��Ԃ�
		return false;
	}

	return true;
}

bool Medaman::ReturnBasePosition()
{
	//�^�C�}�[����ʒu�ɖ߂邽�߂ɗv���鎞�Ԉȉ��łȂ��Ȃ甲����
	const float timer = waitModeTimer - changeModeTime;
	if (!(timer < returnBasePositionTime)) { return false; }

	//�C�[�W���O�p��0�`1�̒l���Z�o����
	const float time = timer / returnBasePositionTime;

	//��ʒu�ɖ߂�
	mainBody->ReturnBasePosition(time);
	for (const std::unique_ptr<MedamanAvatar>& avatar : avatars) {
		//���g�����Ɏ��S���Ă������΂�
		if (avatar->GetIsDead()) { continue; }

		avatar->ReturnBasePosition(time);
	}

	return true;
}

bool Medaman::WaitModeMainBodyRota()
{
	//�{�̂��U����ԂłȂ���Δ�����
	if (!isMainBodyAttackMode) { return false; }

	//�^�C�}�[�����[�h�`�F���W���Ԉȉ��łȂ��Ȃ甲����
	if (!(waitModeTimer < changeModeTime)) { return false; }

	//�C�[�W���O�p��0�`1�̒l���Z�o����
	const float time = waitModeTimer / changeModeTime;

	//�ҋ@��Ԃɂ��邽�ߖ{�̂���]������
	mainBody->ChangeWaitMode(time);

	return true;
}

bool Medaman::WaitModeAvatarRota()
{
	//�{�̂��U����ԂłȂ甲����
	if (isMainBodyAttackMode) { return false; }

	//�^�C�}�[�����[�h�`�F���W���Ԉȉ��łȂ��Ȃ甲����
	if (!(waitModeTimer < changeModeTime)) { return false; }

	//�C�[�W���O�p��0�`1�̒l���Z�o����
	const float time = waitModeTimer / changeModeTime;

	//�ҋ@��Ԃɂ��邽�ߕ��g����]������
	for (const std::unique_ptr<MedamanAvatar>& avatar : avatars) {
		//���g�����Ɏ��S���Ă������΂�
		if (avatar->GetIsDead()) { continue; }

		avatar->ChangeWaitMode(time);
	}

	return true;
}

bool Medaman::DeadExplosion()
{
	//���S��ԂłȂ���Δ�����
	if (!(phase == Phase::Dead)) { return false; }
	//���ɔ����񐔂��\���Ȃ甲����
	const int explosionNum = 12;
	if (explosionCount >= explosionNum) { return false; }

	//�������o�����^�C�}�[�X�V
	const float explosionTime = 20;
	explosionTimer++;
	if ((explosionTimer % (int)explosionTime) == 0) {
		//�������o�p�p�[�e�B�N������
		Vector3 particlePos = mainBody->GetWorldPos();
		const float distance = 8.0f;
		particlePos.x += (float)rand() / RAND_MAX * distance - distance / 2.0f;
		particlePos.y += (float)rand() / RAND_MAX * distance - distance / 2.0f;
		particlePos.z += (float)rand() / RAND_MAX * distance - distance / 2.0f;
		const float size = 2.0f;
		ParticleEmitter::GetInstance()->Explosion(particlePos, size);

		//�������o�񐔍X�V
		explosionCount++;

		//�������w��񐔂ɒB�����玀�S�t���O�𗧂Ă�
		if (explosionCount >= explosionNum) {
			isDead = true;

			//�|�������J�E���^�[�𑝂₷(�{�� + ���g��)
			const int defeatNum = 5;
			EnemyDefeatCounter::AddCounter(defeatNum);

			//�{�̂̃��f����ύX
			mainBody->ChangeDeadModel();
		}
	}

	//�C�[�W���O�p��0�`1�̒l���Z�o����
	const float backTime = 180;
	float time = explosionTimer / backTime;
	time = min(time, 1.0f);
	//���_�}�[���{�̂���̈ʒu�ɖ߂�
	mainBody->DeadBasePosBack(time);

	return true;
}

bool Medaman::DeadFall()
{
	//���S��ԂłȂ���Δ�����
	if (!(phase == Phase::Dead)) { return false; }

	//���_�}�[���{�̂Ɏ��S��������������
	mainBody->DeadFall();

	//�{�̂��폜��ԂȂ�폜
	if (mainBody->GetIsDelete()) {
		isDelete = true;
	}

	return true;
}

void Medaman::Damage(const int damageNum)
{
	//�����̐����̕��_���[�W����炤
	HP -= damageNum;

	//HP��0�ȉ��ɂȂ����玀�S��Ԃɂ���
	if (HP <= 0) {
		HP = 0;

		phase = Phase::Dead;
	}

	//�_���[�W���������̂�HP�o�[�̒�����ύX����
	if (hpUI) {
		hpUI->Damage(HP);
	}
}

void Medaman::CheckAllAvatarDead()
{
	//���ɖ{�̂��U����ԂȂ甲����
	if (isMainBodyAttackMode) { return; }

	//��̂ł������Ă����甲����
	for (const std::unique_ptr<MedamanAvatar>& avatar : avatars) {
		if (!avatar->GetIsDead()) {
			return;
		}
	}

	//�{�̂��U����Ԃɂ���
	isMainBodyAttackMode = true;
	//�{�̂̃��f�����N���Ă����Ԃ̃��f���ɕύX����
	mainBody->ChangeModel();
	//�{�̂͑ҋ@���Ȃ̂ŁA�ҋ@��Ԃɂ��Ă���
	phase = Phase::Wait;

	//�U�����e�𖢐ݒ�ɂ��Ă���
	attackType = AttackType::None;
	//�U����ԂŖ{�̂��U�������ԂɂȂ��Ă��܂����ꍇ�̂��߂ɍU����ԃ^�C�}�[�����������Ă���
	attackModeTimer = 0;
}

void Medaman::AttackEnd()
{
	//�U����ԏI������̂ŕK�v�ȏ����Z�b�g
	mainBody->AttackEnd();
	for (const std::unique_ptr<MedamanAvatar>& avatar : avatars) {
		//���g�����Ɏ��S���Ă������΂�
		if (avatar->GetIsDead()) { continue; }

		avatar->AttackEnd();
	}
}

void Medaman::SetReturnBasePosition()
{
	//��ʒu�ɖ߂�Ƃ��̏o�����W���L�^����
	mainBody->SetReturnBasePosition();
	for (const std::unique_ptr<MedamanAvatar>& avatar : avatars) {
		//���g�����Ɏ��S���Ă������΂�
		if (avatar->GetIsDead()) { continue; }

		//����W�ɖ߂��ʒu���Z�b�g
		avatar->SetReturnBasePosition();
	}
}
