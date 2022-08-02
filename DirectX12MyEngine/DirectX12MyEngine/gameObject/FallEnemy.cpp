#include "FallEnemy.h"
#include "Easing.h"

FallEnemy* FallEnemy::Create(ObjModel* model, const Vector3& position)
{
	//�G�̃C���X�^���X�𐶐�
	FallEnemy* upDownEnemy = new FallEnemy();
	if (upDownEnemy == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	upDownEnemy->model = model;

	// ������
	if (!upDownEnemy->Initialize()) {
		delete upDownEnemy;
		assert(0);
		return nullptr;
	}

	//�������W���Z�b�g
	upDownEnemy->startPos = position;

	//���W���Z�b�g
	upDownEnemy->position = position;

	return upDownEnemy;
}

void FallEnemy::Update()
{
	//�s��
	Action();

	//��ʊO��O�܂ōs������폜
	FrontOfScreenDelete();

	//3D�I�u�W�F�N�g�̍X�V
	ObjObject3d::Update();
}

void FallEnemy::Action()
{
	switch (phase)
	{
	case Phase::Fall:
	{
		//�C�[�W���O�ō~������
		const float fallTime = 60;
		fallTimer++;
		const float time = fallTimer / fallTime;

		//�����ʒu����100�~�肽�Ƃ���Œ�~����
		const float stayPosY = startPos.y - 100;
		position.y = Easing::OutQuart(startPos.y, stayPosY, time);

		//�C�[�W���O���I�������玟�̃t�F�[�Y��
		if (fallTimer >= fallTime) {
			phase = Phase::Shot;
		}
	}
	break;

	case Phase::Shot:
		//���˃^�C�}�[�J�E���g�_�E��
		--fireTimer;
		if (fireTimer <= 0) {
			//�e�𔭎�
			Fire();
			//���˃^�C�}�[��������
			fireTimer = fireInterval;
		}

		break;
	}
}
