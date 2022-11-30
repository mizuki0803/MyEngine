#include "StraightBullet.h"
#include "ParticleEmitter.h"

StraightBullet* StraightBullet::Create(ObjModel* model, const Vector3& position, const Vector3& velocity)
{
	//直進弾のインスタンスを生成
	StraightBullet* straightBullet = new StraightBullet();
	if (straightBullet == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!straightBullet->Initialize()) {
		delete straightBullet;
		assert(0);
		return nullptr;
	}

	//モデルをセット
	assert(model);
	straightBullet->model = model;

	//弾の種類をセット
	straightBullet->bulletType = BulletType::Straight;

	//座標をセット
	straightBullet->position = position;

	//速度をセット
	straightBullet->velocity = velocity;

	return straightBullet;
}

void StraightBullet::Update()
{
	//座標を移動
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
}

void StraightBullet::OnCollision(const Vector3& subjectPos, float subjectSize, bool isToDamage)
{
	//死亡させる
	isDead = true;

	//敵内部に演出が出てしまうことがあるので、敵のZ座標から敵の大きさ分押し戻す
	Vector3 pos = GetWorldPos();
	pos.z = subjectPos.z - subjectSize;
	//ショット死亡演出用パーティクル生成
	ParticleEmitter::GetInstance()->ShotDead(pos, scale.x);
}

void StraightBullet::CollisionGround()
{
	//Y座標0以下でなければ抜ける
	if (position.y > 0) { return; }

	//死亡
	isDead = true;
	//ショット死亡演出用パーティクル生成
	Vector3 pos = GetWorldPos();
	pos.y = 0;
	ParticleEmitter::GetInstance()->ShotDead(pos, scale.x);
}
