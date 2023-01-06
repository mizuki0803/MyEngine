#include "Galaxy.h"
#include "BasePlayer.h"
#include "EnemyDefeatCounter.h"
#include "ParticleEmitter.h"

BasePlayer* Galaxy::player = nullptr;
const float Galaxy::appearModeTime = 600.0f;
const float Galaxy::waitModeTime = 500.0f;

Galaxy* Galaxy::Create(const Vector3& position)
{
	//ギャラクシーのインスタンスを生成
	Galaxy* galaxy = new Galaxy();
	if (galaxy == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!galaxy->Initialize(position)) {
		delete galaxy;
		assert(0);
		return nullptr;
	}

	return galaxy;
}

bool Galaxy::Initialize(const Vector3& position)
{
	//胴体生成
	body.reset(GalaxyBody::Create(position));

	//HPセット
	int maxHP = 1;
	HP = maxHP;

	//ビヘイビアツリー生成
	behaviorTree.reset(GalaxyBehaviorTree::Create(this));

	return true;
}

void Galaxy::Update()
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

void Galaxy::Draw()
{
	//描画
	body->Draw();//胴体
}

void Galaxy::DrawUI()
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

void Galaxy::OnCollisionBody(const int damageNum, const Vector3& collisionPos, const Vector3& subjectVel)
{
	//胴体にダメージ
	body->Damage(damageNum, collisionPos, subjectVel);

	//ギャラクシーにダメージ(実際に喰らったダメージ量をセット)
	const int mainBodyDamageNum = body->GetDamageNum();
	Damage(mainBodyDamageNum);
}

bool Galaxy::AppearModeCount()
{
	//登場状態でなければ抜ける
	if (!(phase == Phase::Appear)) { return false; }

	//タイマーを更新
	appearModeTimer++;

	//指定した時間になったらボス名表示UI生成
	const float bossNameUICreateTime = 160;
	if (appearModeTimer >= bossNameUICreateTime && !bossNameUI) {
		bossNameUI.reset(BossNameUI::Create(1));
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

bool Galaxy::AppearFall()
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

bool Galaxy::WaitMode()
{
	//待機状態でなければ抜ける
	if (!(phase == Phase::Wait)) { return false; }

	return true;
}

bool Galaxy::DeadExplosion()
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

void Galaxy::Damage(const int damageNum)
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
