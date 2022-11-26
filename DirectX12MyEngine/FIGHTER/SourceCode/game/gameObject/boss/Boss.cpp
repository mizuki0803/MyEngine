#include "Boss.h"
#include "Input.h"
#include "SpriteTextureLoader.h"
#include "Player.h"
#include "BossAvatarType01.h"
#include "BossAvatarType02.h"
#include "BossAvatarType03.h"
#include "BossAvatarType04.h"
#include "EnemyDefeatCounter.h"
#include "ParticleEmitter.h"

Player* Boss::player = nullptr;
const float Boss::appearModeTime = 560.0f;
const float Boss::attackModeTime = 600.0f;
const float Boss::waitModeTime = 500.0f;
const float Boss::changeModeTime = 60.0f;
const float Boss::returnBasePositionTime = 180.0f;

Boss* Boss::Create(const Vector3& position)
{
	//ボスのインスタンスを生成
	Boss* boss = new Boss();
	if (boss == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!boss->Initialize(position)) {
		delete boss;
		assert(0);
		return nullptr;
	}

	return boss;
}

bool Boss::Initialize(const Vector3& position)
{
	//本体生成
	mainBody.reset(BossMainBody::Create(position));
	//ボス分身生成
	std::unique_ptr<BossAvatarType01> newAvatarType01;
	newAvatarType01.reset(BossAvatarType01::Create(mainBody.get(), { 2, 0, 0 }));
	avatars.push_back(std::move(newAvatarType01));
	std::unique_ptr<BossAvatarType02> newAvatarType02;
	newAvatarType02.reset(BossAvatarType02::Create(mainBody.get(), { -2, 0, 0 }));
	avatars.push_back(std::move(newAvatarType02));
	std::unique_ptr<BossAvatarType03> newAvatarType03;
	newAvatarType03.reset(BossAvatarType03::Create(mainBody.get(), { 0, 2, 0 }));
	avatars.push_back(std::move(newAvatarType03));
	std::unique_ptr<BossAvatarType04> newAvatarType04;
	newAvatarType04.reset(BossAvatarType04::Create(mainBody.get(), { 0, -2, 0 }));
	avatars.push_back(std::move(newAvatarType04));

	//HPを本体とボス分身の合計で算出する
	int maxHP = BossMainBody::GetMaxHP() + BossAvatar::GetMaxHP() * (int)avatars.size();
	HP = maxHP;

	//ビヘイビアツリー生成
	behaviorTree.reset(BossBehaviorTree::Create(this));

	return true;
}

void Boss::Update()
{
	//死亡した分身の削除
	avatars.remove_if([](std::unique_ptr<BossAvatar>& avatar) {
		return avatar->GetIsDelete();
		});

	//ビヘイビアツリーによる行動遷移
	behaviorTree->Root();

	//更新
	mainBody->Update();//本体
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		avatar->Update();//分身
	}

	//HPUI更新
	if (hpUI) {
		hpUI->Update();
	}
	//ボス名表示UI更新
	if (bossNameUI) {
		bossNameUI->Update();
	}

}

void Boss::Draw()
{
	//描画
	mainBody->Draw();//本体
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		avatar->Draw();//分身
	}
}

void Boss::DrawLightCameraView()
{
	//影用光源ライトから見た視点での描画
	mainBody->DrawLightCameraView();//本体
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		avatar->DrawLightCameraView();//分身
	}
}

void Boss::DrawUI()
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

void Boss::OnCollisionMainBody(const int damageNum)
{
	//待機状態ならダメージを喰らう
	const bool isWaitMode = (phase == Phase::Wait);
	//y軸角度45度以下ならダメージを喰らう
	const bool isDamageRota = (mainBody->GetRotation().y <= 45.0f);

	//本体が攻撃状態ならダメージを喰らうを追加し、どれか一つでも項目を達成していなければダメージを受けないで抜ける
	if (!(isMainBodyAttackMode && isWaitMode && isDamageRota)) { return; }

	//本体にダメージ
	mainBody->Damage(damageNum);

	//ボス全体にダメージ(ボス本体が実際に喰らったダメージ量をセット)
	const int mainBodyDamageNum = mainBody->GetDamageNum();
	Damage(mainBodyDamageNum);
}

void Boss::OnCollisionAvatar(BossAvatar* avatar, const int damageNum)
{
	//分身が既に死亡していたら抜ける
	if (avatar->GetIsDead()) { return; }

	//待機状態ならダメージを喰らう
	const bool isWaitMode = (phase == Phase::Wait);
	//y軸角度45度以下ならダメージを喰らう
	const bool isDamageRota = (avatar->GetRotation().y <= 45.0f);

	//どちらか一つでも項目を達成していなければダメージを受けないで抜ける
	if (!(isWaitMode && isDamageRota)) { return; }

	//衝突した分身にダメージ
	avatar->Damage(damageNum);

	//分身が全滅したかチェック
	CheckAllAvatarDead();

	//ボス全体にダメージ(ボス分身が実際に喰らったダメージ量をセット)
	const int avatarBodyDamageNum = avatar->GetDamageNum();
	Damage(avatarBodyDamageNum);
}

bool Boss::AppearModeCount()
{
	//登場状態でなければ抜ける
	if (!(phase == Phase::Appear)) { return false; }

	//タイマーを更新
	appearModeTimer++;

	//指定した時間になったらボス名表示UI生成
	const float bossNameUICreateTime = 150;
	if (appearModeTimer >= bossNameUICreateTime && !bossNameUI) {
		bossNameUI.reset(BossNameUI::Create(0));
	}

	//指定した時間になったらHPバー生成
	const float hpUICreateTime = appearModeTime - 90;
	if (appearModeTimer >= hpUICreateTime && !hpUI) {
		const Vector2 hpUIPosition = { 30, 150 };
		hpUI.reset(BossHPUI::Create(hpUIPosition, HP));
	}

	//タイマーが指定した時間になったら次のフェーズへ
	if (appearModeTimer >= appearModeTime) {
		phase = Phase::Attack;

		//分身のモデルを起きている状態のモデルに変更する
		for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
			avatar->ChangeModel();
		}

		//ボス名表示UIはもう使用しないので解放しておく
		bossNameUI.reset();
	}

	return true;
}

bool Boss::AppearFall()
{
	//降下にかかる時間
	const float fallTime = 400;
	//既に降下にかかる時間以上なら抜ける
	if (appearModeTimer > fallTime) { return false; }

	//イージング用に0～1の値を算出する
	const float time = appearModeTimer / fallTime;

	//ボス本体を降下させる
	mainBody->Fall(time);

	return true;
}

bool Boss::AttackModeCount()
{
	//攻撃状態でなければ抜ける
	if (!(phase == Phase::Attack)) { return false; }

	//タイマーを更新
	attackModeTimer++;

	//タイマーが指定した時間になったら次のフェーズへ
	if (attackModeTimer >= attackModeTime) {
		phase = Phase::Wait;

		//次に攻撃状態になったときのためにタイマーを初期化しておく
		attackModeTimer = 0;
		//攻撃内容を未設定にしておく
		attackType = AttackType::None;

		//攻撃状態終了するので必要な情報をセット
		AttackEnd();

		//攻撃状態を終えるのでfalseを返す
		return false;
	}

	return true;
}

bool Boss::UpdateBulletShotPos()
{
	//攻撃状態でなければ抜ける
	if (!(phase == Phase::Attack)) { return false; }

	//本体と分身の発射座標を更新
	mainBody->UpdateBulletShotPos();
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		//分身が既に死亡していたら飛ばす
		if (avatar->GetIsDead()) { continue; }

		avatar->UpdateBulletShotPos();
	}

	return true;
}

bool Boss::AttackTypeSelectStart()
{
	//攻撃内容が既に決まっていたらtrueを返す
	if (!(attackType == AttackType::None)) { return true; }

	//攻撃内容設定を開始するためfalseを返す
	return false;
}

bool Boss::AttackTypeTrackingSelect()
{
	//本体が攻撃状態でなければ抜ける
	if (!isMainBodyAttackMode) { return false; }

	//前回の攻撃内容が追従だったら抜ける
	if (preAttackType == AttackType::Tracking) { return false; }

	//プレイヤー自機が画面左側にいたら抜ける
	if (player->GetPosition().x <= 0) { return false; }

	//攻撃内容:追従をセット
	attackType = AttackType::Tracking;
	//1つ前の攻撃内容を更新
	preAttackType = AttackType::Tracking;

	return true;
}

bool Boss::AttackTypeSuperGiantBulletSelect()
{
	//本体が攻撃状態でなければ抜ける
	if (!isMainBodyAttackMode) { return false; }

	//前回の攻撃内容が超巨大弾だったら抜ける
	if (preAttackType == AttackType::SuperGiantBullet) { return false; }

	//攻撃内容:超巨大弾をセット
	attackType = AttackType::SuperGiantBullet;
	//1つ前の攻撃内容を更新
	preAttackType = AttackType::SuperGiantBullet;

	return true;
}

bool Boss::AttackTypeRotateSelect()
{
	//本体が攻撃状態でなければ抜ける
	if (!isMainBodyAttackMode) { return false; }

	//前回の攻撃内容が回転だったら抜ける
	if (preAttackType == AttackType::Rotate) { return false; }

	//攻撃内容:回転をセット
	attackType = AttackType::Rotate;
	//1つ前の攻撃内容を更新
	preAttackType = AttackType::Rotate;

	return true;
}

bool Boss::AttackTypeAvatarBodyBlowSelect()
{
	//本体が攻撃状態でなら抜ける
	if (isMainBodyAttackMode) { return false; }

	//前回の攻撃内容が分身体当たりだったら抜ける
	if (preAttackType == AttackType::AvatarBodyBlow) { return false; }

	//プレイヤー自機が画面右側にいたら抜ける
	if (player->GetPosition().x <= 0) { return false; }

	//攻撃内容:分身体当たりをセット
	attackType = AttackType::AvatarBodyBlow;
	//1つ前の攻撃内容を更新
	preAttackType = AttackType::AvatarBodyBlow;

	return true;
}

bool Boss::AttackTypeAvatarGatlingSelect()
{
	//本体が攻撃状態でなら抜ける
	if (isMainBodyAttackMode) { return false; }

	//前回の攻撃内容が分身ガトリング砲だったら抜ける
	if (preAttackType == AttackType::AvatarGatling) { return false; }

	//攻撃内容:分身ガトリング砲をセット
	attackType = AttackType::AvatarGatling;
	//1つ前の攻撃内容を更新
	preAttackType = AttackType::AvatarGatling;

	return true;
}

bool Boss::AttackTypeAvatarGiantBulletSelect()
{
	//本体が攻撃状態でなら抜ける
	if (isMainBodyAttackMode) { return false; }

	//前回の攻撃内容が分身巨大弾だったら抜ける
	if (preAttackType == AttackType::AvatarGiantBullet) { return false; }

	//攻撃内容:分身巨大弾をセット
	attackType = AttackType::AvatarGiantBullet;
	//1つ前の攻撃内容を更新
	preAttackType = AttackType::AvatarGiantBullet;

	return true;
}

bool Boss::AttackTypeTracking()
{
	//攻撃内容が追従でなければ抜ける
	if (!(attackType == AttackType::Tracking)) { return false; }

	mainBody->AttackTypeTracking(player->GetWorldPos());

	return true;
}

bool Boss::AttackTypeSuperGiantBullet()
{
	//攻撃内容が超巨大弾でなければ抜ける
	if (!(attackType == AttackType::SuperGiantBullet)) { return false; }

	mainBody->AttackTypeSuperGiantBullet();

	return true;
}

bool Boss::AttackTypeRotate()
{
	//攻撃内容が回転でなければ抜ける
	if (!(attackType == AttackType::Rotate)) { return false; }

	mainBody->AttackTypeRotate();

	return true;
}

bool Boss::AttackTypeAvatarBodyBlow()
{
	//攻撃内容が分身体当たりでなければ抜ける
	if (!(attackType == AttackType::AvatarBodyBlow)) { return false; }

	//本体と分身を攻撃内容:分身体当たりで動かす
	mainBody->AttackTypeAvatarBodyBlow();
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		//分身が既に死亡していたら飛ばす
		if (avatar->GetIsDead()) { continue; }

		avatar->AttackTypeAvatarBodyBlow(player->GetWorldPos());
	}

	return true;
}

bool Boss::AttackTypeAvatarGatling()
{
	//攻撃内容が分身ガトリング砲でなければ抜ける
	if (!(attackType == AttackType::AvatarGatling)) { return false; }

	//本体と分身を攻撃内容:分身ガトリング砲で動かす
	mainBody->AttackTypeAvatarGatling(player->GetWorldPos());
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		//分身が既に死亡していたら飛ばす
		if (avatar->GetIsDead()) { continue; }

		avatar->AttackTypeAvatarGatling();
	}

	return true;
}

bool Boss::AttackTypeAvatarGiantBullet()
{
	//攻撃内容が分身巨大弾でなければ抜ける
	if (!(attackType == AttackType::AvatarGiantBullet)) { return false; }

	//本体と分身を攻撃内容:分身巨大弾で動かす
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		//分身が既に死亡していたら飛ばす
		if (avatar->GetIsDead()) { continue; }

		avatar->AttackTypeAvatarGiantBullet();
	}

	return true;
}

bool Boss::AttackModeMainBodyRota()
{
	//本体が攻撃状態でなければ抜ける
	if (!isMainBodyAttackMode) { return false; }

	//タイマーがモードチェンジ時間以下でないなら抜ける
	if (!(attackModeTimer < changeModeTime)) { return false; }

	//イージング用に0～1の値を算出する
	const float time = attackModeTimer / changeModeTime;

	//攻撃状態にするため本体を回転させる
	mainBody->ChangeAttackMode(time);

	return true;
}

bool Boss::AttackModeAvatarRota()
{
	//本体が攻撃状態でなら抜ける
	if (isMainBodyAttackMode) { return false; }

	//タイマーがモードチェンジ時間以下でないなら抜ける
	if (!(attackModeTimer < changeModeTime)) { return false; }

	//イージング用に0～1の値を算出する
	const float time = attackModeTimer / changeModeTime;

	//攻撃状態にするため分身を回転させる
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		//分身が既に死亡していたら飛ばす
		if (avatar->GetIsDead()) { continue; }

		avatar->ChangeAttackMode(time);
	}

	return true;
}

bool Boss::WaitModeCount()
{
	//待機状態でなければ抜ける
	if (!(phase == Phase::Wait)) { return false; }

	//タイマーを更新
	waitModeTimer++;

	//タイマーが指定した時間になったら次のフェーズへ
	if (waitModeTimer >= waitModeTime) {
		phase = Phase::Attack;

		//次に待機状態になったときのためにタイマーを初期化しておく
		waitModeTimer = 0;

		//待機状態を終えるのでfalseを返す
		return false;
	}

	return true;
}

bool Boss::ReturnBasePosition()
{
	//タイマーが基準位置に戻るために要する時間以下でないなら抜ける
	const float timer = waitModeTimer - changeModeTime;
	if (!(timer < returnBasePositionTime)) { return false; }

	//イージング用に0～1の値を算出する
	const float time = timer / returnBasePositionTime;

	//基準位置に戻す
	mainBody->ReturnBasePosition(time);
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		//分身が既に死亡していたら飛ばす
		if (avatar->GetIsDead()) { continue; }

		avatar->ReturnBasePosition(time);
	}

	return true;
}

bool Boss::WaitModeMainBodyRota()
{
	//本体が攻撃状態でなければ抜ける
	if (!isMainBodyAttackMode) { return false; }

	//タイマーがモードチェンジ時間以下でないなら抜ける
	if (!(waitModeTimer < changeModeTime)) { return false; }

	//イージング用に0～1の値を算出する
	const float time = waitModeTimer / changeModeTime;

	//待機状態にするため本体を回転させる
	mainBody->ChangeWaitMode(time);

	return true;
}

bool Boss::WaitModeAvatarRota()
{
	//本体が攻撃状態でなら抜ける
	if (isMainBodyAttackMode) { return false; }

	//タイマーがモードチェンジ時間以下でないなら抜ける
	if (!(waitModeTimer < changeModeTime)) { return false; }

	//イージング用に0～1の値を算出する
	const float time = waitModeTimer / changeModeTime;

	//待機状態にするため分身を回転させる
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		//分身が既に死亡していたら飛ばす
		if (avatar->GetIsDead()) { continue; }

		avatar->ChangeWaitMode(time);
	}

	return true;
}

bool Boss::DeadExplosion()
{
	//死亡状態でなければ抜ける
	if (!(phase == Phase::Dead)) { return false; }
	//既に爆発回数が十分なら抜ける
	const int explosionNum = 12;
	if (explosionCount >= explosionNum) { return false; }

	//爆発演出発生タイマー更新
	const float explosionTime = 20;
	explosionTimer++;
	if ((explosionTimer % (int)explosionTime) == 0) {
		//爆発演出用パーティクル生成
		Vector3 particlePos = mainBody->GetWorldPos();
		const float distance = 5.0f;
		particlePos.x += (float)rand() / RAND_MAX * distance - distance / 2.0f;
		particlePos.y += (float)rand() / RAND_MAX * distance - distance / 2.0f;
		particlePos.z += (float)rand() / RAND_MAX * distance - distance / 2.0f;
		ParticleEmitter::GetInstance()->Explosion(particlePos);

		//爆発演出回数更新
		explosionCount++;

		//爆発が指定回数に達したら死亡フラグを立てる
		if (explosionCount >= explosionNum) {
			isDead = true;

			//倒した数カウンターを増やす(本体 + 分身数)
			const int defeatNum = 5;
			EnemyDefeatCounter::AddCounter(defeatNum);
		}
	}

	//イージング用に0～1の値を算出する
	const float backTime = 180;
	float time = explosionTimer / backTime;
	time = min(time, 1.0f);
	//ボス本体を基準の位置に戻す
	mainBody->DeadBasePosBack(time);

	return true;
}

bool Boss::DeadFall()
{
	//死亡状態でなければ抜ける
	if (!(phase == Phase::Dead)) { return false; }

	//ボス本体に死亡時落下をさせる
	mainBody->DeadFall();

	//本体が削除状態なら削除
	if (mainBody->GetIsDelete()) {
		isDelete = true;

		//爆発演出用パーティクル生成
		Vector3 particlePos = mainBody->GetWorldPos();
		const float distance = 5.0f;
		particlePos.x += (float)rand() / RAND_MAX * distance - distance / 2.0f;
		particlePos.y += (float)rand() / RAND_MAX * distance - distance / 2.0f;
		particlePos.z += (float)rand() / RAND_MAX * distance - distance / 2.0f;
		ParticleEmitter::GetInstance()->Explosion(particlePos);
	}

	return true;
}

void Boss::Damage(const int damageNum)
{
	//引数の数字の分ダメージを喰らう
	HP -= damageNum;

	//HPが0以下になったら死亡状態にする
	if (HP <= 0) {
		HP = 0;

		phase = Phase::Dead;
	}

	//ダメージを喰らったのでHPバーの長さを変更する
	if (hpUI) {
		hpUI->Damage(HP);
	}
}

void Boss::CheckAllAvatarDead()
{
	//既に本体が攻撃状態なら抜ける
	if (isMainBodyAttackMode) { return; }

	//一体でも生きていたら抜ける
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		if (!avatar->GetIsDead()) {
			return;
		}
	}

	//本体を攻撃状態にする
	isMainBodyAttackMode = true;
	//本体のモデルを起きている状態のモデルに変更する
	mainBody->ChangeModel();
	//本体は待機中なので、待機状態にしておく
	phase = Phase::Wait;

	//攻撃内容を未設定にしておく
	attackType = AttackType::None;
	//攻撃状態で本体が攻撃する状態になってしまった場合のために攻撃状態タイマーを初期化しておく
	attackModeTimer = 0;
}

void Boss::AttackEnd()
{
	//攻撃状態終了するので必要な情報をセット
	mainBody->AttackEnd();
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		//分身が既に死亡していたら飛ばす
		if (avatar->GetIsDead()) { continue; }

		avatar->AttackEnd();
	}
}
