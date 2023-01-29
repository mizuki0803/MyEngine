#include "BossHPLogo.h"
#include "Easing.h"

BossHPLogo* BossHPLogo::Create(const Texture& texture, const Vector2& position)
{
	//HPUI���S�̃C���X�^���X�𐶐�
	BossHPLogo* bossHPLogo = new BossHPLogo();
	if (bossHPLogo == nullptr) {
		return nullptr;
	}

	// ������
	if (!bossHPLogo->Initialize(texture, { 0.0f, 0.0f }, false, false)) {
		delete bossHPLogo;
		assert(0);
		return nullptr;
	}

	//�\��������W���Z�b�g
	bossHPLogo->stayPosition = position;
	//��ʊO�ɃZ�b�g���Ă���
	bossHPLogo->position = position;

	return bossHPLogo;
}

void BossHPLogo::StartSetMove(int moveTime, int timer)
{
	//�C�[�W���O�v�Z�p�̎���
	float easeTimer = (float)timer / moveTime;

	//�K���ɉ�ʊO���W���Z�b�g
	const float screenOutPos = -50;
	//��ʊO����\��������W�Ɉړ�������
	position.x = Easing::OutQuint(screenOutPos, stayPosition.x, easeTimer);
}
