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
	eyeDistance = { 0.0f, 4.0f, -15.0f };
	eye += eyeDistance;
	this->eye = eye;
}

void TitleCamera::Update()
{
	//自機が出撃前
	if (!player->GetIsSortie()) {
		//自機の周りをぐるぐる回転
		RoundPlayer();
	}
	//出撃状態
	else {
		//カメラも出撃用に
		Sortie();
	}

	//カメラ更新
	Camera::Update();
}

void TitleCamera::SortieStart()
{
	//出撃開始一周の開始角度をセット
	beforeRoundRotAngle = rotAngle;
	//1～2周に調整する為1周分の値を加算
	beforeRoundRotAngle += 360;
}

void TitleCamera::RoundPlayer()
{
	//角度を変更し続ける
	const float rotSpeed = 0.5f;
	rotAngle += rotSpeed;

	//360を越えないようにする
	if (rotAngle >= 360) { rotAngle -= 360; }

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
		//タイマー更新
		rotRoundTimer++;
		const float time = rotRoundTimer / rotRoundTime;

		//自機の周りをカメラ一周させる
		rotAngle = Easing::OutQuart(beforeRoundRotAngle, 0, time);
		const float radian = XMConvertToRadians(rotAngle);
		const float distance = eyeDistance.z;
		eye.z = distance * cosf(radian);
		eye.x = distance * sinf(radian);
		eye.y = target.y + eyeDistance.y;
		//自機と同じ高さにカメラを近づける
		const float roundStartDistance = 8.0f;
		const float roundEndDistance = 1.0f;
		eyeDistance.y = Easing::OutQuart(roundStartDistance, roundEndDistance, time);
	}

	//自機を注視点に設定し続ける
	target = player->GetPosition();
	dirtyView = true;
}
