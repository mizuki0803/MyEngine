#include "UpDownEnemy.h"
#include "Player.h"

void (UpDownEnemy::* UpDownEnemy::actionFuncTable[])() = {
	&UpDownEnemy::UpBrake,
	&UpDownEnemy::DownBrake,
};

UpDownEnemy* UpDownEnemy::Create(ObjModel* model, const Vector3& position)
{
	//敵のインスタンスを生成
	UpDownEnemy* upDownEnemy = new UpDownEnemy();
	if (upDownEnemy == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	upDownEnemy->model = model;

	// 初期化
	if (!upDownEnemy->Initialize()) {
		delete upDownEnemy;
		assert(0);
		return nullptr;
	}

	//座標をセット
	upDownEnemy->position = position;

	return upDownEnemy;
}

void UpDownEnemy::Update()
{
	//行動
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//3Dオブジェクトの更新
	ObjObject3d::Update();

	//画面外手前まで行ったら削除
	FrontOfScreenDelete();
}

void UpDownEnemy::UpBrake()
{
	//上昇を抑える
	Vector3 accel = { 0, -0.01f, 0 };
	velocity += accel;
	position += velocity;


	//指定した速度まで下降状態になったら下降にブレーキをかける
	const float changePhaseVelY = -0.5f;
	if (velocity.y <= changePhaseVelY) {
		phase = Phase::DownBrake;
	}
}

void UpDownEnemy::DownBrake()
{
	//下降を抑える
	Vector3 accel = { 0, 0.01f, 0 };
	velocity += accel;
	position += velocity;


	//指定した速度まで上昇状態になったら上昇にブレーキをかける
	const float changePhaseVelY = 0.5f;
	if (velocity.y >= changePhaseVelY) {
		phase = Phase::UpBrake;
	}
}
