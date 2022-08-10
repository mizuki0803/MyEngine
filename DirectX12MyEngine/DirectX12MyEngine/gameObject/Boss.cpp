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

	return true;
}

void Boss::Update()
{
	//�s��
	Action();

	//�X�V
	bossBody->Update();//��
	bossHead->Update();//��
}

void Boss::Draw()
{
	//�`��
	bossBody->Draw();//��
	bossHead->Draw();//��
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

		break;
	}
}
