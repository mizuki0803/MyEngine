#include "HowToPlayUI.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"

void (HowToPlayUI::* HowToPlayUI::actionFuncTable[])() = {
	&HowToPlayUI::TextUp,
	&HowToPlayUI::TextStay,
	&HowToPlayUI::TextDown,
};

const Vector2 HowToPlayUI::stayPos = { 640, 600 };
const Vector2 HowToPlayUI::outScreenPos = { 640, 900 };

HowToPlayUI* HowToPlayUI::Create()
{
	//�������jUI�̃C���X�^���X�𐶐�
	HowToPlayUI* howToPlayUI = new HowToPlayUI();
	if (howToPlayUI == nullptr) {
		return nullptr;
	}

	// ������
	if (!howToPlayUI->Initialize()) {
		delete howToPlayUI;
		assert(0);
		return nullptr;
	}

	return howToPlayUI;
}

bool HowToPlayUI::Initialize()
{
	//�g�X�v���C�g����
	chargeHowToPlaySprite.reset(Sprite::Create(SpriteTextureLoader::HowToPlayCharge));
	chargeHowToPlaySprite->SetPosition(outScreenPos);
	const Vector2 texSize = { 740, 100 };
	const Vector2 size = texSize * 0.8f;
	chargeHowToPlaySprite->SetSize(size);
	chargeHowToPlaySprite->SetTexSize(texSize);

	return true;
}

void HowToPlayUI::Update()
{
	//�X�v���C�g�X�V
	chargeHowToPlaySprite->Update();
}

void HowToPlayUI::Draw()
{
	//�s��
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//�X�v���C�g�`��
	chargeHowToPlaySprite->Draw();
}

void HowToPlayUI::TextUp()
{
	//�^�C�}�[�X�V
	const float upTime = 20;
	actionTimer++;
	const float time = actionTimer / upTime;

	//�C�[�W���O�ŏ�ɓ�����
	Vector2 pos = chargeHowToPlaySprite->GetPosition();
	pos.y = Easing::OutCubic(outScreenPos.y, stayPos.y, time);
	chargeHowToPlaySprite->SetPosition(pos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (actionTimer >= upTime) {
		//���̃t�F�[�Y��
		phase = ActionPhase::Stay;
		
		//�^�C�}�[������
		actionTimer = 0;
	}
}

void HowToPlayUI::TextStay()
{
	//�^�C�}�[�X�V
	const float stayTime = 700;
	actionTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (actionTimer >= stayTime) {
		//���̃t�F�[�Y��
		phase = ActionPhase::Down;

		//�^�C�}�[������
		actionTimer = 0;
	}
}

void HowToPlayUI::TextDown()
{
	//�^�C�}�[�X�V
	const float downTime = 20;
	actionTimer++;
	const float time = actionTimer / downTime;

	//�C�[�W���O�ŏ�ɓ�����
	Vector2 pos = chargeHowToPlaySprite->GetPosition();
	pos.y = Easing::InCubic(stayPos.y, outScreenPos.y, time);
	chargeHowToPlaySprite->SetPosition(pos);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (actionTimer >= downTime) {
		//���S�t���O�𗧂Ă�
		isDead = true;
	}
}
