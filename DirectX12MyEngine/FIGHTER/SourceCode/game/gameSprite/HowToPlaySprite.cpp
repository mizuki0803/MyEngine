#include "HowToPlaySprite.h"
#include "Easing.h"

void (HowToPlaySprite::* HowToPlaySprite::actionFuncTable[])() = {
	&HowToPlaySprite::TextUp,
	&HowToPlaySprite::TextStay,
	&HowToPlaySprite::TextDown,
};

const Vector2 HowToPlaySprite::stayPos = { 640, 600 };
const Vector2 HowToPlaySprite::outScreenPos = { 640, 900 };

HowToPlaySprite* HowToPlaySprite::Create(const Texture& texture, const Vector2& size, const Vector2& texSize, const Vector2& leftTop)
{
	//�������jUI�̃C���X�^���X�𐶐�
	HowToPlaySprite* howToPlaySprite = new HowToPlaySprite();
	if (howToPlaySprite == nullptr) {
		return nullptr;
	}

	// ������
	if (!howToPlaySprite->Initialize(texture, { 0.5f, 0.5f }, false, false)) {
		delete howToPlaySprite;
		assert(0);
		return nullptr;
	}

	//���W���Z�b�g
	howToPlaySprite->position = outScreenPos;

	//�傫�����Z�b�g
	howToPlaySprite->size = size;
	howToPlaySprite->texSize = texSize;

	//������W���Z�b�g
	howToPlaySprite->texLeftTop = leftTop;

	return howToPlaySprite;
}

void HowToPlaySprite::Update()
{
	//�s��
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//�X�v���C�g�X�V
	Sprite::Update();
}

void HowToPlaySprite::TextUp()
{
	//�^�C�}�[�X�V
	const float upTime = 20;
	actionTimer++;
	const float time = actionTimer / upTime;

	//�C�[�W���O�ŏ�ɓ�����
	position.y = Easing::OutCubic(outScreenPos.y, stayPos.y, time);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (actionTimer >= upTime) {
		//���̃t�F�[�Y��
		phase = ActionPhase::Stay;

		//�^�C�}�[������
		actionTimer = 0;
	}
}

void HowToPlaySprite::TextStay()
{
	//�^�C�}�[�X�V
	const float stayTime = 500;
	actionTimer++;

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (actionTimer >= stayTime) {
		//���̃t�F�[�Y��
		phase = ActionPhase::Down;

		//�^�C�}�[������
		actionTimer = 0;
	}
}

void HowToPlaySprite::TextDown()
{
	//�^�C�}�[�X�V
	const float downTime = 20;
	actionTimer++;
	const float time = actionTimer / downTime;

	//�C�[�W���O�ŏ�ɓ�����
	position.y = Easing::InCubic(stayPos.y, outScreenPos.y, time);

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (actionTimer >= downTime) {
		//���S�t���O�𗧂Ă�
		isDead = true;
	}
}
