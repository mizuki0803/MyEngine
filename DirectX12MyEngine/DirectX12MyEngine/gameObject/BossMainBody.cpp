#include "BossMainBody.h"
#include "Easing.h"

const float BossMainBody::attackModeRotY = 180.0f;
const float BossMainBody::waitModeRotY = 0.0f;

BossMainBody* BossMainBody::Create(ObjModel* model, const Vector3& position)
{
	//ボス(本体)のインスタンスを生成
	BossMainBody* bossMainBody = new BossMainBody();
	if (bossMainBody == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	bossMainBody->model = model;

	// 初期化
	if (!bossMainBody->Initialize()) {
		delete bossMainBody;
		assert(0);
		return nullptr;
	}

	//座標をセット
	bossMainBody->position = position;

	//初期座標をセット
	bossMainBody->startPos = position;

	return bossMainBody;
}

void BossMainBody::Damage(int damageNum)
{
	//引数の数字の分ダメージを喰らう
	HP -= damageNum;

	//HPが0以下になったら死亡
	if (HP <= 0) {
		isDead = true;
	}
}

void BossMainBody::Fall(const float time)
{
	//生成位置から降りたところで停止する
	Vector3 stayPos = startPos;
	stayPos.y = 0;
	position = Easing::Lerp(startPos, stayPos, time);
}

void BossMainBody::ChangeAttackMode(const float time)
{
	//180度回転させて反対向きにする
	rotation.y = Easing::InOutBack(waitModeRotY, attackModeRotY, time);
}

void BossMainBody::ChangeWaitMode(const float time)
{
	//180度回転させて反対向きにする
	rotation.y = Easing::InOutBack(attackModeRotY, waitModeRotY, time);
}

Vector3 BossMainBody::GetWorldPos()
{
	//ワールド座標を入れる変数
	Vector3 worldPos;
	//平行移動成分を取得
	worldPos.x = matWorld.r[3].m128_f32[0];
	worldPos.y = matWorld.r[3].m128_f32[1];
	worldPos.z = matWorld.r[3].m128_f32[2];

	return worldPos;
}
