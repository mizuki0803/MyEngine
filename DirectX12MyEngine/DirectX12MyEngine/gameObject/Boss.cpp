#include "Boss.h"
#include "Easing.h"
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

	boss->startPos = position;

	return boss;
}

bool Boss::Initialize(ObjModel* bodyModel, ObjModel* headModel, const Vector3& position)
{
	//�{�̐���
	bossMainBody.reset(BossMainBody::Create(bodyModel, position));
	//�{�X���g����
	std::unique_ptr<BossAvatarType01> newbossAvatarType01;
	newbossAvatarType01.reset(BossAvatarType01::Create(headModel, bossMainBody.get(), {5, 0, 0}));
	bossAvatars.push_back(std::move(newbossAvatarType01));
	std::unique_ptr<BossAvatarType02> newbossAvatarType02;
	newbossAvatarType02.reset(BossAvatarType02::Create(headModel, bossMainBody.get(), { -5, 0, 0 }));
	bossAvatars.push_back(std::move(newbossAvatarType02));
	std::unique_ptr<BossAvatarType03> newbossAvatarType03;
	newbossAvatarType03.reset(BossAvatarType03::Create(headModel, bossMainBody.get(), { 0, 5, 0 }));
	bossAvatars.push_back(std::move(newbossAvatarType03));
	std::unique_ptr<BossAvatarType04> newbossAvatarType04;
	newbossAvatarType04.reset(BossAvatarType04::Create(headModel, bossMainBody.get(), { 0, -5, 0 }));
	bossAvatars.push_back(std::move(newbossAvatarType04));


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
	bossAvatars.remove_if([](std::unique_ptr<BossAvatar>& bossAvatar) {
		return bossAvatar->GetIsDead();
		});

	//�r�w�C�r�A�c���[�ɂ��s���J��
	behaviorTree->Root();

	//�X�V
	bossMainBody->Update();//�{��
	for (const std::unique_ptr<BossAvatar>& bossAvatar : bossAvatars) {
		bossAvatar->Update();//���g
	}

	//HP�o�[�X�V
	hpBar->Update();
	//HP�o�[�t���[���X�V
	hpFrame->Update();

	std::string avatarNum = std::to_string(bossAvatars.size());
	DebugText::GetInstance()->Print("AvatarNum : " + avatarNum, 100, 100);
}

void Boss::Draw()
{
	//�`��
	bossMainBody->Draw();//�{��
	for (const std::unique_ptr<BossAvatar>& bossAvatar : bossAvatars) {
		bossAvatar->Draw();//���g
	}
}

void Boss::DrawUI()
{
	//HP�o�[�t���[���`��
	hpFrame->Draw();
	//HP�o�[�`��
	hpBar->Draw();
}

bool Boss::Fall()
{
	//�~����ԂłȂ���Δ�����
	if (!(phase == Phase::Fall)) { return false; }

	//�C�[�W���O�ō~������
	const float fallTime = 300;
	fallTimer++;
	const float time = fallTimer / fallTime;

	//�����ʒu����~�肽�Ƃ���Œ�~����
	Vector3 stayPos = startPos;
	stayPos.y = 0;
	bossMainBody->SetPosition(Easing::Lerp(startPos, stayPos, time));

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

		for (const std::unique_ptr<BossAvatar>& bossAvatar : bossAvatars) {
			bossAvatar->Otamesi();//���g
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

		for (const std::unique_ptr<BossAvatar>& bossAvatar : bossAvatars) {
			bossAvatar->Otamesi2();//���g
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

		for (const std::unique_ptr<BossAvatar>& bossAvatar : bossAvatars) {
			bossAvatar->Otamesi3();//���g
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

		for (const std::unique_ptr<BossAvatar>& bossAvatar : bossAvatars) {
			bossAvatar->Otamesi4();//���g
		}

		return true;
	}

	DebugText::GetInstance()->Print("4noPush", 700, 300);
	return false;
}
