#include "BossWarning.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"
#include "WindowApp.h"

const float BossWarning::warningColorAlpha = 0.2f;

BossWarning* BossWarning::Create(int32_t warningTime)
{
	//�{�X�o��x���̃C���X�^���X�𐶐�
	BossWarning* bossWarning = new BossWarning();
	if (bossWarning == nullptr) {
		return nullptr;
	}

	// ������
	if (!bossWarning->Initialize()) {
		delete bossWarning;
		assert(0);
		return nullptr;
	}

	//�x�����Ԃ��Z�b�g
	bossWarning->warningTime = warningTime;

	return bossWarning;
}

bool BossWarning::Initialize()
{
	//��ʐԃX�v���C�g����
	screenRedSprite.reset(Sprite::Create(SpriteTextureLoader::White, { 0, 0 }));
	//�傫�����Z�b�g
	screenRedSprite->SetSize({ WindowApp::window_width, WindowApp::window_height });
	//�F���Z�b�g
	screenRedSprite->SetColor({ 1, 0, 0, 0 });


	//������W���Z�b�g
	Vector2 leftTop[2] = { {}, {0, 567} };
	for (int i = 0; i < 2; i++) {
		bossWarningSprites[i].reset(BossWarningSprites::Create(leftTop[i]));
	}

	return true;
}

void BossWarning::Update()
{
	//�x��
	Warning();

	//��ʐԃX�v���C�g�X�V
	screenRedSprite->Update();
	//�x���X�v���C�g�Q�X�V
	for (int i = 0; i < 2; i++) {
		bossWarningSprites[i]->Update();
	}
}

void BossWarning::Draw()
{
	//��ʐԃX�v���C�g�`��
	screenRedSprite->Draw();
	//�x���X�v���C�g�Q�X�V
	for (int i = 0; i < 2; i++) {
		bossWarningSprites[i]->Draw();
	}
}

void BossWarning::Warning()
{
	//�^�C�}�[�X�V
	warningTimer++;

	//�x����ʐԊJ�n
	WarningStart(warningTimer);
	//�X�v���C�g�Q�x������
	for (int i = 0; i < 2; i++) {
		bossWarningSprites[i]->Warning();
	}

	//�x����ʐԏI��
	WarningEnd(warningTimer);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (warningTimer >= warningTime) {
		//�폜��Ԃɂ���
		isDelete = true;
	}
}

void BossWarning::WarningStart(int32_t warningTimer)
{
	//�x���J�n�ɂ����鎞��
	const int warningStartTime = 20;
	//�^�C�}�[�����Ԃ��z���Ă����甲����
	if (warningTimer > warningStartTime) { return; }

	//�C�[�W���O�p���Ԍv��
	const float time = (float)warningTimer / warningStartTime;

	//�C�[�W���O�ŉ�ʂ�Ԃ����Ă���(���ۂɂ͐F��Z������)
	DirectX::XMFLOAT4 color = screenRedSprite->GetColor();
	color.w = Easing::OutQuad(0, warningColorAlpha, time);
	screenRedSprite->SetColor(color);

	//�X�v���C�g�Q�̌x���J�n����
	for (int i = 0; i < 2; i++) {
		bossWarningSprites[i]->WarningStart(time);
	}
}

void BossWarning::WarningEnd(int32_t warningTimer)
{
	//���ɖ߂��̂ɂ����鎞��
	const int warningEndTime = 20;
	//�x���I���ɍ��킹��̂ŏI�����n�߂鎞�Ԃ��v�Z
	const int screenColorReturnStartTime = warningTime - warningEndTime;

	//�^�C�}�[���x���I���ɍ��킹��̂ŏI�����n�߂鎞�Ԉȉ��Ȃ甲����
	if (warningTimer < screenColorReturnStartTime) { return; }

	//�C�[�W���O�p���Ԍv��
	const float time = (float)(warningTimer - screenColorReturnStartTime) / warningEndTime;

	//�C�[�W���O�ŉ�ʐԂ�߂��Ă���(���ۂɂ͐F�𔖂�����)
	DirectX::XMFLOAT4 color = screenRedSprite->GetColor();
	color.w = Easing::InQuad(warningColorAlpha, 0, time);
	screenRedSprite->SetColor(color);

	//�X�v���C�g�Q�̌x���I������
	for (int i = 0; i < 2; i++) {
		bossWarningSprites[i]->WarningEnd(time);
	}
}
