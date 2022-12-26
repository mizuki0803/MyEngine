#include "StageDecisionUI.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"

void (StageDecisionUI::* StageDecisionUI::actionFuncTable[])() = {
	&StageDecisionUI::TextStay,
	&StageDecisionUI::TextUp,
	&StageDecisionUI::TextStay,
	&StageDecisionUI::TextDown,
};

const Vector2 StageDecisionUI::stayPos = { 640, 360 };
const Vector2 StageDecisionUI::outScreenPos = { 640, 1000 };

StageDecisionUI* StageDecisionUI::Create()
{
	//�X�e�[�W����m�FUI�̃C���X�^���X�𐶐�
	StageDecisionUI* stageDecisionUI = new StageDecisionUI();
	if (stageDecisionUI == nullptr) {
		return nullptr;
	}

	// ������
	if (!stageDecisionUI->Initialize()) {
		delete stageDecisionUI;
		assert(0);
		return nullptr;
	}

	return stageDecisionUI;
}

bool StageDecisionUI::Initialize()
{
	//�X�v���C�g����
	stageDecisionSprite.reset(Sprite::Create(SpriteTextureLoader::StageDecision));
	stageDecisionSprite->SetPosition(outScreenPos);
	const Vector2 texSize = { 400, 300 };
	stageDecisionSprite->SetTexSize(texSize);
	const Vector2 size = texSize * 1.5f;
	stageDecisionSprite->SetSize(size);

	return true;
}

void StageDecisionUI::Update()
{
	//�s��
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//�X�v���C�g�X�V
	stageDecisionSprite->Update();
}

void StageDecisionUI::Draw()
{
	//�X�v���C�g�`��
	stageDecisionSprite->Draw();
}

void StageDecisionUI::TextUpStart(int stageNum)
{
	//�X�e�[�W�����肷���ԂɃZ�b�g
	isStageDecision = true;

	//�X�e�[�W�ԍ���0�Ȃ�^�C�g���V�[����I�����Ă����Ԃɂ���
	if (stageNum == 0) { isSelectTitleStage = true; }
	else { isSelectTitleStage = false; }

	//�X�v���C�g�̏�Ԃ��ύX����
	float leftTop = 0;
	if (isStageDecision) { leftTop = 0.0f; }
	else { leftTop = 1.0f; }

	//�^�C�g���V�[����I�����Ă���Ƃ��̂݃e�N�X�`����2���炷
	if (isSelectTitleStage) { leftTop += 2; }

	stageDecisionSprite->SetTexLeftTop({ stageDecisionSprite->GetTexSize().x * leftTop, 0 });

	//�ړ��^�C�}�[������
	moveTimer = 0;

	//��Ԃ�ύX
	phase = ActionPhase::Up;
}

void StageDecisionUI::TextDownStart()
{
	//�ړ��^�C�}�[������
	moveTimer = 0;

	//��Ԃ�ύX
	phase = ActionPhase::Down;
}

void StageDecisionUI::ChangeStageDecision(bool isStageDecision)
{
	//���ɓ�����ԂȂ甲����
	if (this->isStageDecision == isStageDecision) { return; }

	//�X�v���C�g�̏�Ԃ��ύX����
	float leftTop = 0;
	if (isStageDecision) { leftTop = 0.0f; }
	else { leftTop = 1.0f; }

	//�^�C�g���V�[����I�����Ă���Ƃ��̂݃e�N�X�`����2���炷
	if (isSelectTitleStage) { leftTop += 2; }

	stageDecisionSprite->SetTexLeftTop({ stageDecisionSprite->GetTexSize().x * leftTop, 0 });

	//��Ԃ������̏�ԂɃZ�b�g
	this->isStageDecision = isStageDecision;
}

void StageDecisionUI::TextStay()
{
}

void StageDecisionUI::TextUp()
{
	//�^�C�}�[�X�V
	const float upTime = 45;
	moveTimer++;
	const float time = moveTimer / upTime;

	//��ɓ�����
	Vector2 pos = stageDecisionSprite->GetPosition();
	pos.y = Easing::OutCubic(outScreenPos.y, stayPos.y, time);
	stageDecisionSprite->SetPosition(pos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (moveTimer >= upTime) {
		//��Ԃ�ύX
		phase = ActionPhase::Show;
	}
}

void StageDecisionUI::TextDown()
{
	//�^�C�}�[�X�V
	const float downTime = 45;
	moveTimer++;
	const float time = moveTimer / downTime;

	//��ɓ�����
	Vector2 pos = stageDecisionSprite->GetPosition();
	pos.y = Easing::OutCubic(stayPos.y, outScreenPos.y, time);
	stageDecisionSprite->SetPosition(pos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (moveTimer >= downTime) {
		//��Ԃ�ύX
		phase = ActionPhase::Stay;
	}
}
