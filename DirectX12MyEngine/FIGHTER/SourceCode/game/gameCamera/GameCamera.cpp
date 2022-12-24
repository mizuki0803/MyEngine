#include "GameCamera.h"
#include "Player.h"

const float GameCamera::advanceSpeed = 0.5f;
const float GameCamera::highSpeedMagnification = 2.8f;
const float GameCamera::slowSpeedMagnification = 0.2f;

void GameCamera::Update()
{
	//��]�@���s�ړ��s��̌v�Z
	DirectX::XMMATRIX matRot, matTrans;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//���[���h�s��̍���
	matWorld = XMMatrixIdentity();	//�ό`�����Z�b�g
	matWorld *= matRot;		//���[���h�s��ɉ�]�𔽉f
	matWorld *= matTrans;	//���[���h�s��ɕ��s�ړ��𔽉f

	//���_�����[���h���W�ɐݒ�
	eye = { matWorld.r[3].m128_f32[0], matWorld.r[3].m128_f32[1], matWorld.r[3].m128_f32[2] };
	//���[���h�O���x�N�g��
	Vector3 forward(0, 0, 1);
	//�J�����̉�]�𔽉f������
	forward = MatrixTransformDirection(forward, matWorld);
	//���_����O���ɐi�񂾈ʒu�𒍎��_�ɐݒ�
	target = eye + forward;
	//�V�n�����]���Ă������悤�ɏ�����x�N�g������]������
	Vector3 baseUp(0, 1, 0);
	up = MatrixTransformDirection(baseUp, matWorld);

	//�V�F�C�N��ԂȂ�J�������V�F�C�N������
	if (isShake) {
		Shake();
	}

	//�r���[�s��Ǝˉe�s��̍X�V
	UpdateMatView();
	UpdateMatProjection();
}

void GameCamera::ShakeStart(const float shakePower, const float shakeTime)
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

void GameCamera::CameraAction(Player* player)
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

void GameCamera::Rotate(const Vector3& playerRotation)
{
	//��](���[���J�����ɒǏ]���Ă��鎩�@�̌X���𗘗p����)
	rotation.x = playerRotation.x / 5;
	rotation.y = playerRotation.y / 5;
	rotation.z = -playerRotation.y / 8;
}

void GameCamera::Move(Player* player)
{
	//�ړ����x
	Vector3 velocity;
	//�J�������X���Ă���p�x�Ɉړ�������
	const float moveSpeed = Player::GetMoveBaseSpeed() * 8;
	const Vector2 rotLimit = Player::GetRotLimit();
	velocity.x = moveSpeed * (rotation.y / rotLimit.y);
	velocity.y = moveSpeed * -(rotation.x / rotLimit.x);

	//���@�̈ړ����x�ύX�ɍ��킹�ĕύX����
	float moveSpeedPhaseSpeed = 1.0f;
	if (player->GetMoveSpeedPhase() == Player::MoveSpeedPhase::HighSpeed) { moveSpeedPhaseSpeed = highSpeedMagnification; }
	else if (player->GetMoveSpeedPhase() == Player::MoveSpeedPhase::SlowSpeed) { moveSpeedPhaseSpeed = slowSpeedMagnification; }

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

void GameCamera::Knockback(Player* player)
{
	//���@�ɂ��킹�ăJ�������m�b�N�o�b�N������
	const float speed = Player::GetKnockbackBaseSpeed() * 8;
	Vector3 velocity = player->GetKnockbackVel();
	velocity *= speed;

	//���@�̈ړ����x�ύX�ɍ��킹�ĕύX����
	float moveSpeedPhaseSpeed = 1.0f;
	if (player->GetMoveSpeedPhase() == Player::MoveSpeedPhase::HighSpeed) { moveSpeedPhaseSpeed = highSpeedMagnification; }
	else if (player->GetMoveSpeedPhase() == Player::MoveSpeedPhase::SlowSpeed) { moveSpeedPhaseSpeed = slowSpeedMagnification; }

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

void GameCamera::Shake()
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
