#include "PlayerFarReticle2D.h"
#include "Easing.h"

const XMFLOAT4 PlayerFarReticle2D::chargeLockonColor = { 0.9f, 0.1f, 0.1f, 1 };

PlayerFarReticle2D* PlayerFarReticle2D::Create(const Texture& texture, const Vector2& normalSize, const Vector2& chargeModeSize)
{
	//���@�t����2D���e�B�N��(��)�̃C���X�^���X�𐶐�
	PlayerFarReticle2D* playerFarReticle2D = new PlayerFarReticle2D();
	if (playerFarReticle2D == nullptr) {
		return nullptr;
	}

	// ������
	if (!playerFarReticle2D->Initialize(texture, { 0.5f, 0.5f }, false, false)) {
		delete playerFarReticle2D;
		assert(0);
		return nullptr;
	}

	//�傫�����Z�b�g
	playerFarReticle2D->size = normalSize;

	//�ʏ펞�̑傫�����L�^���Ă���
	playerFarReticle2D->normalSize = normalSize;

	//�`���[�W���̑傫�����L�^���Ă���
	playerFarReticle2D->chargeModeSize = chargeModeSize;

	return playerFarReticle2D;
}

void PlayerFarReticle2D::Update()
{
	//�`���[�W��Ԃ̋���
	ChargeMode();

	//�G�����b�N�I�����Ă���Ƃ��̈ړ�
	if (isLockon) {
		MoveLockon();
	}
	//�G�����b�N�I�����Ă��Ȃ��Ƃ��̈ړ�
	else {
		Move();
	}

	//�X�v���C�g�̍X�V
	Sprite::Update();
}

void PlayerFarReticle2D::ChargeModeStart()
{
	//�ύX�O�̑傫�����L�^���Ă���
	beforeChangeSize = size;
	//�F���`���[�W���(���b�N�I�����)�ɕύX
	color = chargeLockonColor;
	//�`���[�W��Ԃɂ���
	isChargeMode = true;
	//�`���[�W��ԊJ�n��Ԃɂ���
	isChargeModeStart = true;
	//�`���[�W��ԏI����Ԃ��I�������Ă���
	isChargeModeEnd = false;
	//�`���[�W��ԗp�^�C�}�[��������
	chargeTimer = 0;
}

void PlayerFarReticle2D::ChargeModeEnd()
{
	//�ύX�O�̑傫�����L�^���Ă���
	beforeChangeSize = size;
	//�F��ʏ펞�̐F�ɖ߂�
	color = normalColor;
	//�`���[�W��Ԃ��I��
	isChargeMode = false;
	//�`���[�W��ԊJ�n��Ԃ��I�������Ă���
	isChargeModeStart = false;
	//�`���[�W��ԏI����Ԃɂ���
	isChargeModeEnd = true;
	//�`���[�W��ԗp�^�C�}�[��������
	chargeTimer = 0;	
}

void PlayerFarReticle2D::LockonEnemy(Enemy* enemy)
{
	//�傫�������ɖ߂�
	size = normalSize;
	//���b�N�I���Ώۂ̓G���Z�b�g���Ă���
	lockonEnemy = enemy;
	//���b�N�I�������̂ŐF��ύX����
	color = chargeLockonColor;
	//���b�N�I����Ԃɐݒ�
	isLockon = true;
}

void PlayerFarReticle2D::UnlockonEnemy()
{
	//���b�N�I���Ώۂ̓G���������Ă���
	lockonEnemy = nullptr;
	//���b�N�I�����Ă��Ȃ���Ԃɐݒ�
	isLockon = false;
	//���e�B�N�������̈ʒu(���@�Ǐ])�ɖ߂���Ԃɐݒ�
	isBackPos = true;
	//���̈ʒu(���@�Ǐ])�ɖ߂��^�C�}�[��������
	backTimer = 0;
	//���b�N�I�����������̂ŐF��ʏ�F�ɖ߂�
	color = normalColor;
}

void PlayerFarReticle2D::ChargeMode()
{
	if (isChargeModeStart) {
		ChargeModeStartSizeChange();
	}
	else if (isChargeModeEnd) {
		ChargeModeEndSizeChange();
	}
	else  if (isChargeMode) {
		ChargeModeSizeChange();
	}
}

void PlayerFarReticle2D::ChargeModeStartSizeChange()
{
	//�^�C�}�[���X�V
	const float chargeModeEndTime = 10;
	chargeTimer++;
	const float time = chargeTimer / chargeModeEndTime;

	//�傫����傫������
	size.x = Easing::OutQuad(beforeChangeSize.x, chargeModeSize.x, time);
	size.y = Easing::OutQuad(beforeChangeSize.y, chargeModeSize.y, time);

	//�^�C�}�[���w�肵�����ԂɂȂ�����ً}����I��
	if (chargeTimer >= chargeModeEndTime) {
		//�`���[�W��ԊJ�n��Ԃ��I��
		isChargeModeStart = false;
	}
}

void PlayerFarReticle2D::ChargeModeEndSizeChange()
{
	//�^�C�}�[���X�V
	const float chargeModeEndTime = 10;
	chargeTimer++;
	const float time = chargeTimer / chargeModeEndTime;

	//�傫�������ɖ߂�
	size.x = Easing::OutQuad(beforeChangeSize.x, normalSize.x, time);
	size.y = Easing::OutQuad(beforeChangeSize.y, normalSize.y, time);

	//�^�C�}�[���w�肵�����ԂɂȂ�����ً}����I��
	if (chargeTimer >= chargeModeEndTime) {
		//�`���[�W��ԏI����Ԃ��I��
		isChargeModeEnd = false;
	}
}

void PlayerFarReticle2D::ChargeModeSizeChange()
{
	//�`���[�W��Ԏ��̑傫�����������ύX���Ă�������
	Vector2 sizeChangeSpeed = { 1, 1 };
	size -= sizeChangeSpeed;

	//��胉�C���܂ŏ������Ȃ�����`���[�W���̑傫���ɖ߂�
	const float chargeModeSizeMin = 80.0f;
	if (size.x <= chargeModeSizeMin) {
		size = chargeModeSize;
	}
}

void PlayerFarReticle2D::MoveLockon()
{
	//���b�N�I���Ώۂ̓G�̃X�N���[�����W��Ǐ]����
	position = lockonEnemy->GetScreenPos();

	//���e�B�N������ʊO�ɏo���烍�b�N�I����Ԃ���������
	const bool isOutsideScreen = (position.x > WindowApp::window_width || position.x < 0 || position.y > WindowApp::window_height || position.y < 0);
	if (isOutsideScreen) {
		UnlockonEnemy();
	}
}

void PlayerFarReticle2D::Move()
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
		position = Easing::LerpVec2(position, screenPosition, time);

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
