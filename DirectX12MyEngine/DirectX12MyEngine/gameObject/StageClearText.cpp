#include "StageClearText.h"
#include "SpriteTextureLoader.h"

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
	stageNameSprite.reset(Sprite::Create(SpriteTextureLoader::PlayerHPGaugeOut));
	stageNameSprite->SetPosition({ 640, 200 });

	//�������̕��A�����X�v���C�g����
	for (int i = 0; i < textNum; i++) {
		textSprites[i].reset(Sprite::Create(SpriteTextureLoader::StageClearText));
		const Vector2 size = { 200, 200 };
		const Vector2 texSize = { 155, 155 };
		const Vector2 leftTop = { i * texSize.x, 0 };
		const float distance = size.x;
		const Vector2 centerPos = { 640, 400 };
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
	//�S������\�����Ă��Ȃ����
	if (displayTextNum < textNum) {
		//�\�����镶�����𑝂₷
		UpdateDisplayNum();
	}
	//�\�����Ă�����
	else {
		//�e�L�X�g����ʊO�Ɉړ�������
		TextMove();
	}


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

void StageClearText::TextMove()
{
	//�ړ����J�n���鎞��
	const int moveStartTime = 120;
	//�^�C�}�[�X�V
	moveTimer++;

	//�^�C�}�[���J�n���Ԃ�菬�����Ȃ甲����
	if (moveTimer < moveStartTime) { return; }

	//��������ʊO�Ɉړ�������
	const float moveSpeed = 80.0f;

	for (int i = 0; i < textNum; i++) {
		Vector2 velocity = { moveSpeed, 0 };
		//�����̕����͍��A�E���̕����͉E�Ɉړ�
		if ((i % 2) == 0) { velocity.x = -velocity.x; }
		Vector2 pos = textSprites[i]->GetPosition();
		pos += velocity;
		textSprites[i]->SetPosition(pos);
	}
}
