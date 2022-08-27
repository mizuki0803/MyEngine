#include "BossAvatar.h"
#include "Easing.h"
#include "GameScene.h"

void (BossAvatar::* BossAvatar::attackTypeBPhaseFuncTable[])() = {
	&BossAvatar::AttackTypeBLockon,
	&BossAvatar::AttackTypeBShot,
	&BossAvatar::AttackTypeBBack,
	&BossAvatar::Stay,
};

void (BossAvatar::* BossAvatar::attackTypeCPhaseFuncTable[])() = {
	&BossAvatar::AttackTypeCMoveCenter,
	&BossAvatar::AttackTypeCRotStart,
	&BossAvatar::AttackTypeCShot,
	&BossAvatar::AttackTypeCRotEnd,
	&BossAvatar::Stay,
};


GameScene* BossAvatar::gameScene = nullptr;
ObjModel* BossAvatar::bulletModel = nullptr;
const float BossAvatar::attackModeRotY = 180.0f;
const float BossAvatar::waitModeRotY = 0.0f;
const float BossAvatar::attackCLength = 1.5f;

bool BossAvatar::Initialize()
{
	//大きさは本体の3/4
	scale = { 0.75f, 0.75f, 0.75f };

	if (!ObjObject3d::Initialize()) {
		return false;
	}

	return true;
}

void BossAvatar::Damage(int damageNum)
{
	//引数の数字の分ダメージを喰らう
	HP -= damageNum;

	//HPが0以下になったら死亡
	if (HP <= 0) {
		isDead = true;
	}
}

void BossAvatar::AttackTypeB(const Vector3& playerPosition)
{
	//自機座標をロックオンする
	attackBLockonPos = playerPosition;

	//行動
	(this->*attackTypeBPhaseFuncTable[static_cast<size_t>(attackBPhase)])();
}

void BossAvatar::AttackTypeC()
{
	//行動
	(this->*attackTypeCPhaseFuncTable[static_cast<size_t>(attackCPhase)])();
}

void BossAvatar::ChangeAttackMode(const float time)
{
	//180度回転させて反対向きにする
	rotation.y = Easing::InOutBack(waitModeRotY, attackModeRotY, time);
}

void BossAvatar::ChangeWaitMode(const float time)
{
	//180度回転させて反対向きにする
	rotation.x = 0;
	rotation.y = Easing::InOutBack(attackModeRotY, waitModeRotY, time);
	rotation.z = 0;
}

void BossAvatar::ReturnFixedPosition(const float time)
{
	//親子関係上での定位置座標に戻す
	position.x = Easing::OutQuad(returnStartPos.x, fixedPos.x, time);
	position.y = Easing::OutQuad(returnStartPos.y, fixedPos.y, time);
	position.z = Easing::OutQuad(returnStartPos.z, fixedPos.z, time);
}

void BossAvatar::AttackEnd()
{
	//呼び出した瞬間の座標を固定位置に戻るときの出発座標として記録しておく
	returnStartPos = position;

	//弾発射タイマーを初期化
	fireTimer = 0;
	//弾発射状態を解除
	isFire = false;

	//攻撃内容Bの変数の初期化
	attackBPhase = AttackTypeBPhase::Lockon;
	attackBTimer = 0;

	//攻撃内容Cの変数の初期化
	attackCPhase = AttackTypeCPhase::MoveCenter;
	attackCTimer = 0;
	attackCRotSpeed = 0;
}

Vector3 BossAvatar::GetWorldPos()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//平行移動成分を取得
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
}

void BossAvatar::Fire(const float scale)
{
	//弾の速度を設定
	const float bulletSpeed = 1.0f;
	Vector3 velocity(0, 0, bulletSpeed);
	velocity = MatrixTransformDirection(velocity, matWorld);

	//弾を生成
	std::unique_ptr<EnemyBullet> newBullet;
	newBullet.reset(EnemyBullet::Create(bulletModel, GetWorldPos(), velocity, scale));
	gameScene->AddEnemyBullet(std::move(newBullet));
}

void BossAvatar::AttackTypeBShot()
{
	//ロックオンで決めた速度で飛ばす
	position += attackBVelocity;

	//タイマーを更新
	const float shotTime = 180;
	attackBTimer++;

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackBTimer >= shotTime) {
		attackBPhase = AttackTypeBPhase::Back;

		//タイマー初期化
		attackBTimer = 0;
	}
}

void BossAvatar::AttackTypeBBack()
{
	//タイマーを更新
	const float backTime = 120;
	attackBTimer++;
	const float time = attackBTimer / backTime;

	//親子関係上での定位置座標に戻す
	position.x = Easing::OutQuad(0, fixedPos.x, time);
	position.y = Easing::OutQuad(60, fixedPos.y, time);
	position.z = Easing::OutQuad(0, fixedPos.z, time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackBTimer >= backTime) {
		attackBPhase = AttackTypeBPhase::Stay;

		//タイマー初期化
		attackBTimer = 0;
	}
}

void BossAvatar::AttackTypeCRotStart()
{
	//回転を開始(回転速度を速くする)
	const float rotAccelSpeed = 0.1f;
	attackCRotSpeed += rotAccelSpeed;

	//角度を弧度法に変換
	float radius = XMConvertToRadians(attackCAngle);

	//円の中心から指定した半径の長さをぐるぐる回る
	position.x = cos(radius) * attackCLength;
	position.y = sin(radius) * attackCLength;

	//角度の更新
	attackCAngle += attackCRotSpeed;

	//回転速度が指定した速度以上になったら次のフェーズへ
	const float maxRotSpeed = 4.0f;
	if (attackCRotSpeed >= maxRotSpeed) {
		attackCPhase = AttackTypeCPhase::Shot;
	}
}

void BossAvatar::AttackTypeCShot()
{
	//タイマーを更新
	const float moveTime = 300;
	attackCTimer++;

	//角度を弧度法に変換
	float radius = XMConvertToRadians(attackCAngle);

	//円の中心から指定した半径の長さをぐるぐる回る
	position.x = cos(radius) * attackCLength;
	position.y = sin(radius) * attackCLength;

	//角度の更新
	attackCAngle += attackCRotSpeed;

	//弾発射タイマーカウント
	fireTimer++;
	const int fireInterval = 10;
	if (fireTimer >= fireInterval) {
		//弾を発射
		const float bulletScale = 1.0f;
		Fire(bulletScale);
		//発射タイマーを初期化
		fireTimer = 0;
	}

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackCTimer >= moveTime) {
		attackCPhase = AttackTypeCPhase::RotEnd;

		//タイマー初期化
		attackCTimer = 0;
	}
}

void BossAvatar::AttackTypeCRotEnd()
{
	//回転を終了(回転速度を遅くする)
	const float rotAccelSpeed = -0.1f;
	attackCRotSpeed += rotAccelSpeed;

	//角度を弧度法に変換
	float radius = XMConvertToRadians(attackCAngle);

	//円の中心から指定した半径の長さをぐるぐる回る
	position.x = cos(radius) * attackCLength;
	position.y = sin(radius) * attackCLength;

	//角度の更新
	attackCAngle += attackCRotSpeed;

	//回転速度が0以下になったら次のフェーズへ
	if (attackCRotSpeed <= 0) {
		attackCPhase = AttackTypeCPhase::Stay;
	}
}

void BossAvatar::Stay()
{
}
