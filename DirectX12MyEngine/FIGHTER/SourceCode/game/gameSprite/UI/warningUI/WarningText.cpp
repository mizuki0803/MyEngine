#include "WarningText.h"
#include "Easing.h"

const DirectX::XMFLOAT4 WarningText::warningTextColorMax = { 0.9f, 0.01f, 0.07f, 1.0f };
const DirectX::XMFLOAT4 WarningText::warningTextColorMin = { 0.7f, 0.15f, 0.1f, 1.0f };

WarningText* WarningText::Create(UINT texNumber, const Vector2& position)
{
	//�x���e�L�X�g�̃C���X�^���X�𐶐�
	WarningText* warningText = new WarningText();
	if (warningText == nullptr) {
		return nullptr;
	}

	// ������
	if (!warningText->Initialize(texNumber, position)) {
		delete warningText;
		assert(0);
		return nullptr;
	}

	return warningText;
}

bool WarningText::Initialize(UINT texNumber, const Vector2& position)
{
	//�X�v���C�g������
	if (!Sprite::Initialize(texNumber, { 0.5f, 0.5f }, false, false)) {
		return false;
	}

	//���W���Z�b�g
	this->position = position;
	//�\���p�̑傫�����Z�b�g
	this->showSize = size;
	//�F���Z�b�g(�x���Ȃ̂ŐԂ����Ă���)
	color = warningTextColorMax;

	//���񂾂�o�������̂ő傫��Y��0�ɂ��Ă���
	size.y = 0;

	return true;
}

void WarningText::WarningStart(float easeTimer)
{
	//�傫�����Ă���
	size.y = Easing::LerpFloat(0, showSize.y, easeTimer);
}

void WarningText::WarningEnd(float easeTimer)
{
	//���������Ă���
	size.y = Easing::LerpFloat(showSize.y, 0, easeTimer);
}

void WarningText::TextColorChange()
{
	//�F��ύX����1���[�v�̎���
	const float colorChangeTime = 30;
	//�^�C�}�[�X�V
	textColorChangeTimer++;
	const float time = textColorChangeTimer / colorChangeTime;

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

	//�^�C�}�[���w�肵�����ԂɂȂ�����
	if (textColorChangeTimer >= colorChangeTime) {
		//�^�C�}�[������
		textColorChangeTimer = 0;

		//�F�ύX��Ԃ��t�ɂ���
		if (isTextColorDeep) { isTextColorDeep = false; }
		else { isTextColorDeep = true; }
	}
}
