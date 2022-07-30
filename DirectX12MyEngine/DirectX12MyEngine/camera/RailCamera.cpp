#include "RailCamera.h"
#include "Input.h"
#include "Player.h"

Player* RailCamera::player = nullptr;

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

	//�ړ�
	Move();		

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

	//�r���[�s��Ǝˉe�s��̍X�V
	UpdateMatView();
	UpdateMatProjection();
}

void RailCamera::Rotate()
{
	//��](���[���J�����ɒǏ]���Ă��鎩�@�̌X���𗘗p����)
	rotation.x = player->GetRotation().x / 5;
	rotation.y = player->GetRotation().y / 5;
	rotation.z = player->GetRotation().y / 10;
}

void RailCamera::Move()
{
	//�ړ����x
	Vector3 velocity(0, 0, 0.1f);
	//�J�������X���Ă���p�x�Ɉړ�������
	const float moveSpeed = 0.8f;
	const Vector2 rotLimit = Player::GetRotLimit();
	velocity.x = moveSpeed * (rotation.y / rotLimit.y);
	velocity.y = moveSpeed * -(rotation.x / rotLimit.x);
	position += velocity;

	//�ړ����E����o�Ȃ��悤�ɂ���
	const Vector2 moveLimit = { 10.0f, 5.0f };
	position.x = max(position.x, -moveLimit.x);
	position.x = min(position.x, +moveLimit.x);
	position.y = max(position.y, -moveLimit.y);
	position.y = min(position.y, +moveLimit.y);
}
