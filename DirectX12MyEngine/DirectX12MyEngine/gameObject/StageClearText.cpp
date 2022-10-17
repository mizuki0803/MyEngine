#include "StageClearText.h"
#include "SpriteTexture.h"

StageClearText* StageClearText::Create()
{
	//�X�e�[�W�N���A���ɕ\������e�L�X�g�̃C���X�^���X�𐶐�
	StageClearText* stageClearText = new StageClearText();
	if (stageClearText == nullptr) {
		return nullptr;
	}

	// ������
	if (!stageClearText->Initialize()) {
		delete stageClearText;
		assert(0);
		return nullptr;
	}

	return stageClearText;
}

bool StageClearText::Initialize()
{
	//�X�e�[�W���X�v���C�g����
	stageNameSprite.reset(Sprite::Create(SpriteTexture::PlayerHPGaugeOut));
	stageNameSprite->SetPosition({640, 200});

	//�������̕��A�����X�v���C�g����
	for (int i = 0; i < textNum; i++) {
		textSprites[i].reset(Sprite::Create(SpriteTexture::Number));
		const Vector2 size = { 100, 100 };
		const Vector2 texSize = { 32, 48 };
		const Vector2 leftTop = { i * texSize.x, 0 };
		const float distance = 120;
		const Vector2 centerPos = { 640, 350 };
		const Vector2 pos = { centerPos.x + ((i % 2) * distance) - (distance / 2), centerPos.y + ((i / 2) * distance) - (distance / 2) };
		textSprites[i]->SetPosition(pos);
		textSprites[i]->SetSize(size);
		textSprites[i]->SetTexSize(texSize);
		textSprites[i]->SetTexLeftTop(leftTop);
	}

	return true;
}

void StageClearText::Update()
{
	//�\�����镶�����𑝂₷
	UpdateDisplayNum();

	//�X�e�[�W���X�v���C�g�X�V
	stageNameSprite->Update();
	//�\�����镶���̂ݕ����X�v���C�g�X�V
	for (int i = 0; i < displayTextNum; i++) {
		textSprites[i]->Update();
	}
}

void StageClearText::Draw()
{
	//�X�e�[�W���X�v���C�g�`��
	stageNameSprite->Draw();
	//�\�����镶���̂ݕ����X�v���C�g�`��
	for (int i = 0; i < displayTextNum; i++) {
		textSprites[i]->Draw();
	}
}

void StageClearText::UpdateDisplayNum()
{
	//���ɑS������\�����Ă����甲����
	if (displayTextNum >= textNum) { return; }

	//�\�����J�n���鎞��
	const int displayTime = 60;
	//�^�C�}�[�X�V
	displayTimer++;

	//�^�C�}�[���\���J�n���Ԃ�菬�����Ȃ甲����
	if (displayTimer < displayTime) { return; }


	//�\�����镶�����𑝂₷����
	const int increaseTime = 20;

	//�^�C�}�[���������𑝂₷���Ԃ̔{���łȂ��Ȃ甲����
	if (!((displayTimer % increaseTime) == 0)) { return; }

	//�\�����镶�����𑝂₷
	displayTextNum++;
}
