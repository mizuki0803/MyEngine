#include "Player.h"
#include "Input.h"
#include "GameScene.h"
#include "StraightBullet.h"
#include "HomingBullet.h"

GameScene* Player::gameScene = nullptr;
ObjModel* Player::bulletModel = nullptr;
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
	position = { 0 ,1 ,10 };
	scale = { 0.5f, 0.5f, 0.5f };

	//3Dオブジェクトの初期化
	if (!ObjObject3d::Initialize())
	{
		return false;
	}

	//レティクルを生成
	reticle.reset(Reticle::Create(1, 15.0f, { 100, 100 }));
	reticle2.reset(Reticle::Create(1, 25.0f, { 50, 50 }));

	//体力を設定
	HP = 3;

	return true;
}

void Player::Update()
{
	//回転
	Rotate();

	//移動
	Move();

	//オブジェクト更新
	ObjObject3d::Update();

	//レティクル更新
	reticle->Update(matWorld, camera->GetMatView(), camera->GetMatProjection());
	reticle2->Update(matWorld, camera->GetMatView(), camera->GetMatProjection());

	//攻撃
	Attack();
}

void Player::DrawUI()
{
	//レティクル描画
	reticle->Draw();
	reticle2->Draw();
}

void Player::OnCollision()
{
	//体力を減らす
	HP--;

	//HPが0以下になったら死亡させる
	if (HP <= 0) {
		isDead = true;
	}
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

void Player::Rotate()
{
	Input* input = Input::GetInstance();

	//回転速度
	const float rotSpeed = 1.0f;
	//角度修正速度
	const float backSpeed = rotSpeed / 2.0f;
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
	rotation.x = max(rotation.x, -rotLimit.x);
	rotation.x = min(rotation.x, +rotLimit.x);
	rotation.y = max(rotation.y, -rotLimit.y);
	rotation.y = min(rotation.y, +rotLimit.y);
}

void Player::Move()
{
	//自機が傾いている角度に移動させる
	Vector3 move = { 0, 0, 0 };
	const float moveSpeed = 0.15f;
	move.x = moveSpeed * (rotation.y / rotLimit.y);
	move.y = moveSpeed * -(rotation.x / rotLimit.x);
	position += move;

	//移動限界から出ないようにする
	const Vector2 moveLimit = { 10.0f, 5.0f };
	position.x = max(position.x, -moveLimit.x);
	position.x = min(position.x, +moveLimit.x);
	position.y = max(position.y, -moveLimit.y);
	position.y = min(position.y, +moveLimit.y);
}

void Player::Attack()
{
	Input* input = Input::GetInstance();
	//発射キーを押したら
	if (input->PushKey(DIK_SPACE) || input->PushGamePadButton(Input::PAD_B)) {
		//チャージ時間を加算
		chargeTimer++;

		//チャージ未完了時
		if (!isChargeShotMode) {
			//ホーミング弾に切り替わる時間
			const int32_t changeModeTime = 60;
			if (chargeTimer >= changeModeTime) {
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
	}
	//発射キーを離したら
	else if (input->ReleaseKey(DIK_SPACE) || input->ReleaseGamePadButton(Input::PAD_B)) {
		//チャージ完了時
		if (isChargeShotMode) {
			//ホーミング弾発射
			ShotHomingBullet();
		}

		//次に発射ボタンを押した時にすぐ発射できるよう直進弾の発射待機をリセット
		isStraightShotWait = false;
		straightShotWaitTimer = 0;
		//チャージショット状態をリセット
		isChargeShotMode = false;
		chargeTimer = 0;
	}
}

void Player::ShotStraightBullet()
{
	//発射位置を自機のワールド座標に設定
	Vector3 shotPos = GetWorldPos();

	//弾の速度を設定
	const float bulletSpeed = 5;
	//自機からレティクルへのベクトルに合わせて飛ばす
	Vector3 velocity = reticle->GetReticle3D()->GetWorldPos() - GetWorldPos();
	velocity = velocity.normalize() * bulletSpeed;

	//直進弾を生成
	std::unique_ptr<PlayerBullet> newBullet;
	newBullet.reset(StraightBullet::Create(bulletModel, shotPos, velocity));
	gameScene->AddPlayerBullet(std::move(newBullet));
}

void Player::ShotHomingBullet()
{
	//発射位置を自機のワールド座標に設定
	Vector3 shotPos = GetWorldPos();

	//自機からレティクルへのベクトルに合わせて飛ばす
	Vector3 velocity = reticle->GetReticle3D()->GetWorldPos() - GetWorldPos();
	velocity.normalize();

	//ホーミング弾を生成
	std::unique_ptr<PlayerBullet> newBullet;
	newBullet.reset(HomingBullet::Create(bulletModel, shotPos, velocity, reticle2->GetReticle2D()->GetLockonEnemy()));
	gameScene->AddPlayerBullet(std::move(newBullet));
}
