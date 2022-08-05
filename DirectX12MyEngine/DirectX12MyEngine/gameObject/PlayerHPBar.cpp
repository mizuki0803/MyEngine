#include "PlayerHPBar.h"
#include "Easing.h"

PlayerHPBar* PlayerHPBar::Create(UINT texNumber, const Vector2 position, const int maxHP)
{
	//HP�o�[�̃C���X�^���X�𐶐�
	PlayerHPBar* playerHPBar = new PlayerHPBar();
	if (playerHPBar == nullptr) {
		return nullptr;
	}

	// ������
	if (!playerHPBar->Initialize(texNumber, { 0.0f, 0.0f }, false, false)) {
		delete playerHPBar;
		assert(0);
		return nullptr;
	}

	//���W���Z�b�g
	playerHPBar->position = position;

	//�ő�HP���Z�b�g
	playerHPBar->maxHP = maxHP;

	return playerHPBar;
}

void PlayerHPBar::Update()
{
	//�����ύX��Ԃ̂Ƃ��̂ݒ����ύX
	if (isChangeLength)
	{
		ChangeLength();
	}

	//�X�v���C�g�̍X�V
	Sprite::Update();
}

void PlayerHPBar::SetChangeLength(const int HP)
{
	//�����ύX�^�C�}�[��������
	changeLengthTimer = 0;
	//�ύX�O�̒���(���݂̏��)���Z�b�g
	changeLengthBefore = size.x;
	//�ύX��̒������Z�b�g
	changeLengthAfter = lengthMax * ((float)HP / maxHP);
	//�����ύX��Ԃɂ���
	isChangeLength = true;
}

void PlayerHPBar::ChangeLength()
{
	//�����ύX���s������
	const int changeTime = 60;

	//�����ύX�^�C�}�[�X�V
	changeLengthTimer++;

	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)changeLengthTimer / changeTime;

	//�X�v���C�g�̃T�C�Y��ύX
	size.x = Easing::OutQuint(changeLengthBefore, changeLengthAfter, easeTimer);
	//�e�N�X�`���T�C�Y���X�V�����T�C�Y�ɍ��킹��
	texSize = size;

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (changeLengthTimer >= changeTime)
	{
		//�����ύX�I��
		isChangeLength = false;
	}
}
