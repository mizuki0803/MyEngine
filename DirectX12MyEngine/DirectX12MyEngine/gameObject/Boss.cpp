#include "Boss.h"
#include "Easing.h"
#include "Input.h"
#include "DebugText.h"

void (Boss::* Boss::actionFuncTable[])() = {
	&Boss::Fall,
	&Boss::Stay
};

bool (Boss::* Boss::spFuncTable[])() = {
	&Boss::Otamesi,
	&Boss::Otamesi2
};

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
	//�̐���
	bossBody.reset(BossBody::Create(bodyModel, position));
	//������
	bossHead.reset(BossHead::Create(headModel, bossBody.get()));


	//HP�o�[����
	const Vector2 hpBarPosition = { 20, 120 };
	hpBar.reset(BossHPBar::Create(4, hpBarPosition, maxHP));
	//HP�o�[�t���[������
	const float posDiff = 3.0f;	//HP�o�[�̍��W�Ƃ̍���
	const Vector2 hpFramePosition = { hpBarPosition.x - posDiff, hpBarPosition.y - posDiff };
	hpFrame.reset(BossHPFrame::Create(5, hpFramePosition));

	//�r�w�C�r�A�c���[����
	//behaviorTree.reset(BossBehaviorTree::Create(this));

	return true;
}

void Boss::Update()
{
	//�s��
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//�r�w�C�r�A�c���[�ɂ��s���J��
	//behaviorTree->Root();


	for (int i = 0; i < 2; i++) {
		if ((this->*spFuncTable[i])()) { break; }
	}


	//�X�V
	bossBody->Update();//��
	bossHead->Update();//��

	//HP�o�[�X�V
	hpBar->Update();
	//HP�o�[�t���[���X�V
	hpFrame->Update();
}

void Boss::Draw()
{
	//�`��
	bossBody->Draw();//��
	bossHead->Draw();//��
}

void Boss::DrawUI()
{
	//HP�o�[�t���[���`��
	hpFrame->Draw();
	//HP�o�[�`��
	hpBar->Draw();
}

bool Boss::Otamesi()
{
	//�f�o�b�O�p�L�[����
	Input* input = Input::GetInstance();
	if (input->PushKey(DIK_1)) {
		DebugText::GetInstance()->Print("1push", 300, 300);
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
		return true;
	}

	DebugText::GetInstance()->Print("2noPush", 400, 300);
	return false;
}

void Boss::Fall()
{
	//�C�[�W���O�ō~������
	const float fallTime = 300;
	fallTimer++;
	const float time = fallTimer / fallTime;

	//�����ʒu����~�肽�Ƃ���Œ�~����
	Vector3 stayPos = startPos;
	stayPos.y = 0;
	bossBody->SetPosition(Easing::Lerp(startPos, stayPos, time));

	//�C�[�W���O���I�������玟�̃t�F�[�Y��
	if (fallTimer >= fallTime) {
		phase = Phase::Stay;
	}
}

void Boss::Stay()
{
	//�f�o�b�O�p�L�[����
	Input* input = Input::GetInstance();

	Vector3 posBody = bossBody->GetPosition();
	Vector3 posHead = bossHead->GetPosition();
	const float moveSpeed = 1.0f;
	if (input->PushKey(DIK_LEFT)) {
		//posBody.x -= moveSpeed;
		posHead.x -= moveSpeed;
	}
	if (input->PushKey(DIK_RIGHT)) {
		//posBody.x += moveSpeed;
		posHead.x += moveSpeed;
	}
	if (input->PushKey(DIK_UP)) {
		//posBody.y += moveSpeed;
		posHead.y += moveSpeed;
	}
	if (input->PushKey(DIK_DOWN)) {
		//posBody.y -= moveSpeed;
		posHead.y -= moveSpeed;
	}
	bossBody->SetPosition(posBody);
	bossHead->SetPosition(posHead);


	if (input->TriggerKey(DIK_Q)) {
		HP--;
		//HP��0�ȉ��ɂȂ�Ȃ�
		if (HP <= 0) {

			HP = 0;
		}

		hpBar->ChangeLength(HP);
	}
}
