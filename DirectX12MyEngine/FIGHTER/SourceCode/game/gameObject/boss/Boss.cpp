#include "Boss.h"
#include "Input.h"
#include "SpriteTextureLoader.h"
#include "Player.h"
#include "BossAvatarType01.h"
#include "BossAvatarType02.h"
#include "BossAvatarType03.h"
#include "BossAvatarType04.h"
#include "EnemyDefeatCounter.h"
#include "ParticleEmitter.h"

Player* Boss::player = nullptr;
const float Boss::appearModeTime = 560.0f;
const float Boss::attackModeTime = 600.0f;
const float Boss::waitModeTime = 500.0f;
const float Boss::changeModeTime = 60.0f;
const float Boss::returnBasePositionTime = 180.0f;

Boss* Boss::Create(const Vector3& position)
{
	//�{�X�̃C���X�^���X�𐶐�
	Boss* boss = new Boss();
	if (boss == nullptr) {
		return nullptr;
	}

	// ������
	if (!boss->Initialize(position)) {
		delete boss;
		assert(0);
		return nullptr;
	}

	return boss;
}

bool Boss::Initialize(const Vector3& position)
{
	//�{�̐���
	mainBody.reset(BossMainBody::Create(position));
	//�{�X���g����
	std::unique_ptr<BossAvatarType01> newAvatarType01;
	newAvatarType01.reset(BossAvatarType01::Create(mainBody.get(), { 2, 0, 0 }));
	avatars.push_back(std::move(newAvatarType01));
	std::unique_ptr<BossAvatarType02> newAvatarType02;
	newAvatarType02.reset(BossAvatarType02::Create(mainBody.get(), { -2, 0, 0 }));
	avatars.push_back(std::move(newAvatarType02));
	std::unique_ptr<BossAvatarType03> newAvatarType03;
	newAvatarType03.reset(BossAvatarType03::Create(mainBody.get(), { 0, 2, 0 }));
	avatars.push_back(std::move(newAvatarType03));
	std::unique_ptr<BossAvatarType04> newAvatarType04;
	newAvatarType04.reset(BossAvatarType04::Create(mainBody.get(), { 0, -2, 0 }));
	avatars.push_back(std::move(newAvatarType04));

	//HP��{�̂ƃ{�X���g�̍��v�ŎZ�o����
	int maxHP = BossMainBody::GetMaxHP() + BossAvatar::GetMaxHP() * (int)avatars.size();
	HP = maxHP;

	//�r�w�C�r�A�c���[����
	behaviorTree.reset(BossBehaviorTree::Create(this));

	return true;
}

void Boss::Update()
{
	//���S�������g�̍폜
	avatars.remove_if([](std::unique_ptr<BossAvatar>& avatar) {
		return avatar->GetIsDelete();
		});

	//�r�w�C�r�A�c���[�ɂ��s���J��
	behaviorTree->Root();

	//�X�V
	mainBody->Update();//�{��
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		avatar->Update();//���g
	}

	//HPUI�X�V
	if (hpUI) {
		hpUI->Update();
	}
	//�{�X���\��UI�X�V
	if (bossNameUI) {
		bossNameUI->Update();
	}

}

void Boss::Draw()
{
	//�`��
	mainBody->Draw();//�{��
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		avatar->Draw();//���g
	}
}

void Boss::DrawLightCameraView()
{
	//�e�p�������C�g���猩�����_�ł̕`��
	mainBody->DrawLightCameraView();//�{��
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		avatar->DrawLightCameraView();//���g
	}
}

void Boss::DrawUI()
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

void Boss::OnCollisionMainBody(const int damageNum)
{
	//�ҋ@��ԂȂ�_���[�W����炤
	const bool isWaitMode = (phase == Phase::Wait);
	//y���p�x45�x�ȉ��Ȃ�_���[�W����炤
	const bool isDamageRota = (mainBody->GetRotation().y <= 45.0f);

	//�{�̂��U����ԂȂ�_���[�W����炤��ǉ����A�ǂꂩ��ł����ڂ�B�����Ă��Ȃ���΃_���[�W���󂯂Ȃ��Ŕ�����
	if (!(isMainBodyAttackMode && isWaitMode && isDamageRota)) { return; }

	//�{�̂Ƀ_���[�W
	mainBody->Damage(damageNum);

	//�{�X�S�̂Ƀ_���[�W(�{�X�{�̂����ۂɋ������_���[�W�ʂ��Z�b�g)
	const int mainBodyDamageNum = mainBody->GetDamageNum();
	Damage(mainBodyDamageNum);
}

void Boss::OnCollisionAvatar(BossAvatar* avatar, const int damageNum)
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
	avatar->Damage(damageNum);

	//���g���S�ł������`�F�b�N
	CheckAllAvatarDead();

	//�{�X�S�̂Ƀ_���[�W(�{�X���g�����ۂɋ������_���[�W�ʂ��Z�b�g)
	const int avatarBodyDamageNum = avatar->GetDamageNum();
	Damage(avatarBodyDamageNum);
}

bool Boss::AppearModeCount()
{
	//�o���ԂłȂ���Δ�����
	if (!(phase == Phase::Appear)) { return false; }

	//�^�C�}�[���X�V
	appearModeTimer++;

	//�w�肵�����ԂɂȂ�����{�X���\��UI����
	const float bossNameUICreateTime = 150;
	if (appearModeTimer >= bossNameUICreateTime && !bossNameUI) {
		bossNameUI.reset(BossNameUI::Create(0));
	}

	//�w�肵�����ԂɂȂ�����HP�o�[����
	const float hpUICreateTime = appearModeTime - 90;
	if (appearModeTimer >= hpUICreateTime && !hpUI) {
		const Vector2 hpUIPosition = { 30, 150 };
		hpUI.reset(BossHPUI::Create(hpUIPosition, HP));
	}

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (appearModeTimer >= appearModeTime) {
		phase = Phase::Attack;

		//���g�̃��f�����N���Ă����Ԃ̃��f���ɕύX����
		for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
			avatar->ChangeModel();
		}

		//�{�X���\��UI�͂����g�p���Ȃ��̂ŉ�����Ă���
		bossNameUI.reset();
	}

	return true;
}

bool Boss::AppearFall()
{
	//�~���ɂ����鎞��
	const float fallTime = 400;
	//���ɍ~���ɂ����鎞�Ԉȏ�Ȃ甲����
	if (appearModeTimer > fallTime) { return false; }

	//�C�[�W���O�p��0�`1�̒l���Z�o����
	const float time = appearModeTimer / fallTime;

	//�{�X�{�̂��~��������
	mainBody->Fall(time);

	return true;
}

bool Boss::AttackModeCount()
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

bool Boss::UpdateBulletShotPos()
{
	//�U����ԂłȂ���Δ�����
	if (!(phase == Phase::Attack)) { return false; }

	//�{�̂ƕ��g�̔��ˍ��W���X�V
	mainBody->UpdateBulletShotPos();
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		//���g�����Ɏ��S���Ă������΂�
		if (avatar->GetIsDead()) { continue; }

		avatar->UpdateBulletShotPos();
	}

	return true;
}

bool Boss::AttackTypeSelectStart()
{
	//�U�����e�����Ɍ��܂��Ă�����true��Ԃ�
	if (!(attackType == AttackType::None)) { return true; }

	//�U�����e�ݒ���J�n���邽��false��Ԃ�
	return false;
}

bool Boss::AttackTypeTrackingSelect()
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

bool Boss::AttackTypeSuperGiantBulletSelect()
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

bool Boss::AttackTypeRotateSelect()
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

bool Boss::AttackTypeAvatarBodyBlowSelect()
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

bool Boss::AttackTypeAvatarGatlingSelect()
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

bool Boss::AttackTypeAvatarGiantBulletSelect()
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

bool Boss::AttackTypeTracking()
{
	//�U�����e���Ǐ]�łȂ���Δ�����
	if (!(attackType == AttackType::Tracking)) { return false; }

	mainBody->AttackTypeTracking(player->GetWorldPos());

	return true;
}

bool Boss::AttackTypeSuperGiantBullet()
{
	//�U�����e��������e�łȂ���Δ�����
	if (!(attackType == AttackType::SuperGiantBullet)) { return false; }

	mainBody->AttackTypeSuperGiantBullet();

	return true;
}

bool Boss::AttackTypeRotate()
{
	//�U�����e����]�łȂ���Δ�����
	if (!(attackType == AttackType::Rotate)) { return false; }

	mainBody->AttackTypeRotate();

	return true;
}

bool Boss::AttackTypeAvatarBodyBlow()
{
	//�U�����e�����g�̓�����łȂ���Δ�����
	if (!(attackType == AttackType::AvatarBodyBlow)) { return false; }

	//�{�̂ƕ��g���U�����e:���g�̓�����œ�����
	mainBody->AttackTypeAvatarBodyBlow();
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		//���g�����Ɏ��S���Ă������΂�
		if (avatar->GetIsDead()) { continue; }

		avatar->AttackTypeAvatarBodyBlow(player->GetWorldPos());
	}

	return true;
}

bool Boss::AttackTypeAvatarGatling()
{
	//�U�����e�����g�K�g�����O�C�łȂ���Δ�����
	if (!(attackType == AttackType::AvatarGatling)) { return false; }

	//�{�̂ƕ��g���U�����e:���g�K�g�����O�C�œ�����
	mainBody->AttackTypeAvatarGatling(player->GetWorldPos());
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		//���g�����Ɏ��S���Ă������΂�
		if (avatar->GetIsDead()) { continue; }

		avatar->AttackTypeAvatarGatling();
	}

	return true;
}

bool Boss::AttackTypeAvatarGiantBullet()
{
	//�U�����e�����g����e�łȂ���Δ�����
	if (!(attackType == AttackType::AvatarGiantBullet)) { return false; }

	//�{�̂ƕ��g���U�����e:���g����e�œ�����
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		//���g�����Ɏ��S���Ă������΂�
		if (avatar->GetIsDead()) { continue; }

		avatar->AttackTypeAvatarGiantBullet();
	}

	return true;
}

bool Boss::AttackModeMainBodyRota()
{
	//�{�̂��U����ԂłȂ���Δ�����
	if (!isMainBodyAttackMode) { return false; }

	//�^�C�}�[�����[�h�`�F���W���Ԉȉ��łȂ��Ȃ甲����
	if (!(attackModeTimer < changeModeTime)) { return false; }

	//�C�[�W���O�p��0�`1�̒l���Z�o����
	const float time = attackModeTimer / changeModeTime;

	//�U����Ԃɂ��邽�ߖ{�̂���]������
	mainBody->ChangeAttackMode(time);

	return true;
}

bool Boss::AttackModeAvatarRota()
{
	//�{�̂��U����ԂłȂ甲����
	if (isMainBodyAttackMode) { return false; }

	//�^�C�}�[�����[�h�`�F���W���Ԉȉ��łȂ��Ȃ甲����
	if (!(attackModeTimer < changeModeTime)) { return false; }

	//�C�[�W���O�p��0�`1�̒l���Z�o����
	const float time = attackModeTimer / changeModeTime;

	//�U����Ԃɂ��邽�ߕ��g����]������
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		//���g�����Ɏ��S���Ă������΂�
		if (avatar->GetIsDead()) { continue; }

		avatar->ChangeAttackMode(time);
	}

	return true;
}

bool Boss::WaitModeCount()
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

		//�ҋ@��Ԃ��I����̂�false��Ԃ�
		return false;
	}

	return true;
}

bool Boss::ReturnBasePosition()
{
	//�^�C�}�[����ʒu�ɖ߂邽�߂ɗv���鎞�Ԉȉ��łȂ��Ȃ甲����
	const float timer = waitModeTimer - changeModeTime;
	if (!(timer < returnBasePositionTime)) { return false; }

	//�C�[�W���O�p��0�`1�̒l���Z�o����
	const float time = timer / returnBasePositionTime;

	//��ʒu�ɖ߂�
	mainBody->ReturnBasePosition(time);
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		//���g�����Ɏ��S���Ă������΂�
		if (avatar->GetIsDead()) { continue; }

		avatar->ReturnBasePosition(time);
	}

	return true;
}

bool Boss::WaitModeMainBodyRota()
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

bool Boss::WaitModeAvatarRota()
{
	//�{�̂��U����ԂłȂ甲����
	if (isMainBodyAttackMode) { return false; }

	//�^�C�}�[�����[�h�`�F���W���Ԉȉ��łȂ��Ȃ甲����
	if (!(waitModeTimer < changeModeTime)) { return false; }

	//�C�[�W���O�p��0�`1�̒l���Z�o����
	const float time = waitModeTimer / changeModeTime;

	//�ҋ@��Ԃɂ��邽�ߕ��g����]������
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		//���g�����Ɏ��S���Ă������΂�
		if (avatar->GetIsDead()) { continue; }

		avatar->ChangeWaitMode(time);
	}

	return true;
}

bool Boss::DeadExplosion()
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
		const float distance = 5.0f;
		particlePos.x += (float)rand() / RAND_MAX * distance - distance / 2.0f;
		particlePos.y += (float)rand() / RAND_MAX * distance - distance / 2.0f;
		particlePos.z += (float)rand() / RAND_MAX * distance - distance / 2.0f;
		ParticleEmitter::GetInstance()->Explosion(particlePos);

		//�������o�񐔍X�V
		explosionCount++;

		//�������w��񐔂ɒB�����玀�S�t���O�𗧂Ă�
		if (explosionCount >= explosionNum) {
			isDead = true;

			//�|�������J�E���^�[�𑝂₷(�{�� + ���g��)
			const int defeatNum = 5;
			EnemyDefeatCounter::AddCounter(defeatNum);
		}
	}

	//�C�[�W���O�p��0�`1�̒l���Z�o����
	const float backTime = 180;
	float time = explosionTimer / backTime;
	time = min(time, 1.0f);
	//�{�X�{�̂���̈ʒu�ɖ߂�
	mainBody->DeadBasePosBack(time);

	return true;
}

bool Boss::DeadFall()
{
	//���S��ԂłȂ���Δ�����
	if (!(phase == Phase::Dead)) { return false; }

	//�{�X�{�̂Ɏ��S��������������
	mainBody->DeadFall();

	//�{�̂��폜��ԂȂ�폜
	if (mainBody->GetIsDelete()) {
		isDelete = true;

		//�������o�p�p�[�e�B�N������
		Vector3 particlePos = mainBody->GetWorldPos();
		const float distance = 5.0f;
		particlePos.x += (float)rand() / RAND_MAX * distance - distance / 2.0f;
		particlePos.y += (float)rand() / RAND_MAX * distance - distance / 2.0f;
		particlePos.z += (float)rand() / RAND_MAX * distance - distance / 2.0f;
		ParticleEmitter::GetInstance()->Explosion(particlePos);
	}

	return true;
}

void Boss::Damage(const int damageNum)
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

void Boss::CheckAllAvatarDead()
{
	//���ɖ{�̂��U����ԂȂ甲����
	if (isMainBodyAttackMode) { return; }

	//��̂ł������Ă����甲����
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
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

void Boss::AttackEnd()
{
	//�U����ԏI������̂ŕK�v�ȏ����Z�b�g
	mainBody->AttackEnd();
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		//���g�����Ɏ��S���Ă������΂�
		if (avatar->GetIsDead()) { continue; }

		avatar->AttackEnd();
	}
}
