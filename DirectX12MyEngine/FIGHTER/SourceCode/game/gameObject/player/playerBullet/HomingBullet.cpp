#include "HomingBullet.h"
#include "Easing.h"
#include "Enemy.h"
#include "ParticleEmitter.h"

const float HomingBullet::blastSize = 16.0f;

HomingBullet* HomingBullet::Create(const Vector3& position, const Vector3& velocity, const float size, Enemy* enemy)
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

	//弾の種類をセット
	homingBullet->bulletType = BulletType::Homing;

	//座標をセット
	homingBullet->position = position;

	//速度をセット
	homingBullet->velocity = velocity;

	//サイズをセット
	homingBullet->scale = { size, size, size };

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
		const float maxTime = 120;
		homingTimer++;
		homingTimer = min(homingTimer, maxTime);
		const float time = homingTimer / maxTime;
		const float speed = 1.5f;
		velocity = Easing::LerpVec3(velocity, toPlayer, time) * speed;
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
	PlayerBullet::Update();

	//チャージショット演出用パーティクル生成
	ParticleEmitter::GetInstance()->ChargeShot(GetWorldPos(), scale.x);
}

void HomingBullet::OnCollision(float subjectSize)
{
	//死亡させる
	isDead = true;

	//チャージショット死亡演出用パーティクル生成
	ParticleEmitter::GetInstance()->ChargeShotDead(GetWorldPos(), blastSize);
}

void HomingBullet::CollisionGround()
{
	//Y座標0以下でなければ抜ける
	if (position.y > 0) { return; }

	//死亡
	isDead = true;
	//チャージショット死亡演出用パーティクル生成
	ParticleEmitter::GetInstance()->ChargeShotDead(GetWorldPos(), blastSize);
}
