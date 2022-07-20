#include "RailCamera.h"
#include "Input.h"

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

	//�ړ�
	Vector3 velocity(0, 0, 0.1f);
	//XY����͂ňړ�������
	float moveSpeed = 0.3f;
	if (input->PushKey(DIK_RIGHT)) { velocity.x += moveSpeed; }
	if (input->PushKey(DIK_LEFT)) { velocity.x -= moveSpeed; }
	if (input->PushKey(DIK_UP)) { velocity.y += moveSpeed; }
	if (input->PushKey(DIK_DOWN)) { velocity.y -= moveSpeed; }

	//�X�e�B�b�N�X���̔�������
	const float stickNum = 500;
	if (input->TiltGamePadLStickX(stickNum) || input->TiltGamePadLStickX(-stickNum) || input->TiltGamePadLStickY(stickNum) || input->TiltGamePadLStickY(-stickNum)) {
		//�v���C���[�̓X�e�B�b�N��|���������ɓ���
		float padRota = input->GetPadLStickAngle();
		float moveAngle = XMConvertToRadians(padRota);
		velocity.x = moveSpeed * cosf(moveAngle);
		velocity.y = moveSpeed * -sinf(moveAngle);
	}

	position += velocity;

	//XY�̈ړ��͈͂�ݒ�
	const XMFLOAT2 moveLimit = { 20.0f, 10.0f };

	position.x = max(position.x, -moveLimit.x);
	position.x = min(position.x, +moveLimit.x);
	position.y = max(position.y, -moveLimit.y);
	position.y = min(position.y, +moveLimit.y);
	//��]
	//Vector3 rot(0.1f, 0, 0);
	//rotation += rot;

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
