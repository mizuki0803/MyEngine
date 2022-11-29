#include "WarningTextBack.h"
#include "Easing.h"

WarningTextBack* WarningTextBack::Create(UINT texNumber, const Vector2& position, const Vector2 size)
{
	//�x���e�L�X�g�w�i�̃C���X�^���X�𐶐�
	WarningTextBack* warningTextBack = new WarningTextBack();
	if (warningTextBack == nullptr) {
		return nullptr;
	}

	// ������
	if (!warningTextBack->Initialize(texNumber, position, size)) {
		delete warningTextBack;
		assert(0);
		return nullptr;
	}

	return warningTextBack;
}

bool WarningTextBack::Initialize(UINT texNumber, const Vector2& position, const Vector2 size)
{
	//�X�v���C�g������
	if (!Sprite::Initialize(texNumber, { 0.5f, 0.5f }, false, false)) {
		return false;
	}

	//���W���Z�b�g
	this->position = position;
	//�\���p�̑傫�����Z�b�g
	this->size = size;
	showSize = this->size;
	//�F���Z�b�g(�x���Ȃ̂ŐԂ����Ă���)
	DirectX::XMFLOAT4 textBackColor = { 0.6f, 0.2f, 0.2f, 0.9f };
	color = textBackColor;

	//���񂾂�o�������̂ő傫��Y��0�ɂ��Ă���
	this->size.y = 0;

	return true;
}

void WarningTextBack::WarningStart(float easeTimer)
{
	//�傫�����Ă���
	size.y = Easing::LerpFloat(0, showSize.y, easeTimer);
}

void WarningTextBack::WarningEnd(float easeTimer)
{
	//���������Ă���
	size.y = Easing::LerpFloat(showSize.y, 0, easeTimer);
}
