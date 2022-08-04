#include "CircularEnemy.h"

CircularEnemy* CircularEnemy::Create(ObjModel* model, const Vector3& centerPosition, const float angle, const float length, const float rotSpeed)
{
	//敵のインスタンスを生成
	CircularEnemy* circularEnemy = new CircularEnemy();
	if (circularEnemy == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	circularEnemy->model = model;

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

	//3Dオブジェクトの更新
	ObjObject3d::Update();

	//画面外手前まで行ったら削除
	FrontOfScreenDelete();
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
