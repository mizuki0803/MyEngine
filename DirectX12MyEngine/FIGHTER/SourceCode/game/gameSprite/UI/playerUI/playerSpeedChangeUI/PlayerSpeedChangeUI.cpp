#include "PlayerSpeedChangeUI.h"
#include "SpriteTextureLoader.h"

PlayerSpeedChangeUI* PlayerSpeedChangeUI::Create(const Vector2& position, const float maxGaugeNum)
{
	//�v���C���[���x�ύXUI�̃C���X�^���X�𐶐�
	PlayerSpeedChangeUI* playerSpeedChangeUI = new PlayerSpeedChangeUI();
	if (playerSpeedChangeUI == nullptr) {
		return nullptr;
	}

	// ������
	if (!playerSpeedChangeUI->Initialize(position, maxGaugeNum)) {
		delete playerSpeedChangeUI;
		assert(0);
		return nullptr;
	}

	return playerSpeedChangeUI;
}

bool PlayerSpeedChangeUI::Initialize(const Vector2& position, const float maxGaugeNum)
{
	//���x�ύXUI�o�[����
	const Vector2 barPosition = position;
	speedChangeBar.reset(PlayerSpeedChangeBar::Create(SpriteTextureLoader::PlayerHPGaugeIn, barPosition, maxGaugeNum));
	speedChangeBar->SetColor({ 1, 1, 0, 1 });
	//���x�ύXUI�o�[�t���[������
	const float posDiff = 3.0f;	//HP�o�[�̍��W�Ƃ̍���
	const Vector2 hpFramePosition = { barPosition.x - posDiff, barPosition.y - posDiff };
	speedChangeFrame.reset(Sprite::Create(SpriteTextureLoader::PlayerHPGaugeOut, { 0, 0 }));
	speedChangeFrame->SetPosition(hpFramePosition);

	return true;
}

void PlayerSpeedChangeUI::Update()
{
	//�o�[�X�V
	speedChangeBar->Update();
	//�t���[���X�V
	speedChangeFrame->Update();
}

void PlayerSpeedChangeUI::Draw()
{
	//�t���[���`��
	speedChangeFrame->Draw();
	//�o�[�`��
	speedChangeBar->Draw();
}

void PlayerSpeedChangeUI::ChangeLength(const float gaugeNum)
{
	//�o�[�̒�����ύX����
	speedChangeBar->ChangeLength(gaugeNum);
}
