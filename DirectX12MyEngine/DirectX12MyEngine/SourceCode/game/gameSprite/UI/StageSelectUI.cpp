#include "StageSelectUI.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"
#include "EnemyDefeatCounter.h"

void (StageSelectUI::* StageSelectUI::actionFuncTable[])() = {
	&StageSelectUI::TextStay,
	&StageSelectUI::TextInScreen,
	&StageSelectUI::TextStay,
	&StageSelectUI::TextOutScreen,
};

const float StageSelectUI::inScreenPosY = 100;
const float StageSelectUI::outScreenPosY = -100;
const float StageSelectUI::frameNumberDistanceY = 25;

StageSelectUI* StageSelectUI::Create()
{
	//�X�e�[�W�I�����UI�̃C���X�^���X�𐶐�
	StageSelectUI* stageSelectUI = new StageSelectUI();
	if (stageSelectUI == nullptr) {
		return nullptr;
	}

	// ������
	if (!stageSelectUI->Initialize()) {
		delete stageSelectUI;
		assert(0);
		return nullptr;
	}

	return stageSelectUI;
}

bool StageSelectUI::Initialize()
{
	//�f�����X�v���C�g����
	planetNameSprite.reset(Sprite::Create(SpriteTextureLoader::PlanetName));
	planetNameSprite->SetPosition({ 640, outScreenPosY });
	const Vector2 planetNameTexSize = { 500, 140 };
	planetNameSprite->SetTexSize(planetNameTexSize);
	planetNameSprite->SetSize(planetNameTexSize);

	//�n�C�X�R�A�g�X�v���C�g����
	highScoreFrameSprite.reset(Sprite::Create(SpriteTextureLoader::HighScoreFrame));
	highScoreFrameSprite->SetPosition({ 1000, outScreenPosY });
	const Vector2 frameTexSize = { 172, 126 };
	highScoreFrameSprite->SetTexSize(frameTexSize);
	highScoreFrameSprite->SetSize(frameTexSize);

	//�����X�v���C�g����
	for (int i = 0; i < highScoreDigit; i++) {
		const Vector2 texSize = { 48, 48 };
		const Vector2 size = texSize * 0.8f;
		const Vector2 centerPos = highScoreFrameSprite->GetPosition();
		const Vector2 firstPos = { centerPos.x + size.x, centerPos.y + frameNumberDistanceY };
		const Vector2 pos = { firstPos.x - i * size.x, firstPos.y };
		numberSprites[i].reset(NumberSprite::Create(SpriteTextureLoader::HitPlusNumber, pos, size, texSize));
	}

	//��������X�v���C�g����
	howToPlaySprite.reset(Sprite::Create(SpriteTextureLoader::StageSelectHowToPlay));
	howToPlaySprite->SetPosition({ 640, 550 });
	const Vector2 howToPLayTexSize = { 200, 64 };
	howToPlaySprite->SetTexSize(howToPLayTexSize);
	howToPlaySprite->SetSize(howToPLayTexSize);

	return true;
}

void StageSelectUI::Update()
{
	//�s��
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//�X�v���C�g�X�V
	planetNameSprite->Update();
	highScoreFrameSprite->Update();
	for (int i = 0; i < highScoreDigit; i++) {
		numberSprites[i]->Update();
	}
	howToPlaySprite->Update();
}

void StageSelectUI::Draw()
{
	//�X�v���C�g�`��
	planetNameSprite->Draw();
	highScoreFrameSprite->Draw();
	for (int i = 0; i < highScoreDigit; i++) {
		numberSprites[i]->Draw();
	}

	//�\����ԂłȂ���Δ�����
	if (!(phase == ActionPhase::Show)) { return; }

	howToPlaySprite->Draw();
}

void StageSelectUI::TextInScreenStart(const int stageNum)
{
	//�^�C�}�[��������������
	moveTimer = 0;

	//�X�e�[�W�ԍ��ɍ��킹�ĕ\��������̂����߂�
	planetNameSprite->SetTexLeftTop({ planetNameSprite->GetTexSize().x * stageNum, 0 });

	//�n�C�X�R�A��\�����邩����
	if (stageNum == 1) { isHighScoreShow = true; }
	else { isHighScoreShow = false; }

	//�n�C�X�R�A��\������ꍇ
	if (isHighScoreShow) {
		//�����X�v���C�g���n�C�X�R�A�ɍX�V
		const int highScore = EnemyDefeatCounter::GetHighScore();

		//�n�C�X�R�A��0(���ݒ�)�̏ꍇ�͖_��\��
		if (highScore == 0) {
			//�����̕��A�����X�V
			for (int i = 0; i < highScoreDigit; i++) {
				numberSprites[i]->SetNumber(10);
			}
		}
		//�n�C�X�R�A��0�ȊO���ݒ肳��Ă���ꍇ
		else {
			//�����̕��A�����X�V
			for (int i = 0; i < highScoreDigit; i++) {
				//���鐔
				int divisionNum = 1;
				//������10����Z����(1,10,100,...)
				for (int j = 0; j < i; j++) {
					divisionNum *= 10;
				}
				//�w�肵�����̐������o��
				int numberDigit = (highScore / divisionNum) % 10;
				//�Z�o�������̐����𐔎��X�v���C�g�Ɋ��蓖�Ă�
				numberSprites[i]->SetNumber(numberDigit);
			}
		}
	}

	//��������X�v���C�g���X�e�[�W�ɍ��킹�ĕύX
	howToPlaySprite->SetTexLeftTop({ howToPlaySprite->GetTexSize().x * stageNum, 0 });

	//��ʓ��Ɉړ������ԂɕύX
	phase = ActionPhase::InScreen;

	//�\�����Ă����Ԃɂ���
	isShow = true;
}

void StageSelectUI::TextOutScreenStart()
{
	//�^�C�}�[��������������
	moveTimer = 0;

	//��ʊO�Ɉړ������ԂɕύX
	phase = ActionPhase::OutScreen;

	//��ʊO�Ɉړ��J�n������W���Z�b�g
	moveOutScreenStartPos = planetNameSprite->GetPosition();

	//�\�����Ă��Ȃ���Ԃɂ���
	isShow = false;
}

void StageSelectUI::TextStay()
{
}

void StageSelectUI::TextInScreen()
{
	//�^�C�}�[�X�V
	const float moveTime = 45;
	moveTimer++;
	const float time = moveTimer / moveTime;

	//��ʓ��ɓ�����
	Vector2 pos = planetNameSprite->GetPosition();
	pos.y = Easing::OutCubic(outScreenPosY, inScreenPosY, time);
	planetNameSprite->SetPosition(pos);
	//�n�C�X�R�A��\������ꍇ
	if (isHighScoreShow) {
		//�g�X�v���C�g
		Vector2 framePos = highScoreFrameSprite->GetPosition();
		framePos.y = Easing::OutCubic(outScreenPosY, inScreenPosY, time);
		highScoreFrameSprite->SetPosition(framePos);

		//�����X�v���C�g
		for (int i = 0; i < highScoreDigit; i++) {
			Vector2 numberPos = numberSprites[i]->GetPosition();
			numberPos.y = framePos.y + frameNumberDistanceY;
			numberSprites[i]->SetPosition(numberPos);
		}
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (moveTimer >= moveTime) {
		//��Ԃ�ύX
		phase = ActionPhase::Show;
	}
}

void StageSelectUI::TextOutScreen()
{
	//�^�C�}�[�X�V
	const float moveTime = 45;
	moveTimer++;
	const float time = moveTimer / moveTime;

	//��ʊO�ɓ�����
	Vector2 pos = planetNameSprite->GetPosition();
	pos.y = Easing::OutCubic(moveOutScreenStartPos.y, outScreenPosY, time);
	planetNameSprite->SetPosition(pos);
	//�n�C�X�R�A��\������ꍇ
	if (isHighScoreShow) {
		//�g�X�v���C�g
		Vector2 framePos = highScoreFrameSprite->GetPosition();
		framePos.y = Easing::OutCubic(moveOutScreenStartPos.y, outScreenPosY, time);
		highScoreFrameSprite->SetPosition(framePos);

		//�����X�v���C�g
		for (int i = 0; i < highScoreDigit; i++) {
			Vector2 numberPos = numberSprites[i]->GetPosition();
			numberPos.y = framePos.y + frameNumberDistanceY;
			numberSprites[i]->SetPosition(numberPos);
		}
	}

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (moveTimer >= moveTime) {
		//��Ԃ�ύX
		phase = ActionPhase::Stay;
	}
}