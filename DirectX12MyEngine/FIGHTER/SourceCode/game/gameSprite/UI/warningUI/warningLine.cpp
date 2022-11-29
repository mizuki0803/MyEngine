#include "warningLine.h"
#include "Easing.h"

WarningLine* WarningLine::Create(UINT texNumber, const Vector2& showPos, const bool isMoveRight)
{
	//�x�����C���̃C���X�^���X�𐶐�
	WarningLine* warningLine = new WarningLine();
	if (warningLine == nullptr) {
		return nullptr;
	}

	// ������
	if (!warningLine->Initialize(texNumber, showPos, isMoveRight)) {
		delete warningLine;
		assert(0);
		return nullptr;
	}

	return warningLine;
}

bool WarningLine::Initialize(UINT texNumber, const Vector2& showPos, const bool isMoveRight)
{
	//�X�v���C�g������
	if (!Sprite::Initialize(texNumber, { 0.5f, 0.5f }, false, false)) {
		return false;
	}

	//�\�����W���Z�b�g
	this->showPos = showPos;
	//�\���p�̑傫�����Z�b�g
	this->showSize = size;
	//�F���Z�b�g
	const DirectX::XMFLOAT4 lineColor = { 1, 0.3f, 0.3f, 0.8f };
	color = lineColor;

	//���񂾂�o�������̂ő傫��Y��0�ɂ��Ă���
	size.y = 0;

	//�E�Ɉړ����邩
	this->isMoveRight = isMoveRight;

	return true;
}

void WarningLine::StartSet(const Vector2& createPos)
{
	//�������W���Z�b�g
	this->createPos = createPos;
	//���W�𐶐����W�ɃZ�b�g
	position = createPos;
}

void WarningLine::WarningStart(float easeTimer)
{
	//�������W����ړ����đ傫�����Ă���
	position.y = Easing::LerpFloat(createPos.y, showPos.y, easeTimer);
	size.y = Easing::LerpFloat(0, showSize.y, easeTimer);

	//���C�����ړ�������
	LineMove();
}

void WarningLine::WarningEnd(float easeTimer)
{
	//���������Ȃ��琶�����W�ɖ߂��Ă���
	position.y = Easing::LerpFloat(showPos.y, createPos.y, easeTimer);
	size.y = Easing::LerpFloat(showSize.y, 0, easeTimer);

	//���C�����ړ�������
	LineMove();
}

void WarningLine::LineMove()
{
	//������W�����炷
	const float moveSpeed = 3.0f;
	if (isMoveRight) { texLeftTop.x += moveSpeed; }
	else { texLeftTop.x -= moveSpeed; }
}
