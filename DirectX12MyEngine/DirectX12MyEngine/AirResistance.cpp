#include "AirResistance.h"
#include "Input.h"


AirResistance* AirResistance::Create(Model* model)
{
	//�C���X�^���X�𐶐�
	AirResistance* instance = new AirResistance();
	if (instance == nullptr) {
		return nullptr;
	}

	//������
	if (!instance->Initialize(model)) {
		delete instance;
		assert(0);
	}

	return instance;
}

AirResistance::~AirResistance()
{
	//�I�u�W�F�N�g���
	delete sphereObject;
}

bool AirResistance::Initialize(Model* model)
{
	//�e�I�u�W�F�N�g����
	sphereObject = Object3d::Create(model);
	if (sphereObject == nullptr) {
		return false;
	}

	//�����ʒu���Z�b�g
	sphereObject->SetPosition(initPos);

	//�������x���Z�b�g
	vel = initVel;

	return true;
}

void AirResistance::Update()
{
	Input* input = Input::GetInstance();

	//�X�y�[�X�L�[�ŏ�����
	if (input->TriggerKey(DIK_SPACE))
	{
		Reset();
	}

	//�C��̓���
	if (!isOnGround)
	{
		Turret();
	}

	//�I�u�W�F�N�g�X�V
	sphereObject->Update();
}

void AirResistance::Draw()
{
	//�I�u�W�F�N�g�`��
	sphereObject->Draw();
}

void AirResistance::Turret()
{
	XMFLOAT3 pos = sphereObject->GetPosition();
	const float gravity = 9.8f / 60;
	const float gPower = gravity * mass;

	//��C��R���v�Z
	float resistance = vel.y;
	float resiAccel = resistance / mass / 60;

	//�d��-��C��R�ŉ����x���Z�o����
	accel.y = -gPower - resiAccel;

	//���x�ɉ����x�����Z
	vel.y += accel.y;

	//���W�ɑ��x�����Z
	pos.x += vel.x;
	pos.y += vel.y;

	//y��0�ȉ��͍s���Ȃ�
	if (pos.y < 0)
	{
		pos.y = 0;

		//�n�ʂɂ���
		isOnGround = true;
	}

	//�X�V�������W���Z�b�g
	sphereObject->SetPosition(pos);
}

void AirResistance::Reset()
{
	//�����ʒu�ɃZ�b�g
	sphereObject->SetPosition(initPos);

	//�������x���Z�b�g
	vel = initVel;

	//�n�ʂɂ��Ă��Ȃ�
	isOnGround = false;
}
