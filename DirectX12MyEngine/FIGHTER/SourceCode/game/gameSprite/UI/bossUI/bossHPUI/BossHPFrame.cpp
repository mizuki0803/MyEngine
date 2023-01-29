#include "BossHPFrame.h"
#include "Easing.h"

BossHPFrame* BossHPFrame::Create(const Texture& texture, const Vector2& position)
{
	//HP�o�[�t���[���̃C���X�^���X�𐶐�
	BossHPFrame* bossHPFrame = new BossHPFrame();
	if (bossHPFrame == nullptr) {
		return nullptr;
	}

	// ������
	if (!bossHPFrame->Initialize(texture, { 0.0f, 0.0f }, false, false)) {
		delete bossHPFrame;
		assert(0);
		return nullptr;
	}

	//�g�̑傫�����Z�b�g
	bossHPFrame->frameSize = bossHPFrame->texSize;

	//�\��������W���Z�b�g
	bossHPFrame->stayPosition = position;
	//��ʊO�ɃZ�b�g���Ă���
	bossHPFrame->position = position;

	//�傫����0�ɂ��Ă���
	bossHPFrame->size.y = 0;
	bossHPFrame->texSize.y = 0;

	return bossHPFrame;
}

void BossHPFrame::StartSetMove(int moveTime, int timer)
{
	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)timer / moveTime;

	//�K���ɉ�ʊO���W���Z�b�g
	const float screenOutPos = -50;
	//��ʊO����\��������W�Ɉړ�������
	position.x = Easing::OutQuint(screenOutPos, stayPosition.x, easeTimer);
}

void BossHPFrame::StartSetChangeSize(int changeTime, int timer)
{
	//�^�C�}�[�̒l���ύX�ɂ����鎞�Ԃ��z���Ă����甲����
	if (timer > changeTime) { return; }

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)timer / changeTime;

	//�Q�[�W�����ɐL�΂��Ă���
	size.y = Easing::OutQuart(0, frameSize.y, easeTimer);
	texSize = size;
}
