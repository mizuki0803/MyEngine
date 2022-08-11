#include "Boss.h"
#include "Easing.h"
#include "Input.h"

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

	return true;
}

void Boss::Update()
{
	//�s��
	Action();

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

void Boss::Action()
{
	switch (phase)
	{
	case Phase::Fall:
	{
		//�C�[�W���O�ō~������
		const float fallTime = 300;
		fallTimer++;
		const float time = fallTimer / fallTime;

		//�����ʒu����~�肽�Ƃ���Œ�~����
		Vector3 stayPos = startPos;
		stayPos.y = 0;
		bossBody->SetPosition(Easing::Lerp(startPos, stayPos, time)) ;

		//�C�[�W���O���I�������玟�̃t�F�[�Y��
		if (fallTimer >= fallTime) {
			phase = Phase::Stay;
		}
	}
	break;

	case Phase::Stay:
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

		break;
	}
}
