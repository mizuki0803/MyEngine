#include "StageResultUI.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"

StageResultUI* StageResultUI::Create(const int enemyDefeatNum, bool isHighScore)
{
	//�X�e�[�W���U���gUI�̃C���X�^���X�𐶐�
	StageResultUI* stageResultUI = new StageResultUI();
	if (stageResultUI == nullptr) {
		return nullptr;
	}

	// ������
	if (!stageResultUI->Initialize(enemyDefeatNum)) {
		delete stageResultUI;
		assert(0);
		return nullptr;
	}

	return stageResultUI;
}

bool StageResultUI::Initialize(const int enemyDefeatNum)
{
	//���j���擾
	this->enemyDefeatNum = enemyDefeatNum;

	//�g�X�v���C�g����
	frameSprite.reset(Sprite::Create(SpriteTextureLoader::DefeatNumText));
	const Vector2 framePos = { 640, 360 };
	frameSprite->SetPosition(framePos);
	const Vector2 frameSize = { 508, 108 };
	frameSprite->SetSize(frameSize);
	frameSprite->SetTexSize(frameSize);

	//�������擾
	int defeatNum = enemyDefeatNum;
	while (defeatNum != 0) {
		defeatNum = defeatNum / 10;
		enemyDefeatNumDigit++;
	}

	//�����̕��A�����X�v���C�g����
	for (int i = 0; i < enemyDefeatNumDigit; i++) {
		std::unique_ptr<NumberSprite> newNumberSprite;
		const Vector2 size = { 32, 48 };
		const Vector2 pos = { framePos.x + 150 - (i * size.x), framePos.y };
		newNumberSprite.reset(NumberSprite::Create(SpriteTextureLoader::Number, pos, size, size));
		numberSprites.push_back(std::move(newNumberSprite));
	}

	//B�{�^���X�v���C�g����
	bButtonSprite.reset(Sprite::Create(SpriteTextureLoader::BButton));
	const Vector2 bButtonPos = { framePos.x, framePos.y + 90 };
	bButtonSprite->SetPosition(bButtonPos);
	const Vector2 bButtonSize = { 48, 48 };
	bButtonSprite->SetSize(bButtonSize);
	const Vector2 bButtonTexSize = { 64, 64 };
	bButtonSprite->SetTexSize(bButtonTexSize);

	return true;
}

void StageResultUI::Update()
{
	//�\���p���j���̍X�V
	UpdateDisplayNum();

	//�g�X�v���C�g�X�V
	frameSprite->Update();
	//�\���p���j���̕������X�v���C�g�X�V
	for (int i = 0; i < enemyDefeatDisplayNumDigit; i++) {
		numberSprites[i]->Update();
	}
	//B�{�^���X�v���C�g�X�V
	if (isDrawButtonSprite) {
		bButtonSprite->Update();
	}
}

void StageResultUI::Draw()
{
	//�g�X�v���C�g�`��
	frameSprite->Draw();
	//�\���p���j���̕������X�v���C�g�`��
	for (int i = 0; i < enemyDefeatDisplayNumDigit; i++) {
		numberSprites[i]->Draw();
	}
	//B�{�^���X�v���C�g�`��
	if (isDrawButtonSprite) {
		bButtonSprite->Draw();
	}
}

void StageResultUI::UpdateDisplayNum()
{
	//���U���g��\�����I���Ă����甲����
	if (isResultEnd) { return; }

	//�X�V�J�n���鎞��
	const int updateStartTime = 30;
	//�^�C�}�[�X�V
	updateDisplayNumTimer++;

	//�^�C�}�[���X�V�J�n���Ԃ�菬�����Ȃ甲����
	if (updateDisplayNumTimer < updateStartTime) { return; }

	//�X�V���鎞��
	const int updateMinTime = 120;
	const float updateTime = (float)(updateMinTime + (enemyDefeatNum / 10));
	const float time = (updateDisplayNumTimer - updateStartTime) / updateTime;

	//�\���p���j���𑝂₵�Ă���
	enemyDefeatDisplayNum = (int)Easing::LerpFloat(0, (float)enemyDefeatNum, time);

	//�\���p���j���̌������X�V
	int defeatNum = enemyDefeatDisplayNum;
	int digit = 0;
	while (defeatNum != 0) {
		defeatNum = defeatNum / 10;
		digit++;
	}
	enemyDefeatDisplayNumDigit = digit;

	//�����X�v���C�g�X�V
	UpdateNumberSprite();

	//�^�C�}�[���w�肵�����ԂɂȂ����甲����
	if ((updateDisplayNumTimer - updateStartTime) >= updateTime) {
		//���U���g�\���������������Ƃɂ���
		isResultEnd = true;
	}
}

void StageResultUI::UpdateNumberSprite()
{
	//���������ꂼ��o�͂���
	for (int i = 0; i < enemyDefeatDisplayNumDigit; i++) {
		//���鐔
		int divisionNum = 1;
		//������10����Z����(1,10,100,...)
		for (int j = 0; j < i; j++) {
			divisionNum *= 10;
		}

		//�w�肵�����̐������o��
		int numberDigit = (enemyDefeatDisplayNum / divisionNum) % 10;

		//�Z�o�������̐����𐔎��X�v���C�g�Ɋ��蓖�Ă�
		numberSprites[i]->SetNumber(numberDigit);
	}
}
