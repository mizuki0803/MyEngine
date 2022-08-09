#include "Boss.h"
#include "Easing.h"

Boss* Boss::Create(ObjModel* model, const Vector3& position)
{
	//�{�X�̃C���X�^���X�𐶐�
	Boss* boss = new Boss();
	if (boss == nullptr) {
		return nullptr;
	}

	//���f�����Z�b�g
	assert(model);
	boss->model = model;

	// ������
	if (!boss->Initialize()) {
		delete boss;
		assert(0);
		return nullptr;
	}

	//�������W���Z�b�g
	boss->startPos = position;

	//���W���Z�b�g
	boss->position = position;

	boss->scale = { 3.0f, 3.0f, 3.0f };

	return boss;
}

void Boss::Update()
{
	//�s��
	Action();

	//3D�I�u�W�F�N�g�̍X�V
	ObjObject3d::Update();
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
		position = Easing::Lerp(startPos, stayPos, time);

		//�C�[�W���O���I�������玟�̃t�F�[�Y��
		if (fallTimer >= fallTime) {
			phase = Phase::Stay;
		}
	}
	break;

	case Phase::Stay:
		//�Ƃ肠�����������Ȃ�

		break;
	}
}
