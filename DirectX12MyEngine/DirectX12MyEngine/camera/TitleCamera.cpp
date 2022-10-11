#include "TitleCamera.h"
#include "TitlePlayer.h"
#include "Input.h"
#include "Easing.h"

void TitleCamera::Initialize()
{
	//カメラ初期化
	Camera::Initialize();

	//カメラの注視点を自機にセット
	target = player->GetPosition();

	//カメラの視点を自機から少し離れたところにセット
	Vector3 eye = target;
	eyeDistance = { 0.0f, 8.0f, -15.0f };
	eye += eyeDistance;
	this->eye = eye;
}

void TitleCamera::Update()
{
	if (!player->GetIsSortie()) {

		const float rotSpeed = 0.5f;
		rotAngle += rotSpeed;
		const float radian = XMConvertToRadians(rotAngle);
		const float distance = eyeDistance.z/* + 5 * sinf(radian)*/;

		eye.z = distance * cosf(radian);
		eye.x = distance * sinf(radian);
		dirtyView = true;
	}
	else {
		Sortie();
	}
	Camera::Update();
}

void TitleCamera::SortieStart()
{
	//出撃開始一周の開始角度をセット
	beforeRoundRotAngle = rotAngle;
	//一周分に修正
	beforeRoundRotAngle = (float)((int)beforeRoundRotAngle % 360);
	beforeRoundRotAngle += 360;
}

void TitleCamera::Sortie()
{
	const float rotRoundTime = 120;
	if (rotRoundTimer <= rotRoundTime) {
		//タイマー更新
		rotRoundTimer++;
		const float time = rotRoundTimer / rotRoundTime;

		//自機の周りをカメラ一周させる
		rotAngle = Easing::OutQuart(beforeRoundRotAngle, 0, time);
		eyeDistance.y = Easing::OutQuart(8.0f, 1.0f, time);
		const float radian = XMConvertToRadians(rotAngle);
		const float distance = eyeDistance.z;
		eye.z = distance * cosf(radian);
		eye.x = distance * sinf(radian);
		eye.y = target.y + eyeDistance.y;
	}

	//自機を注視点に設定し続ける
	target = player->GetPosition();
	dirtyView = true;
}
