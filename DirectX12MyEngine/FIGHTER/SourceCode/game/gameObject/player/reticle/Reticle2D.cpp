#include "Reticle2D.h"
#include "Easing.h"

const XMFLOAT4 Reticle2D::normalColor = { 0.5f, 0.9f, 0.2f, 1 };

Reticle2D* Reticle2D::Create(const Texture& texture, const Vector2& size)
{
	//2D���e�B�N���̃C���X�^���X�𐶐�
	Reticle2D* reticle2d = new Reticle2D();
	if (reticle2d == nullptr) {
		return nullptr;
	}

	// ������
	if (!reticle2d->Initialize(texture, { 0.5f, 0.5f }, false, false)) {
		delete reticle2d;
		assert(0);
		return nullptr;
	}

	//�傫�����Z�b�g
	reticle2d->size = size;

	return reticle2d;
}

bool Reticle2D::Initialize(const Texture& texture, const Vector2& anchorpoint, bool isFlipX, bool isFlipY)
{
	if (!Sprite::Initialize(texture, anchorpoint, isFlipX, isFlipY)) {
		return false;
	}

	//�ʏ�F���Z�b�g
	color = normalColor;

	return true;
}

void Reticle2D::Update()
{
	//�ړ�
	Move();

	//�X�v���C�g�̍X�V
	Sprite::Update();
}

void Reticle2D::Move()
{
	//3D���e�B�N���̍��W���X�N���[�����W�ɕϊ�������
	Vector3 positionReticle = worldPos3d;

	//�r���[�|�[�g�s��
	XMMATRIX matViewport = {
		{1280 / 2, 0, 0, 0 },
		{0, -720 / 2, 0, 0},
		{0, 0, 1, 0},
		{1280 / 2, 720 / 2, 0, 1}
	};

	//�r���[�s��ƃv���W�F�N�V�����s��ƃr���[�|�[�g�s�������
	XMMATRIX matViewProjectionViewport =
		matView * matProjection * matViewport;

	//���[���h���W����X�N���[�����W�ɕϊ�
	positionReticle = MatrixTransformWDivision(positionReticle, matViewProjectionViewport);
	Vector2 screenPosition = { positionReticle.x, positionReticle.y };

	//2D���e�B�N���̍��W��ݒ�
	position = screenPosition;
}
