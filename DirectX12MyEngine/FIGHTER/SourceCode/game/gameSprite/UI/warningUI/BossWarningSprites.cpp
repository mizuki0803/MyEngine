#include "BossWarningSprites.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"
#include "WindowApp.h"

BossWarningSprites* BossWarningSprites::Create(const Vector2& leftTopPos)
{
	//�{�X�o��x�����o�̃C���X�^���X�𐶐�
	BossWarningSprites* bossWarningEffect = new BossWarningSprites();
	if (bossWarningEffect == nullptr) {
		return nullptr;
	}

	// ������
	if (!bossWarningEffect->Initialize(leftTopPos)) {
		delete bossWarningEffect;
		assert(0);
		return nullptr;
	}

	return bossWarningEffect;
}

bool BossWarningSprites::Initialize(const Vector2& leftTopPos)
{
	//���C���̖{������
	for (int i = 0; i < 2; i++) {
		//�x�����C���p�w�i�X�v���C�g����
		//�傫��
		const Vector2 backSize = { WindowApp::window_width, 30 };
		//�\�����W
		Vector2 showPos = { WindowApp::window_width / 2, leftTopPos.y + backSize.y / 2 };
		if (i == 1) {
			//��̃��C���Ɖ��̃��C���̋���
			const float distanceLineToLine = 124.0f;
			showPos.y += distanceLineToLine;
		}
		warningLineBack[i].reset(WarningLineBack::Create(SpriteTextureLoader::White, showPos, backSize));

		//���C�����E�����ɓ�������
		bool isMoveRight = true;
		if (i == 1) { isMoveRight = false; }
		warningLine[i].reset(WarningLine::Create(SpriteTextureLoader::WarningLine, showPos, isMoveRight));
	}

	//�����w�i�p�X�v���C�g����
	//�傫��
	const Vector2 warningTextBackSize = { WindowApp::window_width, 98 };
	//���W
	Vector2 textBackPos = { WindowApp::window_width / 2, warningLineBack[0]->GetShowPos().y + warningLineBack[0]->GetShowSize().y / 2 + warningTextBackSize.y / 2 };
	warningTextBack.reset(WarningTextBack::Create(SpriteTextureLoader::White, textBackPos, warningTextBackSize));

	//�x���e�L�X�g����
	Vector2 textPos = { WindowApp::window_width / 2, warningTextBack->GetPosition().y + warningTextBack->GetSize().y / 2 };
	warningText.reset(WarningText::Create(SpriteTextureLoader::WarningText, textPos));

	for (int i = 0; i < 2; i++) {
		//�������W���Z�b�g
		Vector2 createPos = { WindowApp::window_width / 2, warningTextBack->GetPosition().y + warningTextBack->GetSize().y / 2 };
		warningLineBack[i]->StartSet(createPos);
		warningLine[i]->StartSet(createPos);
	}

	return true;
}

void BossWarningSprites::Update()
{
	for (int i = 0; i < 2; i++) {
		//�x�����C���p�w�i�X�v���C�g�X�V
		warningLineBack[i]->Update();
		//�x�����C���X�V
		warningLine[i]->Update();
	}
	//�����w�i�p�X�v���C�g�X�V
	warningTextBack->Update();
	//�����X�v���C�g�X�V
	warningText->Update();
}

void BossWarningSprites::Draw()
{
	//�����w�i�p�X�v���C�g�`��
	warningTextBack->Draw();
	//�����X�v���C�g�`��
	warningText->Draw();
	for (int i = 0; i < 2; i++) {
		//�x�����C���p�w�i�X�v���C�g�X�V
		warningLineBack[i]->Draw();
		//�x�����C���X�V
		warningLine[i]->Draw();
	}
}

void BossWarningSprites::Warning()
{
	//�x�����C���X�v���C�g�𓮂���(LeftTop�����炷)
	for (int i = 0; i < 2; i++) {
		warningLine[i]->LineMove();
	}
	//�e�L�X�g�̐F�ύX
	warningText->TextColorChange();
}

void BossWarningSprites::WarningStart(float easeTimer)
{
	//�x���J�n����
	for (int i = 0; i < 2; i++) {
		warningLine[i]->WarningStart(easeTimer);
		warningLineBack[i]->WarningStart(easeTimer);
	}
	warningText->WarningStart(easeTimer);
	warningTextBack->WarningStart(easeTimer);
}

void BossWarningSprites::WarningEnd(float easeTimer)
{
	//�x���I������
	for (int i = 0; i < 2; i++) {
		warningLine[i]->WarningEnd(easeTimer);
		warningLineBack[i]->WarningEnd(easeTimer);
	}
	warningText->WarningEnd(easeTimer);
	warningTextBack->WarningEnd(easeTimer);
}
