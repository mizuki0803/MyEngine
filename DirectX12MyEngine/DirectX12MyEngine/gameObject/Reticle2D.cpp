#include "Reticle2D.h"

Reticle2D* Reticle2D::Create(UINT texNumber, const Vector2& size)
{
	//2D���e�B�N���̃C���X�^���X�𐶐�
	Reticle2D* reticle2d = new Reticle2D();
	if (reticle2d == nullptr) {
		return nullptr;
	}

	// ������
	if (!reticle2d->Initialize(texNumber, { 0.5f, 0.5f }, false, false)) {
		delete reticle2d;
		assert(0);
		return nullptr;
	}

	//�傫�����Z�b�g
	reticle2d->size = size;

	return reticle2d;
}

bool Reticle2D::Initialize(UINT texNumber, Vector2 anchorpoint, bool isFlipX, bool isFlipY)
{
	if (!Sprite::Initialize(texNumber, anchorpoint, isFlipX, isFlipY)) {
		return false;
	}

	size = {100, 100};

	return true;
}

void Reticle2D::Update()
{
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

	//2D���e�B�N���̍��W��ݒ�
	position = { positionReticle.x, positionReticle.y };

	Sprite::Update();
}
