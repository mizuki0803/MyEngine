#include "EnemyBreakEffect.h"

bool EnemyBreakEffect::isGroundMode = true;
bool EnemyBreakEffect::isGravityMode = false;
GameCamera* EnemyBreakEffect::gameCamera = nullptr;

EnemyBreakEffect* EnemyBreakEffect::Create(ObjModel* model, const Vector3& position, const Vector3& velocity, const Vector3& rotSpeed, const Vector3& scale)
{
	//雪玉エフェクトのインスタンスを生成
	EnemyBreakEffect* enemyBreakEffect = new EnemyBreakEffect();
	if (enemyBreakEffect == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!enemyBreakEffect->Initialize()) {
		delete enemyBreakEffect;
		assert(0);
		return nullptr;
	}

	//モデルをセット
	assert(model);
	enemyBreakEffect->model = model;

	//座標をセット
	enemyBreakEffect->position = position;

	//速度をセット
	enemyBreakEffect->velocity = velocity;

	//回転の速さをセット
	enemyBreakEffect->rotSpeed = rotSpeed;

	//大きさをセット
	enemyBreakEffect->scale = scale;

	return enemyBreakEffect;
}

void EnemyBreakEffect::Update()
{
	//時間経過で削除
	if (--deathTimer <= 0) {
		isDelete = true;
	}

    //移動
    Move();

    //オブジェクト更新
    ObjObject3d::Update();

	//画面外手前まで行ったら削除
	FrontOfScreenDelete();
}

void EnemyBreakEffect::Move()
{
	//重力あり状態なら
	if (isGravityMode) {
		//重力っぽい値をY軸にかけ続ける
		const float gravity = 0.01f;
		velocity.y -= gravity;
	}
	//速度を座標に加算して移動させる
    position += velocity;

	//回転させる
	rotation += rotSpeed;

    //地面あり状態の場合のみ地面に着いたら削除状態にする
    if (isGroundMode && position.y <= 0.0f) {
        isDelete = true;
    }
}

void EnemyBreakEffect::FrontOfScreenDelete()
{
	//座標が自機より手前(画面外手前)まで行ったら削除
	const float flontOfScreenDiffence = 50;
	const float deletePos = gameCamera->GetPosition().z - flontOfScreenDiffence;

	if (position.z <= deletePos) {
		isDelete = true;
	}
}
