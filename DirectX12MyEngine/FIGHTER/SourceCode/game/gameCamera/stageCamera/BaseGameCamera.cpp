#include "BaseGameCamera.h"
#include "BasePlayer.h"

const float BaseGameCamera::moveSpeedPlayerMagnification = 8.0f;
const float BaseGameCamera::highSpeedMagnification = 2.8f;
const float BaseGameCamera::slowSpeedMagnification = 0.2f;

void BaseGameCamera::Update()
{
	//���s�ړ��s��̌v�Z
	const XMMATRIX matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//���[���h�s����X�V
	UpdateMatWorld(matTrans);
	//���������Z�������[���h�s����X�V
	UpdateSwayMatWorld(matTrans);

	//���_�A�����_���X�V
	UpdateEyeTarget();

	//�V�F�C�N��ԂȂ�J�������V�F�C�N������
	if (isShake) {
		Shake();
	}

	//�r���[�s��Ǝˉe�s��̍X�V
	UpdateMatView();
	UpdateMatProjection();
}

void BaseGameCamera::ShakeStart(const float shakePower, const float shakeTime)
{
	//�V�F�C�N�^�C�}�[�����Z�b�g
	shakeTimer = 0;
	//�V�F�C�N���鎞�Ԃ��Z�b�g
	this->shakeTime = shakeTime;
	//�V�F�C�N�ő�̋������Z�b�g
	this->maxShakePower = shakePower;
	//�V�F�C�N��Ԃɂ���
	isShake = true;
}

void BaseGameCamera::UpdateMatWorld(const XMMATRIX& matTrans)
{
	//��]�@
	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	//�q�ł��鎩�@�p�̃��[���h�s��̍���
	matWorld = XMMatrixIdentity();	//�ό`�����Z�b�g
	matWorld *= matRot;		//���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans;	//���[���h�s��ɕ��s�ړ��𔽉f
}

void BaseGameCamera::UpdateSwayMatWorld(const XMMATRIX& matTrans)
{
	//���������Z������]�pX
	const float cameraRotX = rotation.x + swayX;

	//��]�@���s�ړ��s��̌v�Z
	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(cameraRotX));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	//���[���h�s��̍���
	cameraMatWorld = XMMatrixIdentity();	//�ό`�����Z�b�g
	cameraMatWorld *= matRot;		//���[���h�s��ɉ�]�𔽉f
	cameraMatWorld *= matTrans;	//���[���h�s��ɕ��s�ړ��𔽉f
}

void BaseGameCamera::UpdateEyeTarget()
{
	//���_�����[���h���W�ɐݒ�
	eye = { cameraMatWorld.r[3].m128_f32[0], cameraMatWorld.r[3].m128_f32[1], cameraMatWorld.r[3].m128_f32[2] };
	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//�J�����̉�]�𔽉f������
	forward = MatrixTransformDirection(forward, cameraMatWorld);
	//���_����O���ɐi�񂾈ʒu�𒍎��_�ɐݒ�
	target = eye + forward;
	//�V�n�����]���Ă������悤�ɏ�����x�N�g������]������
	Vector3 baseUp(0, 1, 0);
	up = MatrixTransformDirection(baseUp, cameraMatWorld);
}

void BaseGameCamera::CameraAction(BasePlayer* player)
{
	//�X�e�[�W�N���A���
	if (isStageClearMode) {
		//�X�e�[�W�N���A��s��
		StageClear();
	}
	//�ė����
	else if (isCrash) {
		Crash();
	}
	//����ȊO
	else {
		//��]
		Rotate(player->GetRotation());
		//�������Ȃ��Ǝ₵���̂ł���炳���Ă���
		Sway();

		//�v���C���[���_���[�W�m�b�N�o�b�N��ԂȂ�m�b�N�o�b�N����
		if (player->GetIsDamageKnockback()) {
			Knockback(player);
		}
		//�_���[�W��ԂłȂ��Ȃ�ʏ�ړ�
		else {
			Move(player);
		}
	}
}

void BaseGameCamera::Rotate(const Vector3& playerRotation)
{
	//��](���[���J�����ɒǏ]���Ă��鎩�@�̌X���𗘗p����)
	const Vector3 PlayerRotDivNum = { 5, 5, 8 }; //�J�����̌X�����Z�o���邽�߂̎��@�̊p�x���犄��l
	rotation.x = playerRotation.x / PlayerRotDivNum.x;
	rotation.y = playerRotation.y / PlayerRotDivNum.y;
	rotation.z = -playerRotation.y / PlayerRotDivNum.z;
}

void BaseGameCamera::Sway()
{
	//�����ő�̑���
	const float rotXMaxSpeed = 0.008f;
	//�����܂�Ԃ�
	const float swayXLimit = 0.2f;
	//�����̉����x
	const float swayXSpeedAccel = 0.0008f;
	//���]
	if (isSwayXUp) {
		//���x���ő�łȂ��Ƃ�
		if (!isSwaySpeedMax) {
			//�����ɉ����x�����Z����
			swayXSpeed += swayXSpeedAccel;

			//�������ő�ɂȂ�����t���O�𗧂Ă�
			if (swayXSpeed >= rotXMaxSpeed) {
				isSwaySpeedMax = true;
			}
		}
		//�����̊p�x���܂�Ԃ��܂ŗ�����
		if (swayX >= swayXLimit) {
			//�����ɉ����x�����Z���Ă���
			swayXSpeed -= swayXSpeedAccel;

			//������0�ɂȂ�����
			if (swayXSpeed <= 0) {
				//����]�ɕύX
				isSwayXUp = false;
				//�����ő�t���O�����낵�Ă���
				isSwaySpeedMax = false;
			}
		}
	}
	//����]
	else {
		//���x���ő�łȂ��Ƃ�
		if (!isSwaySpeedMax) {
			//�����ɉ����x�����Z����
			swayXSpeed -= swayXSpeedAccel;

			//�������ő�ɂȂ�����t���O�𗧂Ă�
			if (swayXSpeed <= -rotXMaxSpeed) {
				isSwaySpeedMax = true;
			}
		}
		//�����̊p�x���܂�Ԃ��܂ŗ�����
		if (swayX <= -swayXLimit) {
			//�����ɉ����x�����Z���Ă���
			swayXSpeed += swayXSpeedAccel;

			//������0�ɂȂ�����
			if (swayXSpeed >= 0) {
				//���]�ɕύX
				isSwayXUp = true;
				//�����ő�t���O�����낵�Ă���
				isSwaySpeedMax = false;
			}
		}
	}

	//�p�x�ɉ��Z���Ă���炳����
	swayX += swayXSpeed;
}

void BaseGameCamera::Move(BasePlayer* player)
{
	//�ړ����x
	Vector3 velocity;
	//�J�������X���Ă���p�x�Ɉړ�������
	const float moveSpeed = BasePlayer::GetMoveBaseSpeed() * moveSpeedPlayerMagnification;
	const Vector2 rotLimit = BasePlayer::GetRotLimit();
	velocity.x = moveSpeed * (rotation.y / rotLimit.y);
	velocity.y = moveSpeed * -(rotation.x / rotLimit.x);

	//���@�̈ړ����x�ύX�ɍ��킹�ĕύX����
	float moveSpeedPhaseSpeed = 1.0f;
	if (player->GetMoveSpeedPhase() == BasePlayer::MoveSpeedPhase::HighSpeed) { moveSpeedPhaseSpeed = highSpeedMagnification; }
	else if (player->GetMoveSpeedPhase() == BasePlayer::MoveSpeedPhase::SlowSpeed) { moveSpeedPhaseSpeed = slowSpeedMagnification; }

	//�O�i����ꍇ��Z�����Ɉړ�
	if (isAdvance) { velocity.z = advanceSpeed * moveSpeedPhaseSpeed; }
	//�ړ�
	position += velocity;

	//�ړ����E����o�Ȃ��悤�ɂ���
	position.x = max(position.x, moveLimitMin.x);
	position.x = min(position.x, moveLimitMax.x);
	position.y = max(position.y, moveLimitMin.y);
	position.y = min(position.y, moveLimitMax.y);
}

void BaseGameCamera::Knockback(BasePlayer* player)
{
	//���@�ɂ��킹�ăJ�������m�b�N�o�b�N������
	const float speed = BasePlayer::GetKnockbackBaseSpeed() * moveSpeedPlayerMagnification;
	Vector3 velocity = player->GetKnockbackVel();
	velocity *= speed;

	//���@�̈ړ����x�ύX�ɍ��킹�ĕύX����
	float moveSpeedPhaseSpeed = 1.0f;
	if (player->GetMoveSpeedPhase() == BasePlayer::MoveSpeedPhase::HighSpeed) { moveSpeedPhaseSpeed = highSpeedMagnification; }
	else if (player->GetMoveSpeedPhase() == BasePlayer::MoveSpeedPhase::SlowSpeed) { moveSpeedPhaseSpeed = slowSpeedMagnification; }

	//�O�i����ꍇ��Z�����Ɉړ�
	const float knockBackSpeed = 0.3f;
	if (isAdvance) { velocity.z = advanceSpeed * moveSpeedPhaseSpeed * knockBackSpeed; }
	//�ړ�
	position += velocity;

	//�ړ����E����o�Ȃ��悤�ɂ���
	position.x = max(position.x, moveLimitMin.x);
	position.x = min(position.x, moveLimitMax.x);
	position.y = max(position.y, moveLimitMin.y);
	position.y = min(position.y, moveLimitMax.y);
}

void BaseGameCamera::Shake()
{
	//�^�C�}�[���J�E���g
	shakeTimer++;
	const float time = shakeTimer / shakeTime;

	//�V�F�C�N����l���v�Z
	const float randShake = maxShakePower * (1 - time);
	Vector3 shake{};

	//�[�����Z������邽�߂�0�̏ꍇ�̓����_���𐶐����Ȃ�
	if (!((int)randShake == 0)) {
		shake.x = (float)((rand() % (int)randShake) - randShake / 2);
		shake.y = (float)((rand() % (int)randShake) - randShake / 2);
	}

	//�l���傫���̂Ŋ���Z���ď���������
	const float div = 100;
	shake /= div;

	//�J�����ɃV�F�C�N�̒l�����Z
	eye += shake;

	//�V�F�C�N������������V�F�C�N��Ԃ�����
	if (shakeTimer >= shakeTime) {
		isShake = false;
	}
}
