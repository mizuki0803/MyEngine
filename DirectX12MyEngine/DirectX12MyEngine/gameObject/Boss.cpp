#include "Boss.h"
#include "Input.h"
#include "DebugText.h"
#include "Player.h"
#include "BossAvatarType01.h"
#include "BossAvatarType02.h"
#include "BossAvatarType03.h"
#include "BossAvatarType04.h"

Player* Boss::player = nullptr;
const float Boss::attackModeTime = 600.0f;
const float Boss::waitModeTime = 480.0f;
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

	//HP�o�[����
	const Vector2 hpBarPosition = { 20, 120 };
	hpBar.reset(BossHPBar::Create(4, hpBarPosition, maxHP));
	//HP�o�[�t���[������
	const float posDiff = 3.0f;	//HP�o�[�̍��W�Ƃ̍���
	const Vector2 hpFramePosition = { hpBarPosition.x - posDiff, hpBarPosition.y - posDiff };
	hpFrame.reset(BossHPFrame::Create(5, hpFramePosition));

	//�r�w�C�r�A�c���[����
	behaviorTree.reset(BossBehaviorTree::Create(this));

	return true;
}

void Boss::Update()
{
	//���S�������g�̍폜
	avatars.remove_if([](std::unique_ptr<BossAvatar>& avatar) {
		return avatar->GetIsDead();
		});

	//�r�w�C�r�A�c���[�ɂ��s���J��
	behaviorTree->Root();

	//�X�V
	mainBody->Update();//�{��
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		avatar->Update();//���g
	}

	//HP�o�[�X�V
	hpBar->Update();
	//HP�o�[�t���[���X�V
	hpFrame->Update();

	if (phase == Phase::Dead) {
		DebugText::GetInstance()->Print("BossDeadMode", 100, 60);
	}

	std::string hpNum = std::to_string(HP);
	DebugText::GetInstance()->Print("HP : " + hpNum, 100, 80);
	std::string avatarNum = std::to_string(avatars.size());
	DebugText::GetInstance()->Print("AvatarNum : " + avatarNum, 100, 100);
}

void Boss::Draw()
{
	//�`��
	mainBody->Draw();//�{��
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		avatar->Draw();//���g
	}
}

void Boss::DrawUI()
{
	//HP�o�[�t���[���`��
	hpFrame->Draw();
	//HP�o�[�`��
	hpBar->Draw();
}

void Boss::OnCollisionMainBody(const int damageNum)
{
	//�ҋ@��ԂȂ�_���[�W����炤
	const bool isWaitMode = (phase == Phase::Wait);
	//���g���S�Ď���ł���Ȃ�_���[�W����炤
	const bool isAllAvatarDead = (avatars.size() == 0);
	//y���p�x45�x�ȉ��Ȃ�_���[�W����炤
	const bool isDamageRota = (mainBody->GetRotation().y <= 45.0f);

	//�ǂꂩ��ł����ڂ�B�����Ă��Ȃ���΃_���[�W���󂯂Ȃ��Ŕ�����
	if (!(isWaitMode && isAllAvatarDead && isDamageRota)) { return; }

	//�{�̂Ƀ_���[�W
	mainBody->Damage(damageNum);

	//�{�X�S�̂Ƀ_���[�W(�{�X�{�̂����ۂɋ������_���[�W�ʂ��Z�b�g)
	const int mainBodyDamageNum = mainBody->GetDamageNum();
	Damage(mainBodyDamageNum);
}

void Boss::OnCollisionAvatar(BossAvatar* avatar, const int damageNum)
{
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

bool Boss::FallMode()
{
	//�~����ԂłȂ���Δ�����
	if (!(phase == Phase::Fall)) { return false; }

	//�^�C�}�[���X�V
	const float fallModeTime = 300;
	fallModeTimer++;
	const float time = fallModeTimer / fallModeTime;

	//�{�X�{�̂��~��������
	mainBody->FallMode(time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (fallModeTimer >= fallModeTime) {
		phase = Phase::Attack;

		//���g�̃��f�����N���Ă����Ԃ̃��f���ɕύX����
		for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
			avatar->ChangeModel();
		}
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
		avatar->ChangeWaitMode(time);
	}

	return true;
}

bool Boss::DeadMode()
{
	//���S��ԂłȂ���Δ�����
	if (!(phase == Phase::Dead)) { return false; }

	//�^�C�}�[���X�V
	const float deadModeTime = 300;
	deadModeTimer++;

	//�{�X�{�̂Ɏ��S���̓�����������
	mainBody->DeadMode();

	//�^�C�}�[���w�肵�����ԂɂȂ����玀�S�t���O�𗧂Ă�(�폜)
	if (deadModeTimer >= deadModeTime) {
		isDead = true;
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
	hpBar->ChangeLength(HP);
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
		avatar->AttackEnd();
	}
}
