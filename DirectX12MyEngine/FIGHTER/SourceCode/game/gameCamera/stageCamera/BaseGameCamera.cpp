#include "BaseGameCamera.h"
#include "BasePlayer.h"

const float BaseGameCamera::moveSpeedPlayerMagnification = 8.0f;
const float BaseGameCamera::highSpeedMagnification = 2.8f;
const float BaseGameCamera::slowSpeedMagnification = 0.2f;

void BaseGameCamera::Update()
{
	//平行移動行列の計算
	const XMMATRIX matTrans = XMMatrixTranslation(position.x, position.y, position.z);
	//ワールド行列を更新
	UpdateMatWorld(matTrans);
	//ゆらゆらを加算したワールド行列を更新
	UpdateSwayMatWorld(matTrans);

	//視点、注視点を更新
	UpdateEyeTarget();

	//シェイク状態ならカメラをシェイクさせる
	if (isShake) {
		Shake();
	}

	//ビュー行列と射影行列の更新
	UpdateMatView();
	UpdateMatProjection();
}

void BaseGameCamera::ShakeStart(const float shakePower, const float shakeTime)
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

void BaseGameCamera::UpdateMatWorld(const XMMATRIX& matTrans)
{
	//回転　
	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(rotation.x));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	//子である自機用のワールド行列の合成
	matWorld = XMMatrixIdentity();	//変形をリセット
	matWorld *= matRot;		//ワールド行列に回転を反映
	matWorld *= matTrans;	//ワールド行列に平行移動を反映
}

void BaseGameCamera::UpdateSwayMatWorld(const XMMATRIX& matTrans)
{
	//ゆらゆらを加算した回転角X
	const float cameraRotX = rotation.x + swayX;

	//回転　平行移動行列の計算
	XMMATRIX matRot;
	matRot = XMMatrixIdentity();
	matRot *= XMMatrixRotationZ(XMConvertToRadians(rotation.z));
	matRot *= XMMatrixRotationX(XMConvertToRadians(cameraRotX));
	matRot *= XMMatrixRotationY(XMConvertToRadians(rotation.y));
	//ワールド行列の合成
	cameraMatWorld = XMMatrixIdentity();	//変形をリセット
	cameraMatWorld *= matRot;		//ワールド行列に回転を反映
	cameraMatWorld *= matTrans;	//ワールド行列に平行移動を反映
}

void BaseGameCamera::UpdateEyeTarget()
{
	//視点をワールド座標に設定
	eye = { cameraMatWorld.r[3].m128_f32[0], cameraMatWorld.r[3].m128_f32[1], cameraMatWorld.r[3].m128_f32[2] };
	//ワールド前方ベクトル
	Vector3 forward(0, 0, 1);
	//カメラの回転を反映させる
	forward = MatrixTransformDirection(forward, cameraMatWorld);
	//視点から前方に進んだ位置を注視点に設定
	target = eye + forward;
	//天地が反転してもいいように上方向ベクトルも回転させる
	Vector3 baseUp(0, 1, 0);
	up = MatrixTransformDirection(baseUp, cameraMatWorld);
}

void BaseGameCamera::CameraAction(BasePlayer* player)
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
		//動きがないと寂しいのでゆらゆらさせておく
		Sway();

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

void BaseGameCamera::Rotate(const Vector3& playerRotation)
{
	//回転(レールカメラに追従している自機の傾きを利用する)
	const Vector3 PlayerRotDivNum = { 5, 5, 8 }; //カメラの傾きを算出するための自機の角度から割る値
	rotation.x = playerRotation.x / PlayerRotDivNum.x;
	rotation.y = playerRotation.y / PlayerRotDivNum.y;
	rotation.z = -playerRotation.y / PlayerRotDivNum.z;
}

void BaseGameCamera::Sway()
{
	//ゆらゆら最大の速さ
	const float rotXMaxSpeed = 0.008f;
	//ゆらゆら折り返し
	const float swayXLimit = 0.2f;
	//ゆらゆらの加速度
	const float swayXSpeedAccel = 0.0008f;
	//上回転
	if (isSwayXUp) {
		//速度が最大でないとき
		if (!isSwaySpeedMax) {
			//速さに加速度を加算する
			swayXSpeed += swayXSpeedAccel;

			//速さが最大になったらフラグを立てる
			if (swayXSpeed >= rotXMaxSpeed) {
				isSwaySpeedMax = true;
			}
		}
		//ゆらゆらの角度が折り返しまで来たら
		if (swayX >= swayXLimit) {
			//速さに加速度を減算していく
			swayXSpeed -= swayXSpeedAccel;

			//速さが0になったら
			if (swayXSpeed <= 0) {
				//下回転に変更
				isSwayXUp = false;
				//速さ最大フラグを下ろしておく
				isSwaySpeedMax = false;
			}
		}
	}
	//下回転
	else {
		//速度が最大でないとき
		if (!isSwaySpeedMax) {
			//速さに加速度を減算する
			swayXSpeed -= swayXSpeedAccel;

			//速さが最大になったらフラグを立てる
			if (swayXSpeed <= -rotXMaxSpeed) {
				isSwaySpeedMax = true;
			}
		}
		//ゆらゆらの角度が折り返しまで来たら
		if (swayX <= -swayXLimit) {
			//速さに加速度を加算していく
			swayXSpeed += swayXSpeedAccel;

			//速さが0になったら
			if (swayXSpeed >= 0) {
				//上回転に変更
				isSwayXUp = true;
				//速さ最大フラグを下ろしておく
				isSwaySpeedMax = false;
			}
		}
	}

	//角度に加算してゆらゆらさせる
	swayX += swayXSpeed;
}

void BaseGameCamera::Move(BasePlayer* player)
{
	//移動速度
	Vector3 velocity;
	//カメラが傾いている角度に移動させる
	const float moveSpeed = BasePlayer::GetMoveBaseSpeed() * moveSpeedPlayerMagnification;
	const Vector2 rotLimit = BasePlayer::GetRotLimit();
	velocity.x = moveSpeed * (rotation.y / rotLimit.y);
	velocity.y = moveSpeed * -(rotation.x / rotLimit.x);

	//自機の移動速度変更に合わせて変更する
	float moveSpeedPhaseSpeed = 1.0f;
	if (player->GetMoveSpeedPhase() == BasePlayer::MoveSpeedPhase::HighSpeed) { moveSpeedPhaseSpeed = highSpeedMagnification; }
	else if (player->GetMoveSpeedPhase() == BasePlayer::MoveSpeedPhase::SlowSpeed) { moveSpeedPhaseSpeed = slowSpeedMagnification; }

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

void BaseGameCamera::Knockback(BasePlayer* player)
{
	//自機にあわせてカメラをノックバックさせる
	const float speed = BasePlayer::GetKnockbackBaseSpeed() * moveSpeedPlayerMagnification;
	Vector3 velocity = player->GetKnockbackVel();
	velocity *= speed;

	//自機の移動速度変更に合わせて変更する
	float moveSpeedPhaseSpeed = 1.0f;
	if (player->GetMoveSpeedPhase() == BasePlayer::MoveSpeedPhase::HighSpeed) { moveSpeedPhaseSpeed = highSpeedMagnification; }
	else if (player->GetMoveSpeedPhase() == BasePlayer::MoveSpeedPhase::SlowSpeed) { moveSpeedPhaseSpeed = slowSpeedMagnification; }

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

void BaseGameCamera::Shake()
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
