#include "PlayerHPUI.h"
#include "SpriteTextureLoader.h"

PlayerHPUI* PlayerHPUI::Create(const Vector2& position, const int startHP, const int maxHP)
{
	//HPUI�̃C���X�^���X�𐶐�
	PlayerHPUI* playerHPUI = new PlayerHPUI();
	if (playerHPUI == nullptr) {
		return nullptr;
	}

	// ������
	if (!playerHPUI->Initialize(position, startHP, maxHP)) {
		delete playerHPUI;
		assert(0);
		return nullptr;
	}

	return playerHPUI;
}

bool PlayerHPUI::Initialize(const Vector2& position, const int startHP, const int maxHP)
{
	//HP�o�[����
	const Vector2 hpBarPosition = position;
	hpBar.reset(PlayerHPBar::Create(SpriteTextureLoader::PlayerHPGaugeIn, hpBarPosition, startHP, maxHP));
	//HP�o�[�t���[������
	const float posDiff = 3.0f;	//HP�o�[�̍��W�Ƃ̍���
	const Vector2 hpFramePosition = { hpBarPosition.x - posDiff, hpBarPosition.y - posDiff };
	hpFrame.reset(PlayerHPFrame::Create(SpriteTextureLoader::PlayerHPGaugeOut, hpFramePosition));

    return true;
}

void PlayerHPUI::Update()
{
	//HP�o�[�X�V
	hpBar->Update();
	//HP�t���[���X�V
	hpFrame->Update();
}

void PlayerHPUI::Draw()
{
	//HP�t���[���`��
	hpFrame->Draw();
	//HP�o�[�`��
	hpBar->Draw();
}

void PlayerHPUI::Damage(const int HP)
{
	//HP�o�[�̒�����ύX����
	hpBar->SetChangeLength(HP);
}

void PlayerHPUI::ItemGet(const int HP)
{
	//HP�o�[�̒�����ύX����
	hpBar->SetChangeLength(HP);
	//HP�o�[�t���[����F���`�J�`�J������
	hpFrame->ItemGetModeStart();
}
