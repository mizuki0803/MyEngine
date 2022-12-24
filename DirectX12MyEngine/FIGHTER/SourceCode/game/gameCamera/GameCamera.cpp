#include "GameCamera.h"
#include "Player.h"

const float GameCamera::advanceSpeed = 0.5f;
const float GameCamera::highSpeedMagnification = 2.8f;
const float GameCamera::slowSpeedMagnification = 0.2f;

void GameCamera::Update()
{
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

void GameCamera::ShakeStart(const float shakePower, const float shakeTime)
{
	//シェイクタイマーをリセット
	shakeTimer = 0;
	//シェイクする時間をセット
	this->shakeTime = shakeTime;
	//シェイク最大の強さをセット
	this->maxShakePower = shakePower;
	//シェイク状態にする
	isShake = true;
}

void GameCamera::CameraAction(Player* player)
{
	//ステージクリア状態
	if (isStageClearMode) {
		//ステージクリア後行動
		StageClear();
	}
	//墜落状態
	else if (isCrash) {
		Crash();
	}
	//それ以外
	else {
		//回転
		Rotate(player->GetRotation());

		//プレイヤーがダメージノックバック状態ならノックバックする
		if (player->GetIsDamageKnockback()) {
			Knockback(player);
		}
		//ダメージ状態でないなら通常移動
		else {
			Move(player);
		}
	}
}

void GameCamera::Rotate(const Vector3& playerRotation)
{
	//回転(レールカメラに追従している自機の傾きを利用する)
	rotation.x = playerRotation.x / 5;
	rotation.y = playerRotation.y / 5;
	rotation.z = -playerRotation.y / 8;
}

void GameCamera::Move(Player* player)
{
	//移動速度
	Vector3 velocity;
	//カメラが傾いている角度に移動させる
	const float moveSpeed = Player::GetMoveBaseSpeed() * 8;
	const Vector2 rotLimit = Player::GetRotLimit();
	velocity.x = moveSpeed * (rotation.y / rotLimit.y);
	velocity.y = moveSpeed * -(rotation.x / rotLimit.x);

	//自機の移動速度変更に合わせて変更する
	float moveSpeedPhaseSpeed = 1.0f;
	if (player->GetMoveSpeedPhase() == Player::MoveSpeedPhase::HighSpeed) { moveSpeedPhaseSpeed = highSpeedMagnification; }
	else if (player->GetMoveSpeedPhase() == Player::MoveSpeedPhase::SlowSpeed) { moveSpeedPhaseSpeed = slowSpeedMagnification; }

	//前進する場合はZ方向に移動
	if (isAdvance) { velocity.z = advanceSpeed * moveSpeedPhaseSpeed; }
	//移動
	position += velocity;

	//移動限界から出ないようにする
	position.x = max(position.x, moveLimitMin.x);
	position.x = min(position.x, moveLimitMax.x);
	position.y = max(position.y, moveLimitMin.y);
	position.y = min(position.y, moveLimitMax.y);
}

void GameCamera::Knockback(Player* player)
{
	//自機にあわせてカメラをノックバックさせる
	const float speed = Player::GetKnockbackBaseSpeed() * 8;
	Vector3 velocity = player->GetKnockbackVel();
	velocity *= speed;

	//自機の移動速度変更に合わせて変更する
	float moveSpeedPhaseSpeed = 1.0f;
	if (player->GetMoveSpeedPhase() == Player::MoveSpeedPhase::HighSpeed) { moveSpeedPhaseSpeed = highSpeedMagnification; }
	else if (player->GetMoveSpeedPhase() == Player::MoveSpeedPhase::SlowSpeed) { moveSpeedPhaseSpeed = slowSpeedMagnification; }

	//前進する場合はZ方向に移動
	const float knockBackSpeed = 0.3f;
	if (isAdvance) { velocity.z = advanceSpeed * moveSpeedPhaseSpeed * knockBackSpeed; }
	//移動
	position += velocity;

	//移動限界から出ないようにする
	position.x = max(position.x, moveLimitMin.x);
	position.x = min(position.x, moveLimitMax.x);
	position.y = max(position.y, moveLimitMin.y);
	position.y = min(position.y, moveLimitMax.y);
}

void GameCamera::Shake()
{
	//タイマーをカウント
	shakeTimer++;
	const float time = shakeTimer / shakeTime;

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
