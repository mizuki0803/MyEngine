#include "Boss2.h"
#include "BasePlayer.h"
#include "EnemyDefeatCounter.h"
#include "ParticleEmitter.h"

BasePlayer* Boss2::player = nullptr;
const float Boss2::appearModeTime = 600.0f;
const float Boss2::waitModeTime = 500.0f;

Boss2* Boss2::Create(const Vector3& position)
{
	//ボス2のインスタンスを生成
	Boss2* boss2 = new Boss2();
	if (boss2 == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!boss2->Initialize(position)) {
		delete boss2;
		assert(0);
		return nullptr;
	}

	return boss2;
}

bool Boss2::Initialize(const Vector3& position)
{
	//胴体生成
	body.reset(Boss2Body::Create(position));

	//HPセット
	int maxHP = 1;
	HP = maxHP;

	//ビヘイビアツリー生成
	behaviorTree.reset(Boss2BehaviorTree::Create(this));

	return true;
}

void Boss2::Update()
{
	//更新
	body->Update();//胴体

	//ビヘイビアツリーによる行動遷移
	behaviorTree->Root();

	//HPUI更新
	if (hpUI) {
		hpUI->Update();
	}
	//ボス名表示UI更新
	if (bossNameUI) {
		bossNameUI->Update();
	}
}

void Boss2::Draw()
{
	//描画
	body->Draw();//胴体
}

void Boss2::DrawUI()
{
	//HPUI描画
	if (hpUI) {
		hpUI->Draw();
	}
	//ボス名表示UI描画
	if (bossNameUI) {
		bossNameUI->Draw();
	}
}

void Boss2::OnCollisionBody(const int damageNum, const Vector3& collisionPos, const Vector3& subjectVel)
{
	//胴体にダメージ
	body->Damage(damageNum, collisionPos, subjectVel);

	//ボス2にダメージ(実際に喰らったダメージ量をセット)
	const int mainBodyDamageNum = body->GetDamageNum();
	Damage(mainBodyDamageNum);
}

bool Boss2::AppearModeCount()
{
	//登場状態でなければ抜ける
	if (!(phase == Phase::Appear)) { return false; }

	//タイマーを更新
	appearModeTimer++;

	//指定した時間になったらボス名表示UI生成
	const float bossNameUICreateTime = 160;
	if (appearModeTimer >= bossNameUICreateTime && !bossNameUI) {
		bossNameUI.reset(BossNameUI::Create(0));
	}

	//指定した時間になったらHPバー生成
	const float hpUICreateTime = appearModeTime - 100;
	if (appearModeTimer >= hpUICreateTime && !hpUI) {
		const Vector2 hpUIPosition = { 30, 170 };
		hpUI.reset(BossHPUI::Create(hpUIPosition, HP));
	}

	//タイマーが指定した時間になったら次のフェーズへ
	if (appearModeTimer >= appearModeTime) {
		phase = Phase::Wait;

		//ボス名表示UIはもう使用しないので解放しておく
		bossNameUI.reset();
	}

	return true;
}

bool Boss2::AppearFall()
{
	//降下にかかる時間
	const float fallTime = 400;
	//既に降下にかかる時間以上なら抜ける
	if (appearModeTimer > fallTime) { return false; }

	//イージング用に0～1の値を算出する
	const float time = appearModeTimer / fallTime;

	//胴体を降下させる
	body->Fall(time);

	return true;
}

bool Boss2::WaitMode()
{
	//待機状態でなければ抜ける
	if (!(phase == Phase::Wait)) { return false; }

	return true;
}

bool Boss2::DeadExplosion()
{
	//死亡状態でなければ抜ける
	if (!(phase == Phase::Dead)) { return false; }

	//死亡状態の時間
	const int deadModeTime = 480;
	//タイマー更新
	deadModeTimer++;

	//胴体の死亡行動
	body->Dead();

	//一定間隔で爆発
	const int explosionInterval = 20;
	if ((deadModeTimer % (int)explosionInterval) == 0) {
		//爆発演出用パーティクル生成
		Vector3 particlePos = body->GetWorldPos();
		const float distance = 8.0f;
		particlePos.x += (float)rand() / RAND_MAX * distance - distance / 2.0f;
		particlePos.y += (float)rand() / RAND_MAX * distance - distance / 2.0f;
		particlePos.z += (float)rand() / RAND_MAX * distance - distance / 2.0f;
		const float size = 2.0f;
		ParticleEmitter::GetInstance()->Explosion(particlePos, size);
	}

	//タイマーが指定した時間になったら削除
	if (deadModeTimer >= deadModeTime) {
		isDelete = true;
	}

	return true;
}

void Boss2::Damage(const int damageNum)
{
	//引数の数字の分ダメージを喰らう
	HP -= damageNum;

	//HPが0以下になったら死亡状態にする
	if (HP <= 0) {
		HP = 0;

		phase = Phase::Dead;
		isDead = true;
	}

	//ダメージを喰らったのでHPバーの長さを変更する
	if (hpUI) {
		hpUI->Damage(HP);
	}
}
