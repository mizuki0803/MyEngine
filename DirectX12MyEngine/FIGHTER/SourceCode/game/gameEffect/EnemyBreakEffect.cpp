#include "EnemyBreakEffect.h"

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
    //移動
    Move();

    //オブジェクト更新
    ObjObject3d::Update();
}

void EnemyBreakEffect::Move()
{
    //重力っぽい値をY軸にかけ続ける
    const float gravity = 0.01f;
    velocity.y -= gravity;
    position += velocity;

	//回転させる
	rotation += rotSpeed;

    //地面に着いたら削除状態にする
    if (position.y <= 0.0f) {
        isDelete = true;
    }
}
