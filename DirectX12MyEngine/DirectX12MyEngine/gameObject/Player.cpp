#include "Player.h"
#include "Input.h"

Player* Player::Create(ObjModel* model)
{
	//自機のインスタンスを生成
	Player* player = new Player();
	if (player == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!player->Initialize()) {
		delete player;
		assert(0);
		return nullptr;
	}

	//モデルをセット
	assert(model);
	player->model = model;

	return player;
}

bool Player::Initialize()
{
	position = { 1,1,5 };
	//scale = { 1.5f, 1.5f, 1.5f };

	//3Dオブジェクトの初期化
	if (!ObjObject3d::Initialize())
	{
		return false;
	}

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

	//3Dオブジェクトの更新
	ObjObject3d::Update();

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
	ObjObject3d::Draw();

	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		bullet->Draw();
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
		//弾の速度を設定
		const float bulletSpeed = 0.5f;
		Vector3 velocity(0, 0, bulletSpeed);

		//自機の向きに合わせて飛ばす

		velocity = MatrixTransform(velocity, matWorld);

		//弾を生成
		std::unique_ptr<PlayerBullet> newBullet;
		newBullet.reset(PlayerBullet::Create(model, position, velocity));
		playerBullets.push_back(std::move(newBullet));
	}
}
