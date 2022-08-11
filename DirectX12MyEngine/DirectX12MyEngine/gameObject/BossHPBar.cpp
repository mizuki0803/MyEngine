#include "BossHPBar.h"

BossHPBar* BossHPBar::Create(UINT texNumber, const Vector2 position, const int maxHP)
{
	//HP�o�[�̃C���X�^���X�𐶐�
	BossHPBar* bossHPBar = new BossHPBar();
	if (bossHPBar == nullptr) {
		return nullptr;
	}

	// ������
	if (!bossHPBar->Initialize(texNumber, { 0.0f, 0.0f }, false, false)) {
		delete bossHPBar;
		assert(0);
		return nullptr;
	}

	//���W���Z�b�g
	bossHPBar->position = position;

	//�ő�HP���Z�b�g
	bossHPBar->maxHP = maxHP;

	return bossHPBar;
}

void BossHPBar::ChangeLength(const int HP)
{
	//�X�v���C�g�̃T�C�Y��ύX
	size.y = lengthMax * ((float)HP / maxHP);
	//�e�N�X�`���T�C�Y���X�V�����T�C�Y�ɍ��킹��
	texSize = size;
}
