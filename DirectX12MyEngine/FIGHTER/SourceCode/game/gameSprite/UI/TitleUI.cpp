#include "TitleUI.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"

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
	titleSprite.reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::TitleLogo)));
	const Vector2 titlePos = { 640, 100 };
	titleSprite->SetPosition(titlePos);
	const Vector2 titleTexSize = { 500, 100 };
	titleSprite->SetTexSize(titleTexSize);
	const Vector2 titleSize = titleTexSize * 1.2f;
	titleSprite->SetSize(titleSize);

	//B�{�^���X�v���C�g����	
	bButtonSprite.reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::BButton)));
	const Vector2 bButtonPos = { 640, 630 };
	bButtonSprite->SetPosition(bButtonPos);
	const Vector2 bButtonSize = { 64, 64 };
	bButtonSprite->SetSize(bButtonSize);
	bButtonSprite->SetTexSize(bButtonSize);

	return true;
}

void TitleUI::Update()
{
	//�\�����I���Ă��Ȃ��ꍇ
	if (!isShowEnd) {
		//�{�^���\���؂�ւ�
		DrawChangeButtonSprite();
	}
	//�\�����I���Ă���ꍇ
	else {
		ShowEnd();
	}

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

	//�\�����I�����甲����
	if (isShowEnd) { return; }

	//B�{�^���X�v���C�g�`��
	if (isDrawButtonSprite) {
		bButtonSprite->Draw();
	}
}

void TitleUI::SetShowEnd()
{
	//�\�����I����
	isShowEnd = true;

	//�^�C�}�[��������
	drawTimer = 0;
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

void TitleUI::ShowEnd()
{
	//�\�����I���鎞��
	const float endTime = 20;
	//�^�C�}�[���w�肵�����Ԉȏ�Ȃ甲����
	if (drawTimer >= endTime) { return; }

	//�^�C�}�[�X�V
	drawTimer++;
	const float time = drawTimer / endTime;

	//�C�[�W���O�ŏ�ɓ�����
	Vector2 pos = titleSprite->GetPosition();
	pos.y = Easing::OutQuad(100, -100, time);
	titleSprite->SetPosition(pos);
}
