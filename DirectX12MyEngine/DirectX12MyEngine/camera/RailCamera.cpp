#include "RailCamera.h"
#include "Input.h"
#include "Player.h"

void RailCamera::Initialize()
{
	//�������W��ݒ�
	position = { 0, 0, -30 };
	//������]�p��ݒ�
	rotation = { 0, 0, 0 };

	//�r���[�s��Ǝˉe�s��̏�����
	UpdateMatView();
	UpdateMatProjection();
}

void RailCamera::Update()
{
	Input* input = Input::GetInstance();

	//��]
	Rotate();

	//�v���C���[���_���[�W��ԂȂ�m�b�N�o�b�N����
	if (player->GetIsDamage()) {
		Knockback();
	}
	//�_���[�W��ԂłȂ��Ȃ�ʏ�ړ�
	else {
		Move();
	}

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

void RailCamera::ShakeStart()
{
	//�V�F�C�N�^�C�}�[�����Z�b�g
	shakeTimer = 0;
	//�V�F�C�N��Ԃɂ���
	isShake = true;
}

void RailCamera::Rotate()
{
	//��](���[���J�����ɒǏ]���Ă��鎩�@�̌X���𗘗p����)
	rotation.x = player->GetRotation().x / 5;
	rotation.y = player->GetRotation().y / 5;
	rotation.z = -player->GetRotation().y / 10;
}

void RailCamera::Move()
{
	//�ړ����x
	Vector3 velocity;
	//�J�������X���Ă���p�x�Ɉړ�������
	const float moveSpeed = 1.2f;
	const Vector2 rotLimit = Player::GetRotLimit();
	velocity.x = moveSpeed * (rotation.y / rotLimit.y);
	velocity.y = moveSpeed * -(rotation.x / rotLimit.x);

	//�O�i����ꍇ��Z�����Ɉړ�
	if (isAdvance) { velocity.z = 0.1f; }
	//�ړ�
	position += velocity;

	//�ړ����E����o�Ȃ��悤�ɂ���
	const Vector2 moveLimit = { 15.0f, 7.5f };
	position.x = max(position.x, -moveLimit.x);
	position.x = min(position.x, +moveLimit.x);
	position.y = max(position.y, -moveLimit.y);
	position.y = min(position.y, +moveLimit.y);
}

void RailCamera::Knockback()
{
	//���@�ɂ��킹�ăJ�������m�b�N�o�b�N������
	const float speed = 1.6f;
	Vector3 velocity = player->GetKnockbackVel();
	velocity *= speed;

	//�O�i����ꍇ��Z�����Ɉړ�
	if (isAdvance) { velocity.z = 0.1f; }
	//�ړ�
	position += velocity;

	//�ړ����E����o�Ȃ��悤�ɂ���
	const Vector2 moveLimit = { 15.0f, 7.5f };
	position.x = max(position.x, -moveLimit.x);
	position.x = min(position.x, +moveLimit.x);
	position.y = max(position.y, -moveLimit.y);
	position.y = min(position.y, +moveLimit.y);
}

void RailCamera::Shake()
{
	//�V�F�C�N���鎞�Ԃ�ݒ�
	const float shakeTime = 20;
	//�^�C�}�[���J�E���g
	shakeTimer++;
	const float time = shakeTimer / shakeTime;
	//�V�F�C�N�̍ő�̋�����ݒ�
	const float maxShakePower = 15;

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
