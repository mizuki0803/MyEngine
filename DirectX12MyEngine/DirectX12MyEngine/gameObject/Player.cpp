#include "Player.h"
#include "Input.h"

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
	position = { 0 ,-5 ,25 };
	scale = { 2, 2, 2 };

	//3Dオブジェクトの初期化
	if (!ObjObject3d::Initialize())
	{
		return false;
	}

	//レティクルを生成
	reticle.reset(Reticle::Create(1, 25.0f, { 100, 100 }));
	reticle2.reset(Reticle::Create(1, 50.0f, { 50, 50 }));

	return true;
}

void Player::Update()
{
	//死亡した弾の削除
	playerBullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->GetIsDead();
		});

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

	//弾更新
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		bullet->Update();
	}
}

void Player::Draw()
{
	//オブジェクト描画
	ObjObject3d::Draw();

	//自機弾描画
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		bullet->Draw();
	}
}

void Player::DrawUI()
{
	//レティクル描画
	reticle->Draw();
	reticle2->Draw();
}

void Player::OnCollision()
{
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

void Player::Move()
{
	Input* input = Input::GetInstance();

	//入力で移動させる
	Vector3 move = { 0, 0, 0 };
	float moveSpeed = 0.1f;
	if (input->PushKey(DIK_RIGHT)) { move.x += moveSpeed; }
	if (input->PushKey(DIK_LEFT)) { move.x -= moveSpeed; }
	if (input->PushKey(DIK_UP)) { move.y += moveSpeed; }
	if (input->PushKey(DIK_DOWN)) { move.y -= moveSpeed; }

	position += move;

	//移動限界をから出ないようにする
	const XMFLOAT2 moveLimit = { 5.0f, 5.0f };

	position.x = max(position.x, -moveLimit.x);
	position.x = min(position.x, +moveLimit.x);
	position.y = max(position.y, -moveLimit.y);
	position.y = min(position.y, +moveLimit.y);
}

void Player::Rotate()
{
	Input* input = Input::GetInstance();

	//入力で回転させる
	Vector3 rot = { 0, 0, 0 };
	float rotSpeed = 1.0f;
	if (input->PushKey(DIK_W)) { rot.x -= rotSpeed; }
	if (input->PushKey(DIK_S)) { rot.x += rotSpeed; }
	if (input->PushKey(DIK_A)) { rot.y -= rotSpeed; }
	if (input->PushKey(DIK_D)) { rot.y += rotSpeed; }

	rotation += rot;
}

void Player::Attack()
{
	Input* input = Input::GetInstance();
	//発射キーを押したら
	if (input->TriggerKey(DIK_SPACE))
	{
		//発射位置を自機のワールド座標に設定
		Vector3 shotPos = GetWorldPos();

		//弾の速度を設定
		const float bulletSpeed = 0.5f;
		//自機からレティクルへのベクトルに合わせて飛ばす
		Vector3 velocity = reticle->GetWorldPos() - GetWorldPos();
		velocity = velocity.normalize() * bulletSpeed;

		//弾を生成
		std::unique_ptr<PlayerBullet> newBullet;
		newBullet.reset(PlayerBullet::Create(model, shotPos, velocity));
		playerBullets.push_back(std::move(newBullet));
	}
}
