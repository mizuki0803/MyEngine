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
const float Boss::waitModeTime = 600.0f;
const float Boss::changeModeTime = 60.0f;
const float Boss::returnFixedPositionTime = 180.0f;

Boss* Boss::Create(ObjModel* mainBodyModel, ObjModel* avatarModel, const Vector3& position)
{
	//�{�X�̃C���X�^���X�𐶐�
	Boss* boss = new Boss();
	if (boss == nullptr) {
		return nullptr;
	}

	// ������
	if (!boss->Initialize(mainBodyModel, avatarModel, position)) {
		delete boss;
		assert(0);
		return nullptr;
	}

	return boss;
}

bool Boss::Initialize(ObjModel* mainBodyModel, ObjModel* avatarModel, const Vector3& position)
{
	//�{�̐���
	mainBody.reset(BossMainBody::Create(mainBodyModel, position));
	//�{�X���g����
	std::unique_ptr<BossAvatarType01> newAvatarType01;
	newAvatarType01.reset(BossAvatarType01::Create(avatarModel, mainBody.get(), { 5, 0, 0 }));
	avatars.push_back(std::move(newAvatarType01));
	std::unique_ptr<BossAvatarType02> newAvatarType02;
	newAvatarType02.reset(BossAvatarType02::Create(avatarModel, mainBody.get(), { -5, 0, 0 }));
	avatars.push_back(std::move(newAvatarType02));
	std::unique_ptr<BossAvatarType03> newAvatarType03;
	newAvatarType03.reset(BossAvatarType03::Create(avatarModel, mainBody.get(), { 0, 5, 0 }));
	avatars.push_back(std::move(newAvatarType03));
	std::unique_ptr<BossAvatarType04> newAvatarType04;
	newAvatarType04.reset(BossAvatarType04::Create(avatarModel, mainBody.get(), { 0, -5, 0 }));
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

	if (isDead) {
		DebugText::GetInstance()->Print("BossDead", 100, 60);
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
	//���g���S�Ď���ł��Ȃ�����_���[�W���󂯂Ȃ�
	if (!(avatars.size() == 0)) { return; }

	//�{�̂Ƀ_���[�W
	mainBody->Damage(damageNum);

	//�{�X�S�̂Ƀ_���[�W
	Damage(damageNum);
}

void Boss::OnCollisionAvatar(BossAvatar* avatar, const int damageNum)
{
	//�Փ˂������g�Ƀ_���[�W
	avatar->Damage(damageNum);

	//���g���S�ł������`�F�b�N
	CheckAllAvatarDead();

	//�{�X�S�̂Ƀ_���[�W
	Damage(damageNum);
}

bool Boss::Fall()
{
	//�~����ԂłȂ���Δ�����
	if (!(phase == Phase::Fall)) { return false; }

	//�^�C�}�[���X�V
	const float fallTime = 300;
	fallTimer++;
	const float time = fallTimer / fallTime;

	//�{�X�{�̂��~��������
	mainBody->Fall(time);

	//�^�C�}�[���w�肵�����ԂɂȂ����玟�̃t�F�[�Y��
	if (fallTimer >= fallTime) {
		phase = Phase::Attack;
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

bool Boss::AttackTypeASelect()
{
	//�{�̂��U����ԂłȂ���Δ�����
	if (!isMainBodyAttackMode) { return false; }

	//�O��̍U�����e��A�������甲����
	if (preAttackType == AttackType::A) { return false; }

	//�U�����eA���Z�b�g
	attackType = AttackType::A;
	//1�O�̍U�����e���X�V
	preAttackType = AttackType::A;

	return true;
}

bool Boss::AttackTypeBSelect()
{
	//�{�̂��U����ԂłȂ甲����
	if (isMainBodyAttackMode) { return false; }

	//�O��̍U�����e��B�������甲����
	if (preAttackType == AttackType::B) { return false; }

	//�v���C���[���@����ʉE���ɂ����甲����
	if (player->GetPosition().x <= 0) { return false; }

	//�U�����eB���Z�b�g
	attackType = AttackType::B;
	//1�O�̍U�����e���X�V
	preAttackType = AttackType::B;

	return true;
}

bool Boss::AttackTypeCSelect()
{
	//�{�̂��U����ԂłȂ甲����
	if (isMainBodyAttackMode) { return false; }

	//�O��̍U�����e��C�������甲����
	if (preAttackType == AttackType::C) { return false; }

	//�U�����eC���Z�b�g
	attackType = AttackType::C;
	//1�O�̍U�����e���X�V
	preAttackType = AttackType::C;

	return true;
}

bool Boss::AttackTypeDSelect()
{
	//�{�̂��U����ԂłȂ甲����
	if (isMainBodyAttackMode) { return false; }

	//�O��̍U�����e��D�������甲����
	if (preAttackType == AttackType::D) { return false; }

	//�U�����eC���Z�b�g
	attackType = AttackType::D;
	//1�O�̍U�����e���X�V
	preAttackType = AttackType::D;

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

bool Boss::AttackTypeA()
{
	//�U�����e��A�łȂ���Δ�����
	if (!(attackType == AttackType::A)) { return false; }

	mainBody->AttackTypeA(player->GetWorldPos());

	return true;
}

bool Boss::AttackTypeB()
{
	//�U�����e��B�łȂ���Δ�����
	if (!(attackType == AttackType::B)) { return false; }

	//�{�̂ƕ��g���U�����eB�œ�����
	mainBody->AttackTypeB();
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		avatar->AttackTypeB(player->GetWorldPos());
	}

	return true;
}

bool Boss::AttackTypeC()
{
	//�U�����e��C�łȂ���Δ�����
	if (!(attackType == AttackType::C)) { return false; }

	//�{�̂ƕ��g���U�����eC�œ�����
	mainBody->AttackTypeC(player->GetWorldPos());
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		avatar->AttackTypeC();
	}

	return true;
}

bool Boss::AttackTypeD()
{
	//�U�����e��D�łȂ���Δ�����
	if (!(attackType == AttackType::D)) { return false; }

	//�{�̂ƕ��g���U�����eD�œ�����
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		avatar->AttackTypeD();
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

bool Boss::ReturnFixedPosition()
{
	//�^�C�}�[���Œ�ʒu�ɖ߂邽�߂ɗv���鎞�Ԉȉ��łȂ��Ȃ甲����
	const float timer = waitModeTimer - changeModeTime;
	if (!(timer < returnFixedPositionTime)) { return false; }

	//�C�[�W���O�p��0�`1�̒l���Z�o����
	const float time = timer / returnFixedPositionTime;

	//�Œ�ʒu�ɖ߂�
	mainBody->ReturnFixedPosition(time);
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		avatar->ReturnFixedPosition(time);
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

void Boss::Damage(const int damageNum)
{
	//�����̐����̕��_���[�W����炤
	HP -= damageNum;

	//HP��0�ȉ��ɂȂ����玀�S
	if (HP <= 0) {
		HP = 0;

		isDead = true;
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
