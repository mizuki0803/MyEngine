#include "PlayerSpeedChangeBar.h"

PlayerSpeedChangeBar* PlayerSpeedChangeBar::Create(const Texture& texture, const Vector2& position, const float maxGaugeNum)
{
	//�v���C���[���x�ύXUI�o�[�̃C���X�^���X�𐶐�
	PlayerSpeedChangeBar* playerSpeedChangeBar = new PlayerSpeedChangeBar();
	if (playerSpeedChangeBar == nullptr) {
		return nullptr;
	}

	// ������
	if (!playerSpeedChangeBar->Initialize(texture, { 0.0f, 0.0f }, false, false)) {
		delete playerSpeedChangeBar;
		assert(0);
		return nullptr;
	}

	//���W���Z�b�g
	playerSpeedChangeBar->position = position;

	//�ő�Q�[�W�����Z�b�g
	playerSpeedChangeBar->maxGaugeNum = maxGaugeNum;

	return playerSpeedChangeBar;
}

void PlayerSpeedChangeBar::ChangeLength(const float gaugeNum)
{
	//�X�v���C�g�̃T�C�Y��ύX
	size.x = lengthMax * (gaugeNum / maxGaugeNum);
	//�e�N�X�`���T�C�Y���X�V�����T�C�Y�ɍ��킹��
	texSize = size;
}
