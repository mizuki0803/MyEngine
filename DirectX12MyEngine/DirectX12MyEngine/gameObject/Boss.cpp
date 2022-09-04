#include "Boss.h"
#include "Input.h"
#include "DebugText.h"
#include "Player.h"
#include "BossAvatarType01.h"
#include "BossAvatarType02.h"
#include "BossAvatarType03.h"
#include "BossAvatarType04.h"

Player* Boss::player = nullptr;
const float Boss::attackModeTime = 600.0f;
const float Boss::waitModeTime = 480.0f;
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

	//HPバー生成
	const Vector2 hpBarPosition = { 20, 120 };
	hpBar.reset(BossHPBar::Create(4, hpBarPosition, maxHP));
	//HPバーフレーム生成
	const float posDiff = 3.0f;	//HPバーの座標との差分
	const Vector2 hpFramePosition = { hpBarPosition.x - posDiff, hpBarPosition.y - posDiff };
	hpFrame.reset(BossHPFrame::Create(5, hpFramePosition));

	//ビヘイビアツリー生成
	behaviorTree.reset(BossBehaviorTree::Create(this));

	return true;
}

void Boss::Update()
{
	//死亡した分身の削除
	avatars.remove_if([](std::unique_ptr<BossAvatar>& avatar) {
		return avatar->GetIsDead();
		});

	//ビヘイビアツリーによる行動遷移
	behaviorTree->Root();

	//更新
	mainBody->Update();//本体
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		avatar->Update();//分身
	}

	//HPバー更新
	hpBar->Update();
	//HPバーフレーム更新
	hpFrame->Update();

	if (phase == Phase::Dead) {
		DebugText::GetInstance()->Print("BossDeadMode", 100, 60);
	}

	std::string hpNum = std::to_string(HP);
	DebugText::GetInstance()->Print("HP : " + hpNum, 100, 80);
	std::string avatarNum = std::to_string(avatars.size());
	DebugText::GetInstance()->Print("AvatarNum : " + avatarNum, 100, 100);
}

void Boss::Draw()
{
	//描画
	mainBody->Draw();//本体
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		avatar->Draw();//分身
	}
}

void Boss::DrawUI()
{
	//HPバーフレーム描画
	hpFrame->Draw();
	//HPバー描画
	hpBar->Draw();
}

void Boss::OnCollisionMainBody(const int damageNum)
{
	//待機状態ならダメージを喰らう
	const bool isWaitMode = (phase == Phase::Wait);
	//分身が全て死んでいるならダメージを喰らう
	const bool isAllAvatarDead = (avatars.size() == 0);
	//y軸角度45度以下ならダメージを喰らう
	const bool isDamageRota = (mainBody->GetRotation().y <= 45.0f);

	//どれか一つでも項目を達成していなければダメージを受けないで抜ける
	if (!(isWaitMode && isAllAvatarDead && isDamageRota)) { return; }

	//本体にダメージ
	mainBody->Damage(damageNum);

	//ボス全体にダメージ(ボス本体が実際に喰らったダメージ量をセット)
	const int mainBodyDamageNum = mainBody->GetDamageNum();
	Damage(mainBodyDamageNum);
}

void Boss::OnCollisionAvatar(BossAvatar* avatar, const int damageNum)
{
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

bool Boss::FallMode()
{
	//降下状態でなければ抜ける
	if (!(phase == Phase::Fall)) { return false; }

	//タイマーを更新
	const float fallModeTime = 300;
	fallModeTimer++;
	const float time = fallModeTimer / fallModeTime;

	//ボス本体を降下させる
	mainBody->FallMode(time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (fallModeTimer >= fallModeTime) {
		phase = Phase::Attack;

		//分身のモデルを起きている状態のモデルに変更する
		for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
			avatar->ChangeModel();
		}
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
		avatar->ChangeWaitMode(time);
	}

	return true;
}

bool Boss::DeadMode()
{
	//死亡状態でなければ抜ける
	if (!(phase == Phase::Dead)) { return false; }

	//タイマーを更新
	const float deadModeTime = 300;
	deadModeTimer++;

	//ボス本体に死亡時の動きをさせる
	mainBody->DeadMode();

	//タイマーが指定した時間になったら死亡フラグを立てる(削除)
	if (deadModeTimer >= deadModeTime) {
		isDead = true;
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
	hpBar->ChangeLength(HP);
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
		avatar->AttackEnd();
	}
}
