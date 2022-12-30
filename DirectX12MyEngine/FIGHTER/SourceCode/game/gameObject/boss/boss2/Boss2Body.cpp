#include "Boss2Body.h"
#include "Easing.h"
#include "ParticleEmitter.h"

BaseStageScene* Boss2Body::stageScene = nullptr;
ObjModel* Boss2Body::bodyModel = nullptr;
const Vector3 Boss2Body::normalSize = { 4.5f, 4.5f, 4.5f };
const Vector3 Boss2Body::damageSize = Boss2Body::normalSize * 1.1f;
const XMFLOAT4 Boss2Body::damageColor = { 1, 0.2f, 0.2f, 1 };

Boss2Body* Boss2Body::Create(const Vector3& basePos)
{
	//メダマーン(本体)のインスタンスを生成
	Boss2Body* boss2Body = new Boss2Body();
	if (boss2Body == nullptr) {
		return nullptr;
	}

	//寝ている状態のモデルをセット
	assert(bodyModel);
	boss2Body->model = bodyModel;

	// 初期化
	if (!boss2Body->Initialize()) {
		delete boss2Body;
		assert(0);
		return nullptr;
	}

	//停止する基準の座標をセット
	boss2Body->basePos = basePos;

	//大きさをセット
	boss2Body->scale = normalSize;

	return boss2Body;
}

void Boss2Body::Update()
{
	//ダメージ状態のみの処理
	if (isDamage) {
		DamageMode();
	}

	//オブジェクト更新
	ObjObject3d::Update();
}

void Boss2Body::Damage(int attackPower, const Vector3& collisionPos, const Vector3& subjectVel)
{
	//引数の攻撃力をダメージ量にセット
	damageNum = attackPower;

	//ダメージを与える
	HP -= damageNum;

	//HPが0以下になったら死亡
	if (HP <= 0) {
		isDead = true;

		//HPゲージバグを起こさないようマイナス分を0に調整
		damageNum += HP;
	}

	//ダメージ状態にする
	isDamage = true;
	//ダメージを喰らった瞬間なのでtrue
	isDamageTrigger = true;
	//ダメージ状態タイマー初期化
	damageTimer = 0;
	//色を変更
	color = damageColor;

	//サイズを少し大きくする
	scale = damageSize;

	//ノックバック情報をセット
	SetDamageKnockback(subjectVel);

	//爆発生成する
	DamageExplosion(collisionPos);
}

void Boss2Body::Fall(const float time)
{
	//基準の位置の真上から降りてくる
	Vector3 bornPos = basePos;
	const float fallNum = 70;
	bornPos.y = basePos.y + fallNum;
	position = Easing::LerpVec3(bornPos, basePos, time);
}

Vector3 Boss2Body::GetWorldPos()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//平行移動成分を取得
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
}

void Boss2Body::Wait()
{
}

void Boss2Body::DamageMode()
{
	//ダメージ状態の時間
	const int damageTime = 20;
	damageTimer++;

	//ノックバック
	DamageKnockback();

	//大きくしたサイズを戻す
	DamageSizeReturn();

	//ダメージ色切り替え
	DamageColorMode();

	//ダメージトリガーフラグがtrueなら下ろしておく
	if (isDamageTrigger) { isDamageTrigger = false; }

	//タイマーが指定した時間になったら
	if (damageTimer >= damageTime) {
		//ダメージ状態を終了
		isDamage = false;

		//色を元に戻しておく
		color = { 1, 1, 1, 1 };
	}
}

void Boss2Body::SetDamageKnockback(const Vector3& subjectVel)
{
	//ノックバックする方向を決める(対象の速度の方向)
	knockbackVec = subjectVel;
	knockbackVec.normalize();
}

void Boss2Body::DamageKnockback()
{
	//ノックバックする時間
	const float knockbackTime = 5;
	//指定した時間以上なら抜ける
	if (damageTimer > knockbackTime) { return; }

	const float time = damageTimer / knockbackTime;

	//速度を作成
	const float knockbackBaseSpeed = 0.2f;
	knockbackVel = knockbackVec * (1 - time) * knockbackBaseSpeed;

	//自機をノックバックさせる
	position += knockbackVel;
}

void Boss2Body::DamageSizeReturn()
{
	//大きくしたサイズを元に戻す時間
	const float sizeReturnTime = 10;
	//指定した時間以上なら抜ける
	if (damageTimer > sizeReturnTime) { return; }
	const float time = damageTimer / sizeReturnTime;

	//大きさを元に戻す
	scale = Easing::LerpVec3(damageSize, normalSize, time);
}

void Boss2Body::DamageExplosion(const Vector3& position)
{
	//敵内部に演出が出てしまうことがあるので、敵の大きさ分押し戻す
	Vector3 pos = position;
	pos.z -= scale.z / 2;
	//ランダムでずらす
	const Vector3 randPos = { 2, 2, 1 };
	pos.x += (float)((rand() % (int)randPos.x) - randPos.x / 2);
	pos.y += (float)((rand() % (int)randPos.y) - randPos.y / 2);
	pos.z += (float)((rand() % (int)randPos.z));

	//ショット死亡演出用パーティクル生成
	const float size = 0.75f;
	ParticleEmitter::GetInstance()->Explosion(pos, size);
}

void Boss2Body::DamageColorMode()
{
	//ダメージ色切り替え時間
	const int damageColorChangeTime = 2;

	//タイマーが指定した時間になったら
	if (damageTimer % damageColorChangeTime == 0) {
		//ダメージ色状態を切り替える
		if (isDamageColor) {
			isDamageColor = false;

			//色を元に戻す
			color = { 1, 1, 1, 1 };
		}
		else {
			isDamageColor = true;

			//ダメージ色にする
			color = damageColor;
		}
	}
}
