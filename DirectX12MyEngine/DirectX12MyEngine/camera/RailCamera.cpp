#include "RailCamera.h"
#include "Input.h"
#include "Easing.h"
#include "Player.h"

void RailCamera::Initialize()
{
	//初期座標を設定
	position = { 0, 13, -30 };
	//初期回転角を設定
	rotation = { 0, 0, 0 };

	//ビュー行列と射影行列の初期化
	UpdateMatView();
	UpdateMatProjection();
}

void RailCamera::Update()
{
	//墜落状態でなければ通常レールカメラ
	if (!isCrash) {
		//回転
		Rotate();

		//プレイヤーがダメージ状態ならノックバックする
		if (player->GetIsDamage()) {
			Knockback();
		}
		//ダメージ状態でないなら通常移動
		else {
			Move();
		}
	}
	//墜落状態
	else {
		Crash();
	}

	//回転　平行移動行列の計算
	DirectX::XMMATRIX matRot, matTrans;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//ワールド行列の合成
	matWorld = XMMatrixIdentity();	//変形をリセット
	matWorld *= matRot;		//ワールド行列に回転を反映
	matWorld *= matTrans;	//ワールド行列に平行移動を反映

	//視点をワールド座標に設定
	eye = { matWorld.r[3].m128_f32[0], matWorld.r[3].m128_f32[1], matWorld.r[3].m128_f32[2] };
	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	//カメラの回転を反映させる
	forward = MatrixTransformDirection(forward, matWorld);
	//視点から前方に進んだ位置を注視点に設定
	target = eye + forward;
	//天地が反転してもいいように上方向ベクトルも回転させる
	Vector3 baseUp(0, 1, 0);
	up = MatrixTransformDirection(baseUp, matWorld);

	//シェイク状態ならカメラをシェイクさせる
	if (isShake) {
		Shake();
	}

	//ビュー行列と射影行列の更新
	UpdateMatView();
	UpdateMatProjection();
}

void RailCamera::CrashStart()
{
	//墜落状態にする
	isCrash = true;
	//墜落状態のカメラ位置に移動する
	isMoveCrashPos = true;

	//墜落状態のカメラ位置に移動前の座標をセット
	moveCrashBeforePos = position;
}

void RailCamera::ShakeStart()
{
	//シェイクタイマーをリセット
	shakeTimer = 0;
	//シェイク状態にする
	isShake = true;
}

void RailCamera::Crash()
{
	//自機の少し上にカメラを移動させる
	const Vector3 crashCameraPos = { player->GetWorldPos().x, player->GetWorldPos().y + 3, 0 };
	if (isMoveCrashPos) {
		//タイマーを更新
		const float moveCrashPosTime = 40;
		moveCrashPosTimer++;
		const float time = moveCrashPosTimer / moveCrashPosTime;

		position.x = Easing::OutQuad(moveCrashBeforePos.x, crashCameraPos.x, time);
		position.y = Easing::OutQuad(moveCrashBeforePos.y, crashCameraPos.y, time);

		//タイマーが指定した時間になったら緊急回避終了
		if (moveCrashPosTimer >= moveCrashPosTime) {
			isMoveCrashPos = false;
		}
	}
	//移動終了後も同じ座標を追従し続ける
	else {
		//X,Y座標は自機の少し上を追従する
		position.x = crashCameraPos.x;
		position.y = crashCameraPos.y;
	}

	//Z軸回転する
	const float rotSpeed = 1.5f;
	//自機が一回バウンドするまではZ方向に移動する
	if (player->GetCrashBoundCount() == 0) {
		rotation.z += rotSpeed;
		position.z += 0.1f;
	}
	else if (player->GetCrashBoundCount() == 1) { rotation.z -= rotSpeed; }
}

void RailCamera::Rotate()
{
	//回転(レールカメラに追従している自機の傾きを利用する)
	rotation.x = player->GetRotation().x / 5;
	rotation.y = player->GetRotation().y / 5;
	rotation.z = -player->GetRotation().y / 10;
}

void RailCamera::Move()
{
	//移動速度
	Vector3 velocity;
	//カメラが傾いている角度に移動させる
	const float moveSpeed = 1.2f;
	const Vector2 rotLimit = Player::GetRotLimit();
	velocity.x = moveSpeed * (rotation.y / rotLimit.y);
	velocity.y = moveSpeed * -(rotation.x / rotLimit.x);

	//前進する場合はZ方向に移動
	if (isAdvance) { velocity.z = 0.1f; }
	//移動
	position += velocity;

	//移動限界から出ないようにする
	const Vector2 moveLimitMin = { -15.0f, 8.0f };
	const Vector2 moveLimitMax = { 15.0f, 23.0f };
	position.x = max(position.x, moveLimitMin.x);
	position.x = min(position.x, moveLimitMax.x);
	position.y = max(position.y, moveLimitMin.y);
	position.y = min(position.y, moveLimitMax.y);
}

void RailCamera::Knockback()
{
	//自機にあわせてカメラをノックバックさせる
	const float speed = 1.6f;
	Vector3 velocity = player->GetKnockbackVel();
	velocity *= speed;

	//前進する場合はZ方向に移動
	if (isAdvance) { velocity.z = 0.1f; }
	//移動
	position += velocity;

	//移動限界から出ないようにする
	const Vector2 moveLimitMin = { -15.0f, 8.0f };
	const Vector2 moveLimitMax = { 15.0f, 23.0f };
	position.x = max(position.x, moveLimitMin.x);
	position.x = min(position.x, moveLimitMax.x);
	position.y = max(position.y, moveLimitMin.y);
	position.y = min(position.y, moveLimitMax.y);
}

void RailCamera::Shake()
{
	//シェイクする時間を設定
	const float shakeTime = 20;
	//タイマーをカウント
	shakeTimer++;
	const float time = shakeTimer / shakeTime;
	//シェイクの最大の強さを設定
	const float maxShakePower = 15;

	//シェイクする値を計算
	const float randShake = maxShakePower * (1 - time);
	Vector3 shake{};

	//ゼロ除算を避けるために0の場合はランダムを生成しない
	if (!((int)randShake == 0)) {
		shake.x = (float)((rand() % (int)randShake) - randShake / 2);
		shake.y = (float)((rand() % (int)randShake) - randShake / 2);
	}

	//値が大きいので割り算して小さくする
	const float div = 100;
	shake /= div;

	//カメラにシェイクの値を加算
	eye += shake;

	//シェイクが完了したらシェイク状態を解除
	if (shakeTimer >= shakeTime) {
		isShake = false;
	}
}
