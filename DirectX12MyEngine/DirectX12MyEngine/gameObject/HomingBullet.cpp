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

	//座標をセット
	homingBullet->position = position;

	//速度をセット
	homingBullet->velocity = velocity;

	//追従対象の敵をセット
	homingBullet->enemy = enemy;

	return homingBullet;
}

void HomingBullet::Update()
{
	//座標を移動
	if (enemy) {
		Vector3 toPlayer = enemy->GetWorldPos() - GetWorldPos();
		toPlayer.normalize();
		velocity.normalize();
		const float maxTime = 180;
		float time = homingTimer / maxTime;
		velocity = Easing::Lerp(velocity, toPlayer, time) * 0.3f;
		homingTimer++;
		homingTimer = min(homingTimer, maxTime);
	}

	position += velocity;


	//進行方向を向くようにする
	rotation.y = XMConvertToDegrees(std::atan2(velocity.x, velocity.z));
	XMMATRIX matRot;
	matRot = XMMatrixRotationY(XMConvertToRadians(-rotation.y));
	Vector3 velocityZ = MatrixTransformDirection(velocity, matRot);
	rotation.x = XMConvertToDegrees(std::atan2(-velocityZ.y, velocityZ.z));

	//3Dオブジェクトの更新
	ObjObject3d::Update();
}
