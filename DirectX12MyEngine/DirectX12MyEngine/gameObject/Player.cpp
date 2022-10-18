#include "Player.h"
#include "Input.h"
#include "Easing.h"
#include "GameScene.h"
#include "SpriteTexture.h"
#include "StraightBullet.h"
#include "HomingBullet.h"
#include "ParticleEmitter.h"

void (Player::* Player::stageClearActionFuncTable[])() = {
	&Player::StageClearSideMove,
	&Player::StageClearReturn,
	&Player::StageClearUp,
	&Player::StageClearStay,
	&Player::StageClearBoost,
};

GameScene* Player::gameScene = nullptr;
ObjModel* Player::bulletModel = nullptr;
const float Player::homingBulletSize = 2.0f;
const Vector2 Player::rotLimit = { 35.0f, 25.0f };

Player* Player::Create(ObjModel* model)
{
	//自機のインスタンスを生成
	Player* player = new Player();
	if (player == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	player->model = model;

	// 初期化
	if (!player->Initialize()) {
		delete player;
		assert(0);
		return nullptr;
	}

	return player;
}

bool Player::Initialize()
{
	position = { 0 ,1 ,15 };
	scale = { 1.5f, 1.5f, 1.5f };

	//3Dオブジェクトの初期化
	if (!ObjObject3d::Initialize()) {
		return false;
	}

	//レティクルを生成
	reticles.reset(PlayerReticles::Create());

	//HPバー生成
	const Vector2 hpBarPosition = { 20, 20 };
	hpBar.reset(PlayerHPBar::Create(SpriteTexture::PlayerHPGaugeIn, hpBarPosition, maxHP));
	//HPバーフレーム生成
	const float posDiff = 3.0f;	//HPバーの座標との差分
	const Vector2 hpFramePosition = { hpBarPosition.x - posDiff, hpBarPosition.y - posDiff };
	hpFrame.reset(PlayerHPFrame::Create(SpriteTexture::PlayerHPGaugeOut, hpFramePosition));

	return true;
}

void Player::Update()
{
	//死亡状態なら抜ける
	if (isDead) { return; }

	//行動
	Action();

	//オブジェクト更新
	ObjObject3d::Update();

	//レティクル更新
	reticles->Update(matWorld, camera->GetMatView(), camera->GetMatProjection());

	//HPバー更新
	hpBar->Update();
	//HPバーフレーム更新
	hpFrame->Update();

	//自機のジェット噴射演出用パーティクル生成
	ParticleEmitter::GetInstance()->PlayerJet(matWorld);
}

void Player::Draw()
{
	//死亡状態なら抜ける
	if (isDead) { return; }

	//オブジェクト描画
	ObjObject3d::Draw();
}

void Player::DrawUI()
{
	//墜落状態なら抜ける
	if (isCrash) { return; }
	//ステージクリア状態なら抜ける
	if (isStageClearMode) { return; }

	//レティクル描画
	reticles->Draw();

	//HPバーフレーム描画
	hpFrame->Draw();
	//HPバー描画
	hpBar->Draw();
}

void Player::OnCollisionDamage(const Vector3& subjectPos)
{
	//ダメージを喰らう
	Damage();

	//ノックバック情報をセット
	SetKnockback(subjectPos);

	//ダメージを喰らったのでHPバーの長さを変更する
	hpBar->SetChangeLength(HP);
}

void Player::OnCollisionHeal()
{
	//回復
	Heal();

	//ダメージを喰らったのでHPバーの長さを変更する
	hpBar->SetChangeLength(HP);
}

void Player::StageClearModeStart()
{
	//カメラ追従を解除
	SetIsCameraFollow(false);
	//カメラ追従解除により、ローカル座標にワールド座標を代入
	position = GetWorldPos();

	//ステージクリア移動を右か左か決める
	if (position.x < 0) { isStageClearMoveRight = false; }
	else { isStageClearMoveRight = true; }

	//初期速度をセット
	stageClearMoveVelocity = { 0.2f, 0, 0.2f };
	//左側移動時のためマイナスを渡す
	if (!isStageClearMoveRight) {
		stageClearMoveVelocity.x = -stageClearMoveVelocity.x;
	}
	//回転速度をセット
	stageClearRota = rotation;

	//ステージクリア後の動きをする
	isStageClearMode = true;
}

void Player::StageClearReturnStart(const Vector3& cameraPos)
{
	//カメラ座標を取得
	stageClearCameraPos = cameraPos;

	//カメラホーミング用座標をセット
	cameraHomingPos = cameraPos;
	cameraHomingPos.y += 2.0f;

	//初期速度をセット
	stageClearMoveVelocity = { 0, 0, 2.0f };
	//帰還状態にする
	stageClearModePhase = StageClearModePhase::Return;

	//タイマー初期化
	stageClearModeTimer = 0;
}

void Player::StageClearBoostStart()
{
	//ブースト状態にする
	stageClearModePhase = StageClearModePhase::Boost;
}

Vector3 Player::GetWorldPos()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//平行移動成分を取得
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
}

void Player::Action()
{
	//ステージクリア状態
	if (isStageClearMode) {
		//ステージクリア後行動
		(this->*stageClearActionFuncTable[static_cast<size_t>(stageClearModePhase)])();
	}
	//墜落状態
	else if (isCrash) {
		Crash();
	}
	//それ以外
	else {
		//回転
		Rotate();

		//緊急回避
		Roll();

		//ダメージ状態ならノックバックする
		if (isDamage) {
			Knockback();
		}
		//ダメージ状態でないなら通常移動
		else {
			Move();
		}

		//攻撃
		Attack();
	}
}

void Player::Damage()
{
	//体力を減らす
	HP -= 10;

	//HPが0以下になったら墜落させる
	if (HP <= 0) {
		CrashStart();

		//HPは0以下にならない
		HP = 0;
	}

	color = { 1,0,0,1 };

	//ダメージ状態にする
	isDamage = true;
}

void Player::CrashStart()
{
	//カメラ追従を解除
	SetIsCameraFollow(false);
	//カメラ追従解除により、ローカル座標にワールド座標を代入
	position = GetWorldPos();

	//墜落状態にする
	isCrash = true;
}

void Player::Crash()
{
	//座標移動
	//墜落加速度
	Vector3 crashAccel = { 0, -0.0075f, 0 };
	crashVel += crashAccel;
	position += crashVel;

	//Y座標が0以下になったら
	if (position.y <= 0) {
		//座標は0以下にならない
		position.y = 0;

		//墜落バウンド回数を増加させる
		crashBoundCount++;

		//バウンドさせる
		const float boundStartVel = 0.4f;
		crashVel.y = boundStartVel;
	}

	//墜落回転する
	const Vector3 rotSpeed = { 0, 1, 5 };
	//バウンドするまではZ軸左回転
	if (crashBoundCount == 0) { rotation.z += rotSpeed.z; }
	//1回バウンドしたらZ軸右回転 & Y軸回転
	else if (crashBoundCount == 1) {
		rotation -= rotSpeed;
	}

	//墜落バウンド回数が2回に達したら死亡
	const int maxCrashCount = 2;
	if (crashBoundCount >= maxCrashCount) {
		isDead = true;
	}
}

void Player::Heal()
{
	//体力を増やす
	const int healNum = 10;
	HP += healNum;

	//HPは最大HP以上にならない
	if (HP >= maxHP) {
		HP = maxHP;
	}
}

void Player::Rotate()
{
	Input* input = Input::GetInstance();

	//回転速度
	const float rotSpeed = 1.0f;
	//角度修正速度
	const float backSpeed = rotSpeed / 1.5f;
	Vector3 rot = { 0, 0, 0 };


	//どこまで傾けたら判定をとるか
	const float stickNum = 200;
	//y軸回転
	{
		//キー入力で回転させる
		if (input->PushKey(DIK_A) || input->PushKey(DIK_D)) {
			if (input->PushKey(DIK_A)) { rot.y -= rotSpeed; }
			if (input->PushKey(DIK_D)) { rot.y += rotSpeed; }
		}

		//パッドスティックX軸の判定を取る
		else if (input->TiltGamePadLStickX(stickNum) || input->TiltGamePadLStickX(-stickNum)) {
			//自機はスティックを倒した方向に動く
			const float padRota = input->GetPadLStickAngle();
			const float moveAngle = XMConvertToRadians(padRota);
			const float padStickIncline = input->GetPadLStickIncline().x;
			rot.y = rotSpeed * cosf(moveAngle) * fabsf(padStickIncline);
		}

		//キー入力なし&スティックを倒していない場合
		else {
			//y軸回転の傾きを修正する
			if (rotation.y > 1.0f) {
				rot.y -= backSpeed;
			}
			else if (rotation.y < -1.0f) {
				rot.y += backSpeed;
			}
			else {
				rotation.y = 0;
			}
		}
	}
	//x軸回転
	{
		//キー入力で回転させる
		if (input->PushKey(DIK_W) || input->PushKey(DIK_S)) {
			if (input->PushKey(DIK_W)) { rot.x -= rotSpeed; }
			if (input->PushKey(DIK_S)) { rot.x += rotSpeed; }
		}

		//パッドスティックY軸の判定を取る
		else if (input->TiltGamePadLStickY(stickNum) || input->TiltGamePadLStickY(-stickNum)) {
			//自機はスティックを倒した方向に動く
			const float padRota = input->GetPadLStickAngle();
			const float moveAngle = XMConvertToRadians(padRota);
			const float padStickIncline = input->GetPadLStickIncline().y;
			rot.x = rotSpeed * sinf(moveAngle) * fabsf(padStickIncline);
		}
		//キー入力なし&スティックを倒していない場合
		else {
			//x軸回転の傾きを修正する
			if (rotation.x > 1.0f) {
				rot.x -= backSpeed;
			}
			else if (rotation.x < -1.0f) {
				rot.x += backSpeed;
			}
			else {
				rotation.x = 0;
			}
		}
	}
	//z軸回転
	{
		//動きがないと寂しいのでゆらゆらさせておく
		const float rotZSpeed = 0.03f;
		const float rotZLimit = 0.8f;
		//右回転
		if (isRotZRight) {
			swayZ += rotZSpeed;
			if (swayZ >= rotZLimit) {
				isRotZRight = false;
			}
		}
		//左回転
		else {
			swayZ -= rotZSpeed;
			if (swayZ <= -rotZLimit) {
				isRotZRight = true;
			}
		}

		rotation.z = -rotation.y + swayZ;
	}

	//回転の更新
	rotation += rot;

	//角度の限界値からはみ出さない
	rotation.y = max(rotation.y, -rotLimit.y);
	rotation.y = min(rotation.y, +rotLimit.y);
	rotation.x = max(rotation.x, -rotLimit.x);
	//地面に自機がめり込まむワールドY座標
	const float groundPos = 8.0f;
	//地面にめり込まないようにする場合の角度限界値処理
	if (GetWorldPos().y <= groundPos) {
		//ワールドY座標の限界値
		const float moveLimitWorldY = 4.0f;
		//地面から角度制限開始座標までの割合を計算
		const float ratio = (GetWorldPos().y - moveLimitWorldY) / (groundPos - moveLimitWorldY);

		//割合値が0以上の場合
		if (ratio >= 0) {
			//地面に近づくほど傾かないようにする(通常の角度制限値 * 地面に近い割合)
			const float groundRotLimit = rotLimit.x * ratio;
			rotation.x = min(rotation.x, groundRotLimit);
		}
		//0未満の場合
		else {
			//下方向に傾かないようにする
			rotation.x = min(rotation.x, 0);
		}
	}
	//地面にめり込むワールドY座標と関係ない場所にいる場合は通常通りの角度制限を行う
	else {
		rotation.x = min(rotation.x, +rotLimit.x);
	}
}

void Player::Move()
{
	//自機が傾いている角度に移動させる
	Vector3 velocity = { 0, 0, 0 };
	const float moveSpeed = 0.1f;
	velocity.x = moveSpeed * (rotation.y / rotLimit.y);
	velocity.y = moveSpeed * -(rotation.x / rotLimit.x);
	position += velocity;

	//移動限界から出ないようにする
	const Vector2 moveLimitMax = { 10.0f, 6.0f };
	const Vector2 moveLimitMin = { -10.0f, -4.0f };
	position.x = max(position.x, moveLimitMin.x);
	position.x = min(position.x, moveLimitMax.x);
	position.y = max(position.y, moveLimitMin.y);
	position.y = min(position.y, moveLimitMax.y);
}

void Player::Roll()
{
	Input* input = Input::GetInstance();

	//緊急回避時の動き
	if (isRoll) {
		//タイマーを更新
		const float rollTime = 40;
		rollTimer++;
		const float time = rollTimer / rollTime;

		//Z軸回転する
		const float endRot = rollEndRot - rotation.y + swayZ;
		rotation.z = Easing::OutBack(rollStartRot, endRot, time);

		//タイマーが指定した時間になったら緊急回避終了
		if (rollTimer >= rollTime) {
			isRoll = false;
		}
	}
	//緊急回避可能時
	else {
		//ダメージ状態なら緊急回避は発動できないで抜ける
		if (isDamage) { return; }

		//緊急回避キーを押していなければ抜ける
		if (!(input->TriggerKey(DIK_RSHIFT) || input->TriggerKey(DIK_LSHIFT) || input->TriggerGamePadButton(Input::PAD_RB)
			|| input->TriggerGamePadButton(Input::PAD_LB))) {
			return;
		}
		//緊急回避状態にする
		isRoll = true;
		//タイマーを初期化
		rollTimer = 0;
		//緊急回避開始時のZ軸角度を記憶
		rollStartRot = rotation.z;

		//緊急回避終了時のZ軸角度をセット
		const float rotAmount = 360; //回転量
		if (input->TriggerKey(DIK_RSHIFT) || input->TriggerGamePadButton(Input::PAD_RB)) { rollEndRot = -rotAmount; }		//右回転
		else if (input->TriggerKey(DIK_LSHIFT) || input->TriggerGamePadButton(Input::PAD_LB)) { rollEndRot = rotAmount; }	//左回転
	}
}

void Player::Attack()
{
	Input* input = Input::GetInstance();

	//弾発射座標を更新
	UpdateBulletShotPos();

	//攻撃ボタンを押したら
	if (input->PushKey(DIK_SPACE) || input->PushGamePadButton(Input::PAD_B)) {
		//攻撃ボタンを押しているときの処理
		PushAttackButton();
	}
	//攻撃ボタンを離したら
	else if (input->ReleaseKey(DIK_SPACE) || input->ReleaseGamePadButton(Input::PAD_B)) {
		//攻撃ボタンを離したときの処理
		ReleaseAttackButton();
	}
}

void Player::PushAttackButton()
{
	//ホーミング弾に切り替わる時間
	const int32_t changeModeTime = 50;
	//チャージ時間を加算
	chargeTimer++;

	//レティクルがチャージ状態でないとき && レティクルがロックオン状態でないとき
	if (!reticles->GetIsChargeMode() && !reticles->GetIsLockon()) {
		//タイマーが指定した時間になったら
		if (chargeTimer >= changeModeTime) {
			//レティクルをチャージ状態にする
			reticles->ChargeModeStart();
		}
	}

	//チャージ未完了時
	if (!isChargeShotMode) {
		//タイマーが指定した時間になったら
		if (chargeTimer >= changeModeTime) {
			//チャージショット状態にする
			isChargeShotMode = true;
		}

		//直進弾発射待機処理
		if (isStraightShotWait) {
			straightShotWaitTimer--;
			if (straightShotWaitTimer <= 0) {
				//待機終了
				isStraightShotWait = false;
			}
			return;
		}

		//直進弾発射
		ShotStraightBullet();

		//直進弾発射待ち時間を設定
		const int32_t waitTime = 10;
		//待機開始
		isStraightShotWait = true;
		straightShotWaitTimer = waitTime;
	}
	//チャージ完了時
	else {
		//チャージ演出が最大の大きさになるまでのフレーム数
		const int maxRatioFrame = 5;
		//チャージショット演出用パーティクルの大きさを決める(チャージした時間で大きくなる)
		float sizeRatio = (float)(chargeTimer - changeModeTime) / maxRatioFrame;
		//最大比率1.0を越えない
		const float maxRatio = 1.0f;
		if (sizeRatio >= maxRatio) { sizeRatio = maxRatio; }
		const float particleSize = homingBulletSize * sizeRatio;

		//チャージショット演出用パーティクル生成
		ParticleEmitter::GetInstance()->ChargeShot(bulletShotPos, particleSize);
	}
}

void Player::ReleaseAttackButton()
{
	//チャージ完了時
	if (isChargeShotMode) {
		//ホーミング弾発射
		ShotHomingBullet();

		//レティクルが敵をロックオンしていなければ、発射と同時にチャージ状態終了
		if (!reticles->GetIsLockon()) {
			reticles->ChargeModeEnd();
		}
	}

	//次に発射ボタンを押した時にすぐ発射できるよう直進弾の発射待機をリセット
	isStraightShotWait = false;
	straightShotWaitTimer = 0;
	//チャージショット状態をリセット
	isChargeShotMode = false;
	chargeTimer = 0;
}

void Player::UpdateBulletShotPos()
{
	//自機の中心座標からの距離
	const Vector3 distancePos = { 0, -0.3f, 4.0f };
	//平行移動行列の計算
	XMMATRIX matTrans = XMMatrixTranslation(distancePos.x, distancePos.y, distancePos.z);

	//ワールド行列の合成
	XMMATRIX bulletShotMatWorld;
	bulletShotMatWorld = XMMatrixIdentity();	//変形をリセット
	bulletShotMatWorld *= matTrans;	//ワールド行列に平行移動を反映

	//自機オブジェクトのワールド行列をかける
	bulletShotMatWorld *= matWorld;

	//弾発射座標を取得
	bulletShotPos = { bulletShotMatWorld.r[3].m128_f32[0], bulletShotMatWorld.r[3].m128_f32[1], bulletShotMatWorld.r[3].m128_f32[2] };
}

void Player::ShotStraightBullet()
{
	//発射位置を自機のワールド座標に設定
	Vector3 shotPos = GetWorldPos();

	//弾の速度を設定
	const float bulletSpeed = 5;
	//自機からレティクルへのベクトルに合わせて飛ばす
	Vector3 velocity = reticles->GetNearReticleWorldPos() - GetWorldPos();
	velocity = velocity.normalize() * bulletSpeed;

	//直進弾を生成
	std::unique_ptr<PlayerBullet> newBullet;
	newBullet.reset(StraightBullet::Create(bulletModel, bulletShotPos, velocity));
	gameScene->AddPlayerBullet(std::move(newBullet));
}

void Player::ShotHomingBullet()
{
	//発射位置を自機のワールド座標に設定
	Vector3 shotPos = GetWorldPos();

	//自機からレティクルへのベクトルに合わせて飛ばす
	Vector3 velocity = reticles->GetNearReticleWorldPos() - GetWorldPos();
	velocity.normalize();

	//ホーミング弾を生成
	std::unique_ptr<PlayerBullet> newBullet;
	newBullet.reset(HomingBullet::Create(bulletModel, bulletShotPos, velocity, homingBulletSize, reticles->GetLockonEnemy()));
	gameScene->AddPlayerBullet(std::move(newBullet));
}

void Player::SetKnockback(const Vector3& subjectPos)
{
	//ノックバックする方向を決める(自機のワールド座標 - 対象のワールド座標)
	knockbackVec = GetWorldPos() - subjectPos;
	//ベクトルを正規化
	knockbackVec.normalize();

	//ノックバックタイマーを初期化
	knockbackTimer = 0;
}

void Player::Knockback()
{
	//ノックバックする時間
	const float knockbackTime = 30;
	knockbackTimer++;
	const float time = knockbackTimer / knockbackTime;

	//速度を作成
	knockbackVel = knockbackVec;
	//Z軸方向には移動しないようにする
	knockbackVel.z = 0;
	//ノックバック時間を速度にかけて減速っぽくする
	knockbackVel *= (1 - time);

	//自機をノックバックさせる
	const float speed = 0.2f;
	position += knockbackVel *= speed;

	//移動限界から出ないようにする
	const Vector2 moveLimitMax = { 10.0f, 6.0f };
	const Vector2 moveLimitMin = { -10.0f, -4.0f };
	position.x = max(position.x, moveLimitMin.x);
	position.x = min(position.x, moveLimitMax.x);
	position.y = max(position.y, moveLimitMin.y);
	position.y = min(position.y, moveLimitMax.y);

	//ノックバックが終了したらダメージ状態を解除する
	if (knockbackTimer >= knockbackTime) {
		isDamage = false;
		color = { 1,1,1,1 };
	}
}

void Player::StageClearSideMove()
{
	//タイマー更新
	stageClearModeTimer++;

	//加速度を設定
	Vector3 accel = { 0.003f, 0, 0.002f };
	//回転角度をセット
	float maxRotZ = -(360 + 65);
	//左側移動時の設定
	if (!isStageClearMoveRight) { accel.x = -accel.x;  maxRotZ = -maxRotZ; }

	//速度に加速度を加算
	stageClearMoveVelocity += accel;
	//座標を移動
	position += stageClearMoveVelocity;

	//回転
	const float rotTime = 120;
	float time = stageClearModeTimer / rotTime;
	time = min(time, 1);
	rotation.x = Easing::OutQuart(stageClearRota.x, 0, time);
	rotation.y = Easing::OutQuart(stageClearRota.y, 0, time);
	rotation.z = Easing::OutQuart(stageClearRota.z, maxRotZ, time);
}

void Player::StageClearReturn()
{
	//タイマー更新
	const float maxTime = 250;
	stageClearModeTimer++;
	const float time = stageClearModeTimer / maxTime;

	//指定した時間を過ぎたら
	float homingDistance = 100;
	const float distanceChangeStartTime = 120;
	if (stageClearModeTimer >= distanceChangeStartTime) {
		//ホーミング方向位置を奥からずらす
		const float changeTime = (stageClearModeTimer - distanceChangeStartTime) / (maxTime - distanceChangeStartTime);
		homingDistance = Easing::LerpFloat(100, 0, changeTime);
	}
	cameraHomingPos.z = stageClearCameraPos.z + homingDistance;

	//ホーミング方向に座標を移動
	Vector3 toCamera = cameraHomingPos - GetWorldPos();
	toCamera.normalize();
	stageClearMoveVelocity.normalize();
	stageClearModeTimer = min(stageClearModeTimer, (int)maxTime);
	const float speed = 0.5f;
	stageClearMoveVelocity = Easing::LerpVec3(stageClearMoveVelocity, toCamera, time) * speed;
	position += stageClearMoveVelocity;

	//進行方向を向くようにする
	rotation.y = XMConvertToDegrees(std::atan2(stageClearMoveVelocity.x, stageClearMoveVelocity.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 velocityZ = MatrixTransformDirection(stageClearMoveVelocity, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-velocityZ.y, velocityZ.z));

	//回転角度Zをセット
	const float downward = 180;
	float maxRotZ = (360 * 3 + downward);
	//左側移動時の設定
	if (!isStageClearMoveRight) { maxRotZ = -maxRotZ; }
	rotation.z = Easing::OutQuad(stageClearRota.z, maxRotZ, time);

	//カメラより後ろへいったら次のフェーズへ
	if (position.z <= stageClearCameraPos.z) {
		stageClearModePhase = StageClearModePhase::Up;

		//角度を回転した分引く
		rotation.z = downward;

		//タイマーを初期化
		stageClearModeTimer = 0;
	}
}

void Player::StageClearUp()
{
	//タイマー更新
	const float upTime = 300;
	stageClearModeTimer++;

	//下向きを上向きに
	const Vector3 rotSpeed = { -0.1f, 0, -1 };
	rotation += rotSpeed;
	const float upRotLimit = 40;
	rotation.x = max(rotation.x, -upRotLimit);
	rotation.z = max(rotation.z, 0);

	//自機が傾いている角度に移動させる
	Vector3 velocity;
	velocity.x = stageClearMoveVelocity.x;
	velocity.y = stageClearMoveVelocity.z * (rotation.x / upRotLimit);
	velocity.z = stageClearMoveVelocity.z;
	position += velocity;

	//タイマーが指定した時間になったら次のフェーズへ
	if (stageClearModeTimer >= upTime) {
		stageClearModePhase = StageClearModePhase::Stay;

		//z軸ゆらゆら回転初期化
		isRotZRight = true;
		swayZ = 0.0f;

		//タイマー初期化
		stageClearModeTimer = 0;
	}
}

void Player::StageClearStay()
{
	//動きがないと寂しいのでゆらゆらさせておく
	const float rotZSpeed = 0.03f;
	const float rotZLimit = 0.8f;
	//右回転
	if (isRotZRight) {
		swayZ += rotZSpeed;
		if (swayZ >= rotZLimit) {
			isRotZRight = false;
		}
	}
	//左回転
	else {
		swayZ -= rotZSpeed;
		if (swayZ <= -rotZLimit) {
			isRotZRight = true;
		}
	}
	rotation.z = swayZ;


	//速度を0に
	const float decelerationTime = 120;
	if (stageClearModeTimer >= decelerationTime) { return; }
	stageClearModeTimer++;
	const float time = stageClearModeTimer / decelerationTime;
	Vector3 velocity = Easing::LerpVec3(stageClearMoveVelocity, { 0, 0, 0 }, time);

	//だんだん減速
	position += velocity;
}

void Player::StageClearBoost()
{
	//ブースト移動
	//自機が傾いている角度に移動させる
	Vector3 velocity = { 0, 0, 1.0f };
	velocity = MatrixTransformDirection(velocity, matWorld);
	position += velocity;

	//ステージクリア後の行動が完了していたら抜ける
	if (isStageClearModeCompletion) { return; }

	//タイマー更新
	const float stageClearModeCompletionTime = 300;
	stageClearModeTimer++;

	//タイマーが指定した時間に以下なら抜ける
	if (stageClearModeTimer < stageClearModeCompletionTime) { return; }

	//ステージクリア後の行動が完了したことにする
	isStageClearModeCompletion = true;
}
