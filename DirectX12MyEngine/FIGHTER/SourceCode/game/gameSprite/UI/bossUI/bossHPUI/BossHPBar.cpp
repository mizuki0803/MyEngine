#include "BossHPBar.h"
#include "Easing.h"

BossHPBar* BossHPBar::Create(const Texture& texture, const Vector2& position, const int maxHP)
{
	//HP�o�[�̃C���X�^���X�𐶐�
	BossHPBar* bossHPBar = new BossHPBar();
	if (bossHPBar == nullptr) {
		return nullptr;
	}

	// ������
	if (!bossHPBar->Initialize(texture, { 0.0f, 0.0f }, false, false)) {
		delete bossHPBar;
		assert(0);
		return nullptr;
	}

	//�\��������W���Z�b�g
	bossHPBar->stayPosition = position;
	//��ʊO�ɃZ�b�g���Ă���
	bossHPBar->position = position;

	//�ő�HP���Z�b�g
	bossHPBar->maxHP = maxHP;

	//�傫����0�ɂ��Ă���
	bossHPBar->size.y = 0;
	bossHPBar->texSize.y = 0;

	return bossHPBar;
}

void BossHPBar::Update()
{
	//�����ύX��Ԃ̂Ƃ��̂ݒ����ύX
	if (isChangeLength) {
		ChangeLength();
	}

	//�X�v���C�g�̍X�V
	Sprite::Update();
}

void BossHPBar::StartSetMove(int moveTime, int timer)
{
	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)timer / moveTime;

	//�K���ɉ�ʊO���W���Z�b�g
	const float screenOutPos = -50;
	//��ʊO����\��������W�Ɉړ�������
	position.x = Easing::OutQuint(screenOutPos, stayPosition.x, easeTimer);
}

void BossHPBar::StartSetChangeSize(int changeTime, int timer)
{
	//�^�C�}�[�̒l���ύX�ɂ����鎞�Ԃ��z���Ă����甲����
	if (timer > changeTime) { return; }

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)timer / changeTime;

	//�Q�[�W�����ɐL�΂��Ă���
	size.y = Easing::OutQuart(0, lengthMax, easeTimer);
	texSize = size;
}

void BossHPBar::SetChangeLength(const int HP)
{
	//�����ύX�^�C�}�[��������
	changeLengthTimer = 0;
	//�ύX�O�̒���(���݂̏��)���Z�b�g
	changeLengthBefore = size.y;
	//�ύX��̒������Z�b�g
	changeLengthAfter = lengthMax * ((float)HP / maxHP);
	//�����ύX��Ԃɂ���
	isChangeLength = true;
}

void BossHPBar::ChangeLength()
{
	//�����ύX���s������
	const int changeTime = 20;

	//�����ύX�^�C�}�[�X�V
	changeLengthTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)changeLengthTimer / changeTime;

	//�X�v���C�g�̃T�C�Y��ύX
	size.y = Easing::OutQuint(changeLengthBefore, changeLengthAfter, easeTimer);
	//�e�N�X�`���T�C�Y���X�V�����T�C�Y�ɍ��킹��
	texSize = size;

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (changeLengthTimer >= changeTime) {
		//�����ύX�I��
		isChangeLength = false;
	}
}
