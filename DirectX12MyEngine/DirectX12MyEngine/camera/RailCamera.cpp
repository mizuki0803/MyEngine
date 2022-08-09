#include "RailCamera.h"
#include "Input.h"
#include "Player.h"

void RailCamera::Initialize()
{
	//初期座標を設定
	position = { 0, 0, -30 };
	//初期回転角を設定
	rotation = { 0, 0, 0 };

	//ビュー行列と射影行列の初期化
	UpdateMatView();
	UpdateMatProjection();
}

void RailCamera::Update()
{
	Input* input = Input::GetInstance();

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

void RailCamera::ShakeStart()
{
	//シェイクタイマーをリセット
	shakeTimer = 0;
	//シェイク状態にする
	isShake = true;
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
	const Vector2 moveLimit = { 15.0f, 7.5f };
	position.x = max(position.x, -moveLimit.x);
	position.x = min(position.x, +moveLimit.x);
	position.y = max(position.y, -moveLimit.y);
	position.y = min(position.y, +moveLimit.y);
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
	const Vector2 moveLimit = { 15.0f, 7.5f };
	position.x = max(position.x, -moveLimit.x);
	position.x = min(position.x, +moveLimit.x);
	position.y = max(position.y, -moveLimit.y);
	position.y = min(position.y, +moveLimit.y);
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
