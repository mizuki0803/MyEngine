#include "BossHPUI.h"
#include "SpriteTextureLoader.h"

void (BossHPUI::* BossHPUI::startSetActionFuncTable[])() = {
	&BossHPUI::StartSetMove,
	&BossHPUI::StartSetChangeSize,
};

BossHPUI* BossHPUI::Create(const Vector2& position, const int maxHP)
{
	//�{�X�pHPUI�̃C���X�^���X�𐶐�
	BossHPUI* bossHPUI = new BossHPUI();
	if (bossHPUI == nullptr) {
		return nullptr;
	}

	// ������
	if (!bossHPUI->Initialize(position, maxHP)) {
		delete bossHPUI;
		assert(0);
		return nullptr;
	}

	return bossHPUI;
}

bool BossHPUI::Initialize(const Vector2& position, const int maxHP)
{
	//HPUI���S����
	const Vector2 hpLogoPosition = position;
	hpLogo.reset(BossHPLogo::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::BossHPLogo), hpLogoPosition));
	//HP�o�[�t���[������
	const Vector2 posDiffLogo = { 13.0f, 38.0f };
	const Vector2 hpFramePosition = { hpLogoPosition.x + posDiffLogo.x, hpLogoPosition.y + posDiffLogo.y };
	hpFrame.reset(BossHPFrame::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::BossHPGaugeOut), hpFramePosition));
	//HPUI�o�[����
	const float posDiffFrame = 3.0f;	//HP�g�̍��W�Ƃ̍���
	const Vector2 hpBarPosition = { hpFramePosition.x + posDiffFrame, hpFramePosition.y + posDiffFrame };
	hpBar.reset(BossHPBar::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::BossHPGaugeIn), hpBarPosition, maxHP));

	return true;
}

void BossHPUI::Update()
{
	//�J�n���Z�b�g
	if (isStartSet) {
		StartSet();
	}

	//���S�X�V
	hpLogo->Update();
	//�o�[�X�V
	hpBar->Update();
	//�t���[���X�V
	hpFrame->Update();
}

void BossHPUI::Draw()
{
	//�t���[���`��
	hpFrame->Draw();
	//�o�[�`��
	hpBar->Draw();
	//���S�X�V
	hpLogo->Draw();
}

void BossHPUI::Damage(const int HP)
{
	//�o�[�̒�����ύX����
	hpBar->SetChangeLength(HP);
}

void BossHPUI::StartSet()
{
	//�J�n���Z�b�g�s��
	(this->*startSetActionFuncTable[static_cast<size_t>(startSetPhase)])();
}

void BossHPUI::StartSetMove()
{
	//��ʓ��ړ��ɂ����鎞��
	const int moveTime = 40;
	//�^�C�}�[���X�V
	startSetTimer++;

	//�ړ�������
	hpLogo->StartSetMove(moveTime, startSetTimer);
	hpBar->StartSetMove(moveTime, startSetTimer);
	hpFrame->StartSetMove(moveTime, startSetTimer);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (startSetTimer >= moveTime) {
		//���̃t�F�[�Y��
		startSetPhase = StartSetPhase::ChangeSize;

		//�^�C�}�[��������
		startSetTimer = 0;
	}
}

void BossHPUI::StartSetChangeSize()
{
	//�傫���ύX�ɂ����鎞��
	const int changeTime = 60;
	//�^�C�}�[���X�V
	startSetTimer++;

	//�ړ�������
	hpBar->StartSetChangeSize(changeTime, startSetTimer);

	//�t���[���̑傫���ύX����
	const int frameChangeTime = 20;
	hpFrame->StartSetChangeSize(frameChangeTime, startSetTimer);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (startSetTimer >= changeTime) {
		//�J�n���Z�b�g�I��
		isStartSet = false;
	}
}
