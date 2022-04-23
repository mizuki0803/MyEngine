#include "FrictionSphere.h"
#include "Input.h"


FrictionSphere* FrictionSphere::Create(Model* model)
{
	//�C���X�^���X�𐶐�
	FrictionSphere* instance = new FrictionSphere();
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

FrictionSphere::~FrictionSphere()
{
	//�I�u�W�F�N�g���
	delete sphereObject;
}

bool FrictionSphere::Initialize(Model* model)
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

void FrictionSphere::Update()
{
	Input* input = Input::GetInstance();

	//�X�y�[�X�L�[�ŏ�����
	if (input->TriggerKey(DIK_SPACE))
	{
		Reset();
	}

	//���C��������
	Friction();

	//�I�u�W�F�N�g�X�V
	sphereObject->Update();
}

void FrictionSphere::Draw()
{
	//�I�u�W�F�N�g�`��
	sphereObject->Draw();
}

void FrictionSphere::Friction()
{
	const float gravity = 9.8f / 60;	//�d��
	const float n = mass * gravity;		//�����R��
	const float frictionForce = frictionCoefficient * n; //�����C��
	const float accel = frictionForce / mass; //�����x

	//���x�ɉ����x�����Z
	vel += -accel;

	//�E�������x��0�ȉ��ɂȂ�Ȃ�
	if (vel <= 0) 
	{
		vel = 0;
	}

	//�X�V�������W���Z�b�g
	XMFLOAT3 pos = sphereObject->GetPosition();
	pos.x += vel;
	sphereObject->SetPosition(pos);
}

void FrictionSphere::Reset()
{
	//�����ʒu�ɃZ�b�g
	sphereObject->SetPosition(initPos);

	//�������x���Z�b�g
	vel = initVel;
}
