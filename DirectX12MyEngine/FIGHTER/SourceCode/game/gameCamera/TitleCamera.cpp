#include "TitleCamera.h"
#include "TitlePlayer.h"
#include "Input.h"
#include "Easing.h"

void TitleCamera::Initialize()
{
	//�J����������
	Camera::Initialize();

	//�J�����̒����_�����@�ɃZ�b�g
	target = player->GetPosition();

	//�J�����̎��_�����@���班�����ꂽ�Ƃ���ɃZ�b�g
	Vector3 eye = target;
	eyeDistance = { 0.0f, 4.0f, -15.0f };
	eye += eyeDistance;
	this->eye = eye;
}

void TitleCamera::Update()
{
	//���@���o���O
	if (!player->GetIsSortie()) {
		//���@�̎�������邮���]
		RoundPlayer();
	}
	//�o�����
	else {
		//�J�������o���p��
		Sortie();
	}

	//�J�����X�V
	Camera::Update();
}

void TitleCamera::SortieStart()
{
	//�o���J�n����̊J�n�p�x���Z�b�g
	beforeRoundRotAngle = rotAngle;
	//������ɏC��
	beforeRoundRotAngle = (float)((int)beforeRoundRotAngle % 360);
	beforeRoundRotAngle += 360;
}

void TitleCamera::RoundPlayer()
{
	//�p�x��ύX��������
	const float rotSpeed = 0.5f;
	rotAngle += rotSpeed;
	const float radian = XMConvertToRadians(rotAngle);
	const float distance = eyeDistance.z;
	eye.z = distance * cosf(radian);
	eye.x = distance * sinf(radian);
	dirtyView = true;
}

void TitleCamera::Sortie()
{
	const float rotRoundTime = 120;
	if (rotRoundTimer <= rotRoundTime) {
		//�^�C�}�[�X�V
		rotRoundTimer++;
		const float time = rotRoundTimer / rotRoundTime;

		//���@�̎�����J�������������
		rotAngle = Easing::OutQuart(beforeRoundRotAngle, 0, time);
		eyeDistance.y = Easing::OutQuart(8.0f, 1.0f, time);
		const float radian = XMConvertToRadians(rotAngle);
		const float distance = eyeDistance.z;
		eye.z = distance * cosf(radian);
		eye.x = distance * sinf(radian);
		eye.y = target.y + eyeDistance.y;
	}

	//���@�𒍎��_�ɐݒ肵������
	target = player->GetPosition();
	dirtyView = true;
}
