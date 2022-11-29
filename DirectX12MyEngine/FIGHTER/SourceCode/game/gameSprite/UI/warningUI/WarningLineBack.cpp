#include "WarningLineBack.h"
#include "Easing.h"

WarningLineBack* WarningLineBack::Create(UINT texNumber, const Vector2& showPos, const Vector2 showSize)
{
	//�x�����C���w�i�̃C���X�^���X�𐶐�
	WarningLineBack* warningLineBack = new WarningLineBack();
	if (warningLineBack == nullptr) {
		return nullptr;
	}

	// ������
	if (!warningLineBack->Initialize(texNumber, showPos, showSize)) {
		delete warningLineBack;
		assert(0);
		return nullptr;
	}

	return warningLineBack;
}

bool WarningLineBack::Initialize(UINT texNumber, const Vector2& showPos, const Vector2 showSize)
{
	//�X�v���C�g������
	if (!Sprite::Initialize(texNumber, { 0.5f, 0.5f }, false, false)) {
		return false;
	}

	//�\�����W���Z�b�g
	this->showPos = showPos;
	//�\���p�̑傫�����Z�b�g
	size = showSize;
	this->showSize = size;
	//�F���Z�b�g
	const DirectX::XMFLOAT4 backColor = { 0.9f, 0.5f, 0.5f, 0.8f };
	color = backColor;

	//���񂾂�o�������̂ő傫��Y��0�ɂ��Ă���
	size.y = 0;

	return true;
}

void WarningLineBack::StartSet(const Vector2& createPos)
{
	//�������W���Z�b�g
	this->createPos = createPos;
	//���W�𐶐����W�ɃZ�b�g
	position = createPos;
}

void WarningLineBack::WarningStart(float easeTimer)
{
	//�������W����ړ����đ傫�����Ă���
	position.y = Easing::LerpFloat(createPos.y, showPos.y, easeTimer);
	size.y = Easing::LerpFloat(0, showSize.y, easeTimer);
}

void WarningLineBack::WarningEnd(float easeTimer)
{
	//���������Ȃ��琶�����W�ɖ߂��Ă���
	position.y = Easing::LerpFloat(showPos.y, createPos.y, easeTimer);
	size.y = Easing::LerpFloat(showSize.y, 0, easeTimer);
}
