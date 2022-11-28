#include "CircularEnemy.h"

ObjModel* CircularEnemy::enemyModel = nullptr;
std::array<ObjModel*, 5> CircularEnemy::breakModels;

CircularEnemy* CircularEnemy::Create(const Vector3& centerPosition, const float angle, const float length, const float rotSpeed)
{
	//敵のインスタンスを生成
	CircularEnemy* circularEnemy = new CircularEnemy();
	if (circularEnemy == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(enemyModel);
	circularEnemy->model = CircularEnemy::enemyModel;

	// 初期化
	if (!circularEnemy->Initialize()) {
		delete circularEnemy;
		assert(0);
		return nullptr;
	}

	//中心座標をセット
	circularEnemy->centerPosition = centerPosition;

	//初期角度をセット
	circularEnemy->angle = angle;

	//半径の長さをセット
	circularEnemy->length = length;

	//回転速度をセット
	circularEnemy->rotSpeed = rotSpeed;

	return circularEnemy;
}

void CircularEnemy::SetBreakModel(int modelNum, ObjModel* model)
{
	//破壊時のモデルをセット
	assert(model);
	breakModels[modelNum] = model;
}

void CircularEnemy::Update()
{
	//発射タイマーカウントダウン
	--fireTimer;
	if (fireTimer <= 0) {
		//弾を発射
		Fire();
		//発射タイマーを初期化
		fireTimer = fireInterval;
	}

	//移動
	Move();

	//オブジェクトの更新
	Enemy::Update();

	//画面外手前まで行ったら削除
	FrontOfScreenDelete();
}

void CircularEnemy::OnCollision()
{
	//全敵共通の衝突処理
	Enemy::OnCollision();

	//削除する
	isDelete = true;

	//破壊
	Break();
}

void CircularEnemy::Move()
{
	//角度を弧度法に変換
	float radius = XMConvertToRadians(angle);

	//円の中心から指定した半径の長さをぐるぐる回る
	position.x = cos(radius) * length + centerPosition.x;
	position.y = sin(radius) * length + centerPosition.y;
	position.z = centerPosition.z;

	//角度の更新
	angle += rotSpeed;
}

void CircularEnemy::Break()
{
	//破壊用エフェクト追加
	for (int i = 0; i < breakModels.size(); i++) {
		//モデルが未設定なら飛ばす
		if (!breakModels[i]) { continue; }

		//ランダムでエフェクトの速度をセット
		const Vector3 randVel = { 4, 4, 4 };
		Vector3 velocity;
		velocity.x = (float)((rand() % (int)randVel.x) - randVel.x / 2);
		velocity.y = (float)((rand() % (int)randVel.y) - randVel.y / 4);
		velocity.z = (float)((rand() % (int)randVel.z) - randVel.z / 2);

		//ランダムでエフェクトの回転の速さをセット
		const Vector3 randRotSpeed = { 5, 5, 5 };
		Vector3 rotSpeed;
		rotSpeed.x = (float)((rand() % (int)randRotSpeed.x) - randRotSpeed.x / 2);
		rotSpeed.x = (float)((rand() % (int)randRotSpeed.y) - randRotSpeed.y / 2);
		rotSpeed.z = (float)((rand() % (int)randRotSpeed.z) - randRotSpeed.z / 2);

		//値が大きいので割り算して小さくする
		const float div = 10;
		velocity /= div;
		//大きさをセット
		const Vector3 scale = { 1.5f, 1.5f, 1.5f };

		//破壊時エフェクト生成
		BreakEffect(breakModels[i], velocity, rotSpeed, scale);
	}
}
