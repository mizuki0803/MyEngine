#include "Boss.h"
#include "Input.h"
#include "DebugText.h"
#include "BossAvatarType01.h"
#include "BossAvatarType02.h"
#include "BossAvatarType03.h"
#include "BossAvatarType04.h"

Boss* Boss::Create(ObjModel* bodyModel, ObjModel* headModel, const Vector3& position)
{
	//�{�X�̃C���X�^���X�𐶐�
	Boss* boss = new Boss();
	if (boss == nullptr) {
		return nullptr;
	}

	// ������
	if (!boss->Initialize(bodyModel, headModel, position)) {
		delete boss;
		assert(0);
		return nullptr;
	}

	return boss;
}

bool Boss::Initialize(ObjModel* bodyModel, ObjModel* headModel, const Vector3& position)
{
	//�{�̐���
	mainBody.reset(BossMainBody::Create(bodyModel, position));
	//�{�X���g����
	std::unique_ptr<BossAvatarType01> newAvatarType01;
	newAvatarType01.reset(BossAvatarType01::Create(headModel, mainBody.get(), { 5, 0, 0 }));
	avatars.push_back(std::move(newAvatarType01));
	std::unique_ptr<BossAvatarType02> newAvatarType02;
	newAvatarType02.reset(BossAvatarType02::Create(headModel, mainBody.get(), { -5, 0, 0 }));
	avatars.push_back(std::move(newAvatarType02));
	std::unique_ptr<BossAvatarType03> newAvatarType03;
	newAvatarType03.reset(BossAvatarType03::Create(headModel, mainBody.get(), { 0, 5, 0 }));
	avatars.push_back(std::move(newAvatarType03));
	std::unique_ptr<BossAvatarType04> newAvatarType04;
	newAvatarType04.reset(BossAvatarType04::Create(headModel, mainBody.get(), { 0, -5, 0 }));
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

void Boss::OnCollisionAvator(BossAvatar* avatar, const int damageNum)
{
	//�Փ˂������g�Ƀ_���[�W
	avatar->Damage(damageNum);

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

	//�C�[�W���O���I�������玟�̃t�F�[�Y��
	if (fallTimer >= fallTime) {
		phase = Phase::Stay;
	}

	return true;
}

bool Boss::Otamesi()
{
	//�f�o�b�O�p�L�[����
	Input* input = Input::GetInstance();
	if (input->PushKey(DIK_1)) {
		DebugText::GetInstance()->Print("1push", 300, 300);

		for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
			avatar->Otamesi();//���g
		}

		return true;
	}

	DebugText::GetInstance()->Print("1noPush", 300, 300);
	return false;
}

bool Boss::Otamesi2()
{
	//�f�o�b�O�p�L�[����
	Input* input = Input::GetInstance();
	if (input->PushKey(DIK_2)) {
		DebugText::GetInstance()->Print("2push", 400, 300);

		for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
			avatar->Otamesi2();//���g
		}

		return true;
	}

	DebugText::GetInstance()->Print("2noPush", 400, 300);
	return false;
}

bool Boss::Otamesi3()
{
	//�f�o�b�O�p�L�[����
	Input* input = Input::GetInstance();
	if (input->PushKey(DIK_3)) {
		DebugText::GetInstance()->Print("3push", 600, 300);

		for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
			avatar->Otamesi3();//���g
		}

		return true;
	}

	DebugText::GetInstance()->Print("3noPush", 600, 300);
	return false;
}

bool Boss::Otamesi4()
{
	//�f�o�b�O�p�L�[����
	Input* input = Input::GetInstance();
	if (input->PushKey(DIK_4)) {
		DebugText::GetInstance()->Print("4push", 700, 300);

		for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
			avatar->Otamesi4();//���g
		}

		return true;
	}

	DebugText::GetInstance()->Print("4noPush", 700, 300);
	return false;
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
