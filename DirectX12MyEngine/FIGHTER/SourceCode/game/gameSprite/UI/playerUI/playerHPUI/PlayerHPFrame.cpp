#include "PlayerHPFrame.h"

const DirectX::XMFLOAT4 PlayerHPFrame::normalColor = { 0.57f, 0.56f, 0.5f, 1 };
const DirectX::XMFLOAT4 PlayerHPFrame::itemGetColor = { 0.95f, 0.95f, 0.1f, 1 };

PlayerHPFrame* PlayerHPFrame::Create(const Texture& texture, const Vector2& position)
{
	//HP�o�[�t���[���̃C���X�^���X�𐶐�
	PlayerHPFrame* playerHPFrame = new PlayerHPFrame();
	if (playerHPFrame == nullptr) {
		return nullptr;
	}

	// ������
	if (!playerHPFrame->Initialize(texture, { 0.0f, 0.0f }, false, false)) {
		delete playerHPFrame;
		assert(0);
		return nullptr;
	}

	//���W���Z�b�g
	playerHPFrame->position = position;
	//�F���Z�b�g
	playerHPFrame->color = normalColor;

	return playerHPFrame;
}

void PlayerHPFrame::Update()
{
	//�A�C�e���l�����
	if (isItemGetMode) {
		ItemGetMode();
	}

	//�X�v���C�g�̍X�V
	Sprite::Update();
}

void PlayerHPFrame::ItemGetModeStart()
{
	//�^�C�}�[��������
	itemGetModeTimer = 0;
	//�A�C�e���l����Ԃɂ���
	isItemGetMode = true;
}

void PlayerHPFrame::ItemGetMode()
{
	//�A�C�e���l����Ԃ̎���
	const int itemGetModeTime = 80;
	//�^�C�}�[�X�V
	itemGetModeTimer++;

	//���ԂŐF�����F�ƒʏ�F���݂ɕύX
	if (itemGetModeTimer % 5 == 0) {
		//�ʏ�F�Ȃ�A�C�e���l���F�֕ύX
		if (isNormalColor) {
			color = itemGetColor;
			isNormalColor = false;
		}
		//�A�C�e���l���F�Ȃ�ʏ�F�֕ύX
		else {
			color = normalColor;
			isNormalColor = true;
		}
	}


	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (itemGetModeTimer >= itemGetModeTime) {
		//�A�C�e���l����ԏI��
		isItemGetMode = false;

		//�F�����ɖ߂��Ă���
		isNormalColor = true;
		color = normalColor;
	}
}
