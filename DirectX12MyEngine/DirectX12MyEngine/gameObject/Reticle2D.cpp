#include "Reticle2D.h"
#include "Easing.h"

const XMFLOAT4 Reticle2D::normalColor = { 0.5f, 0.9f, 0.2f, 1 };
const XMFLOAT4 Reticle2D::lockonColor = { 0.9f, 0.1f, 0.1f, 1 };

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

	size = { 100, 100 };
	color = normalColor;

	return true;
}

void Reticle2D::Update()
{
	//�G�����b�N�I�����Ă���Ƃ�
	if (isLockon) {
		MoveLockon();
	}
	//�G�����b�N�I�����Ă��Ȃ��Ƃ�
	else {
		MoveNotLockon();
	}

	//�X�v���C�g�̍X�V
	Sprite::Update();
}

void Reticle2D::LockonEnemy(Enemy* enemy)
{
	//���b�N�I���Ώۂ̓G���Z�b�g���Ă���
	this->lockonEnemy = enemy;
	//���b�N�I�������̂ŐF��ύX����
	color = lockonColor;
	//���b�N�I����Ԃɐݒ�
	isLockon = true;
}

void Reticle2D::UnlockonEnemy()
{
	//���b�N�I���Ώۂ̓G���������Ă���
	this->lockonEnemy = nullptr;
	//���b�N�I�����������̂ŐF��߂�
	color = normalColor;
	//���b�N�I�����Ă��Ȃ���Ԃɐݒ�
	isLockon = false;
	//���e�B�N�������̈ʒu(���@�Ǐ])�ɖ߂���Ԃɐݒ�
	isBackPos = true;
	//���̈ʒu(���@�Ǐ])�ɖ߂��^�C�}�[��������
	backTimer = 0;
}

void Reticle2D::MoveLockon()
{
	//���b�N�I���Ώۂ̓G�̃X�N���[�����W��Ǐ]����
	position = lockonEnemy->GetScreenPos();

	//���e�B�N������ʊO�ɏo���烍�b�N�I����Ԃ���������
	const bool isOutsideScreen = (position.x > WindowApp::window_width || position.x < 0 || position.y > WindowApp::window_height || position.y < 0);
	if (isOutsideScreen) {
		UnlockonEnemy();
	}
}

void Reticle2D::MoveNotLockon()
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

	//���̈ʒu(���@�Ǐ])�ɖ߂���Ԃ̂Ƃ�
	if (isBackPos) {
		//Lerp���g���Ď��@�Ǐ]�ɖ߂�
		const float maxTime = 60;
		backTimer++;
		float time = backTimer / maxTime;
		position = Easing::Lerp(position, screenPosition, time);

		//Lerp���I���Ǝ��@�Ǐ]�ɖ߂��Ă���
		if (backTimer >= maxTime) {
			isBackPos = false;
		}
	}
	//���@�Ǐ]���Ă���Ƃ�
	else {
		//2D���e�B�N���̍��W��ݒ�
		position = screenPosition;
	}
}
