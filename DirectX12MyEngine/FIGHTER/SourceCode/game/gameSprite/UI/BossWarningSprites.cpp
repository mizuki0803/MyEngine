#include "BossWarningSprites.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"
#include "WindowApp.h"

const float BossWarning::warningColorAlpha = 0.4f;
const DirectX::XMFLOAT4 BossWarning::warningTextColorMax = { 0.9f, 0.01f, 0.07f, 1.0f };
const DirectX::XMFLOAT4 BossWarning::warningTextColorMin = { 0.7f, 0.15f, 0.1f, 1.0f };

BossWarning* BossWarning::Create(int32_t warningTime)
{
	//�{�X�o��x�����o�̃C���X�^���X�𐶐�
	BossWarning* bossWarningEffect = new BossWarning();
	if (bossWarningEffect == nullptr) {
		return nullptr;
	}

	// ������
	if (!bossWarningEffect->Initialize()) {
		delete bossWarningEffect;
		assert(0);
		return nullptr;
	}

	//�x�����Ԃ��Z�b�g
	bossWarningEffect->warningTime = warningTime;

	return bossWarningEffect;
}

bool BossWarning::Initialize()
{
	//��ʐԃX�v���C�g����
	screenRedSprite.reset(Sprite::Create(SpriteTextureLoader::White, { 0, 0 }));
	//�傫�����Z�b�g
	screenRedSprite->SetSize({ WindowApp::window_width, WindowApp::window_height });
	//�F���Z�b�g
	screenRedSprite->SetColor({ 1, 0, 0, 0 });
	
	//��ɕ\������x���X�v���C�g�Q
	{
		//���C���̖{������
		for (int i = 0; i < 2; i++) {
			//�x�����C���p�w�i�X�v���C�g����
			lineBackSprite[i].reset(Sprite::Create(SpriteTextureLoader::White));
			//�傫�����Z�b�g
			const Vector2 backSize = { WindowApp::window_width, 29 };
			lineBackSprite[i]->SetSize(backSize);
			//���W���Z�b�g
			Vector2 pos = { WindowApp::window_width / 2, lineBackSprite[i]->GetSize().y / 2 };
			if (i == 1) {
				//��̃��C���Ɖ��̃��C���̋���
				const float distanceLineToLine = 124.0f;
				pos.y += distanceLineToLine;
			}
			lineBackSprite[i]->SetPosition(pos);
			//�F���Z�b�g
			const DirectX::XMFLOAT4 backColor = { 0.9f, 0.5f, 0.5f, 0.8f };
			lineBackSprite[i]->SetColor(backColor);


			//�x�����C���X�v���C�g����
			lineSprite[i].reset(Sprite::Create(SpriteTextureLoader::WarningLine));
			//���W���Z�b�g
			lineSprite[i]->SetPosition(pos);
			//�F���Z�b�g
			const DirectX::XMFLOAT4 lineColor = { 1, 0.3f, 0.3f, 0.8f };
			lineSprite[i]->SetColor(lineColor);
		}

		//�����w�i�p�X�v���C�g����
		textBackSprite.reset(Sprite::Create(SpriteTextureLoader::White, { 0.5f, 0 }));
		//���W���Z�b�g
		Vector2 textBackPos = { WindowApp::window_width / 2, lineBackSprite[0]->GetPosition().y + lineBackSprite[0]->GetSize().y / 2 };
		textBackSprite->SetPosition(textBackPos);
		//�傫�����Z�b�g
		textBackSprite->SetSize({ WindowApp::window_width, 98 });
		//�x���Ȃ̂ŐԂ����Ă���(���킶��o�������̂œ����ɂ��Ă���)
		DirectX::XMFLOAT4 textBackColor = { 0.6f, 0.2f, 0.2f, 0.9f };
		textBackSprite->SetColor(textBackColor);

		//�����X�v���C�g����
		textSprite.reset(Sprite::Create(SpriteTextureLoader::WarningText));
		//���W���Z�b�g(�����p�w�i�Ɠ������W���Z�b�g)
		Vector2 textPos = { WindowApp::window_width / 2, textBackSprite->GetPosition().y + textBackSprite->GetSize().y / 2 };
		textSprite->SetPosition(textPos);
		//�x���Ȃ̂ŐԂ����Ă���
		textSprite->SetColor(warningTextColorMax);
	}
	//���ɕ\������x���X�v���C�g�Q
	{
		//���C���̖{������
		for (int i = 0; i < 2; i++) {
			//�x�����C���p�w�i�X�v���C�g����
			lineBackSpriteDown[i].reset(Sprite::Create(SpriteTextureLoader::White));
			//�傫�����Z�b�g
			const Vector2 backSize = { WindowApp::window_width, 29 };
			lineBackSpriteDown[i]->SetSize(backSize);
			//���W���Z�b�g
			Vector2 pos = { WindowApp::window_width / 2, 567 + lineBackSpriteDown[i]->GetSize().y / 2 };
			if (i == 1) {
				//��̃��C���Ɖ��̃��C���̋���
				const float distanceLineToLine = 124.0f;
				pos.y += distanceLineToLine;
			}
			lineBackSpriteDown[i]->SetPosition(pos);
			//�F���Z�b�g
			const DirectX::XMFLOAT4 backColor = { 0.9f, 0.5f, 0.5f, 0.8f };
			lineBackSpriteDown[i]->SetColor(backColor);


			//�x�����C���X�v���C�g����
			lineSpriteDown[i].reset(Sprite::Create(SpriteTextureLoader::WarningLine));
			//���W���Z�b�g
			lineSpriteDown[i]->SetPosition(pos);
			//�F���Z�b�g
			const DirectX::XMFLOAT4 lineColor = { 1, 0.3f, 0.3f, 0.8f };
			lineSpriteDown[i]->SetColor(lineColor);
		}

		//�����w�i�p�X�v���C�g����
		textBackSpriteDown.reset(Sprite::Create(SpriteTextureLoader::White, { 0.5f, 0 }));
		//���W���Z�b�g
		Vector2 textBackPos = { WindowApp::window_width / 2, lineBackSpriteDown[0]->GetPosition().y + lineBackSpriteDown[0]->GetSize().y / 2 };
		textBackSpriteDown->SetPosition(textBackPos);
		//�傫�����Z�b�g
		textBackSpriteDown->SetSize({ WindowApp::window_width, 98 });
		//�x���Ȃ̂ŐԂ����Ă���(���킶��o�������̂œ����ɂ��Ă���)
		DirectX::XMFLOAT4 textBackColor = { 0.6f, 0.2f, 0.2f, 0.9f };
		textBackSpriteDown->SetColor(textBackColor);

		//�����X�v���C�g����
		textSpriteDown.reset(Sprite::Create(SpriteTextureLoader::WarningText));
		//���W���Z�b�g(�����p�w�i�Ɠ������W���Z�b�g)
		Vector2 textPos = { WindowApp::window_width / 2, textBackSpriteDown->GetPosition().y + textBackSpriteDown->GetSize().y / 2 };
		textSpriteDown->SetPosition(textPos);
		//�x���Ȃ̂ŐԂ����Ă���
		textSpriteDown->SetColor(warningTextColorMax);
	}

	return true;
}

void BossWarning::Update()
{
	//�x��
	Warning();

	//��ʐԃX�v���C�g�X�V
	screenRedSprite->Update();
	//�����w�i�p�X�v���C�g�X�V
	textBackSprite->Update();
	//�����X�v���C�g�X�V
	textSprite->Update();
	for (int i = 0; i < 2; i++) {
		//�x�����C���p�w�i�X�v���C�g�X�V
		lineBackSprite[i]->Update();
		//�x�����C���X�v���C�g�X�V
		lineSprite[i]->Update();
	}

	//�����w�i�p�X�v���C�g�X�V
	textBackSpriteDown->Update();
	//�����X�v���C�g�X�V
	textSpriteDown->Update();
	for (int i = 0; i < 2; i++) {
		//�x�����C���p�w�i�X�v���C�g�X�V
		lineBackSpriteDown[i]->Update();
		//�x�����C���X�v���C�g�X�V
		lineSpriteDown[i]->Update();
	}
}

void BossWarning::Draw()
{
	//��ʐԃX�v���C�g�`��
	screenRedSprite->Draw();
	//�����w�i�p�X�v���C�g�`��
	textBackSprite->Draw();
	//�����X�v���C�g�`��
	textSprite->Draw();
	for (int i = 0; i < 2; i++) {
		//�x�����C���p�w�i�X�v���C�g�X�V
		lineBackSprite[i]->Draw();
		//�x�����C���X�v���C�g�X�V
		lineSprite[i]->Draw();
	}


	//�����w�i�p�X�v���C�g�`��
	textBackSpriteDown->Draw();
	//�����X�v���C�g�`��
	textSpriteDown->Draw();
	for (int i = 0; i < 2; i++) {
		//�x�����C���p�w�i�X�v���C�g�X�V
		lineBackSpriteDown[i]->Draw();
		//�x�����C���X�v���C�g�X�V
		lineSpriteDown[i]->Draw();
	}
}

void BossWarning::Warning()
{
	//�x���^�C�}�[�X�V
	warningTimer++;

	//�x����ʐԊJ�n
	WarningStart(warningTimer);

	//�e�L�X�g�̐F�ύX
	TextColorChange();

	//�x����ʐԏI��
	WarningEnd(warningTimer);

	//�x�����C���X�v���C�g�𓮂���(LeftTop�����炷)
	for (int i = 0; i < 2; i++) {
		Vector2 texLeftTop = lineSprite[i]->GetTexLeftTop();
		Vector2 downTexLeftTop = lineSpriteDown[i]->GetTexLeftTop();
		const float moveSpeed = 3.0f;
		if (i == 0) { texLeftTop.x += moveSpeed; downTexLeftTop.x += moveSpeed; }
		else if (i == 1) { texLeftTop.x -= moveSpeed;  downTexLeftTop.x -= moveSpeed; }
		lineSprite[i]->SetTexLeftTop(texLeftTop);
		lineSpriteDown[i]->SetTexLeftTop(downTexLeftTop);
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (warningTimer >= warningTime) {
		//�폜��Ԃɂ���
		isDelete = true;
	}
}

void BossWarning::WarningStart(int32_t warningTimer)
{
	//�x���J�n�ɂ����鎞��
	const int warningStartTime = 60;
	//�^�C�}�[�����Ԃ��z���Ă����甲����
	if (warningTimer >= warningStartTime) { return; }

	//�C�[�W���O�p���Ԍv��
	const float time = (float)warningTimer / warningStartTime;

	//�C�[�W���O�ŉ�ʂ�Ԃ����Ă���(���ۂɂ͐F��Z������)
	DirectX::XMFLOAT4 color = screenRedSprite->GetColor();
	color.w = Easing::LerpFloat(0, warningColorAlpha, time);
	screenRedSprite->SetColor(color);
}

void BossWarning::WarningEnd(int32_t warningTimer)
{
	//���ɖ߂��̂ɂ����鎞��
	const int warningEndTime = 60;
	//�x���I���ɍ��킹��̂ŏI�����n�߂鎞�Ԃ��v�Z
	const int screenColorReturnStartTime = warningTime - warningEndTime;

	//�^�C�}�[���x���I���ɍ��킹��̂ŏI�����n�߂鎞�Ԉȉ��Ȃ甲����
	if (warningTimer < screenColorReturnStartTime) { return; }

	//�C�[�W���O�p���Ԍv��
	const float time = (float)(warningTimer - screenColorReturnStartTime) / warningEndTime;

	//�C�[�W���O�ŉ�ʐԂ�߂��Ă���(���ۂɂ͐F�𔖂�����)
	DirectX::XMFLOAT4 color = screenRedSprite->GetColor();
	color.w = Easing::LerpFloat(warningColorAlpha, 0, time);
	screenRedSprite->SetColor(color);
}

void BossWarning::TextColorChange()
{
	//�F��ύX����1���[�v�̎���
	const float colorChangeTime = 30;
	//�^�C�}�[�X�V
	textColorChangeTimer++;
	const float time = textColorChangeTimer / colorChangeTime;

	DirectX::XMFLOAT4 color = { 0, 0, 0, 1 };

	//�Z������
	if (isTextColorDeep) {
		color.x = Easing::LerpFloat(warningTextColorMin.x, warningTextColorMax.x, time);
		color.y = Easing::LerpFloat(warningTextColorMin.y, warningTextColorMax.y, time);
		color.z = Easing::LerpFloat(warningTextColorMin.z, warningTextColorMax.z, time);
	}
	//��������
	else {
		color.x = Easing::LerpFloat(warningTextColorMax.x, warningTextColorMin.x, time);
		color.y = Easing::LerpFloat(warningTextColorMax.y, warningTextColorMin.y, time);
		color.z = Easing::LerpFloat(warningTextColorMax.z, warningTextColorMin.z, time);
	}
	textSprite->SetColor(color);
	textSpriteDown->SetColor(color);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (textColorChangeTimer >= colorChangeTime) {
		//�^�C�}�[������
		textColorChangeTimer = 0;

		//�F�ύX��Ԃ��t�ɂ���
		if (isTextColorDeep) { isTextColorDeep = false; }
		else { isTextColorDeep = true; }
	}
}
