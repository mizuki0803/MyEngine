#include "TitleUI.h"
#include "SpriteTextureLoader.h"

TitleUI* TitleUI::Create()
{
	//�^�C�g��UI�̃C���X�^���X�𐶐�
	TitleUI* titleUI = new TitleUI();
	if (titleUI == nullptr) {
		return nullptr;
	}

	// ������
	if (!titleUI->Initialize()) {
		delete titleUI;
		assert(0);
		return nullptr;
	}

	return titleUI;
}

bool TitleUI::Initialize()
{
	//�^�C�g�����S�X�v���C�g����
	titleSprite.reset(Sprite::Create(SpriteTextureLoader::TitleLogo));
	const Vector2 titlePos = { 640, 100 };
	titleSprite->SetPosition(titlePos);
	const Vector2 titleSize = { 300, 100 };
	titleSprite->SetSize(titleSize);
	titleSprite->SetTexSize(titleSize);

	//B�{�^���X�v���C�g����	
	bButtonSprite.reset(Sprite::Create(SpriteTextureLoader::BButton));
	const Vector2 bButtonPos = { 640, 630 };
	bButtonSprite->SetPosition(bButtonPos);
	const Vector2 bButtonSize = { 64, 64 };
	bButtonSprite->SetSize(bButtonSize);
	bButtonSprite->SetTexSize(bButtonSize);

	return true;
}

void TitleUI::Update()
{
	//�{�^���\���؂�ւ�
	DrawChangeButtonSprite();

	//�^�C�g�����S�X�v���C�g�X�V
	titleSprite->Update();
	//B�{�^���X�v���C�g�X�V
	if (isDrawButtonSprite) {
		bButtonSprite->Update();
	}
}

void TitleUI::Draw()
{
	//�^�C�g�����S�X�v���C�g�`��
	titleSprite->Draw();
	//B�{�^���X�v���C�g�`��
	if (isDrawButtonSprite) {
		bButtonSprite->Draw();
	}
}

void TitleUI::DrawChangeButtonSprite()
{
	//�؂�ւ��鎞��
	const int drawChangeTime = 50;
	//�^�C�}�[�X�V
	drawTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (drawTimer >= drawChangeTime) {
		//�\���؂�ւ�
		if (isDrawButtonSprite) { isDrawButtonSprite = false; }
		else { isDrawButtonSprite = true; }

		//�^�C�}�[������
		drawTimer = 0;
	}
}
