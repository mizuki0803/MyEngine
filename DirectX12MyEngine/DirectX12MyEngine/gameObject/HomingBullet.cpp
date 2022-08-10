#include "HomingBullet.h"
#include "Easing.h"
#include "Enemy.h"

HomingBullet* HomingBullet::Create(ObjModel* model, const Vector3& position, const Vector3& velocity, Enemy* enemy)
{
	//ホーミング弾のインスタンスを生成
	HomingBullet* homingBullet = new HomingBullet();
	if (homingBullet == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!homingBullet->Initialize()) {
		delete homingBullet;
		assert(0);
		return nullptr;
	}

	//モデルをセット
	assert(model);
	homingBullet->model = model;

	//弾の種類をセット
	homingBullet->bulletType = BulletType::Homing;

	//座標をセット
	homingBullet->position = position;

	//速度をセット
	homingBullet->velocity = velocity;

	//追従対象の敵をセット
	homingBullet->enemy = enemy;

	return homingBullet;
}

bool HomingBullet::Initialize()
{
	//色を黄色に変更
	XMFLOAT4 yellow = { 1.0f, 1.0f, 0.2f, 1 };
	color = yellow;

	//3Dオブジェクトの初期化
	if (!PlayerBullet::Initialize())
	{
		return false;
	}

	return true;
}

void HomingBullet::Update()
{
	//座標を移動
	if (enemy) {
		Vector3 toPlayer = enemy->GetWorldPos() - GetWorldPos();
		toPlayer.normalize();
		velocity.normalize();
		const float maxTime = 120;
		homingTimer++;
		homingTimer = min(homingTimer, maxTime);
		const float time = homingTimer / maxTime;
		const float speed = 1.5f;
		velocity = Easing::Lerp(velocity, toPlayer, time) * speed;
	}
	position += velocity;


	//時間経過で死亡
	if (--deathTimer <= 0) {
		isDead = true;
	}

	//進行方向を向くようにする
	rotation.y = XMConvertToDegrees(std::atan2(velocity.x, velocity.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 velocityZ = MatrixTransformDirection(velocity, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-velocityZ.y, velocityZ.z));

	//3Dオブジェクトの更新
	ObjObject3d::Update();
}
