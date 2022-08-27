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
const float Boss::waitModeTime = 600.0f;
const float Boss::changeModeTime = 60.0f;
const float Boss::returnFixedPositionTime = 180.0f;

Boss* Boss::Create(ObjModel* mainBodyModel, ObjModel* avatarModel, const Vector3& position)
{
	//ボスのインスタンスを生成
	Boss* boss = new Boss();
	if (boss == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!boss->Initialize(mainBodyModel, avatarModel, position)) {
		delete boss;
		assert(0);
		return nullptr;
	}

	return boss;
}

bool Boss::Initialize(ObjModel* mainBodyModel, ObjModel* avatarModel, const Vector3& position)
{
	//本体生成
	mainBody.reset(BossMainBody::Create(mainBodyModel, position));
	//ボス分身生成
	std::unique_ptr<BossAvatarType01> newAvatarType01;
	newAvatarType01.reset(BossAvatarType01::Create(avatarModel, mainBody.get(), { 5, 0, 0 }));
	avatars.push_back(std::move(newAvatarType01));
	std::unique_ptr<BossAvatarType02> newAvatarType02;
	newAvatarType02.reset(BossAvatarType02::Create(avatarModel, mainBody.get(), { -5, 0, 0 }));
	avatars.push_back(std::move(newAvatarType02));
	std::unique_ptr<BossAvatarType03> newAvatarType03;
	newAvatarType03.reset(BossAvatarType03::Create(avatarModel, mainBody.get(), { 0, 5, 0 }));
	avatars.push_back(std::move(newAvatarType03));
	std::unique_ptr<BossAvatarType04> newAvatarType04;
	newAvatarType04.reset(BossAvatarType04::Create(avatarModel, mainBody.get(), { 0, -5, 0 }));
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

	if (isDead) {
		DebugText::GetInstance()->Print("BossDead", 100, 60);
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
	//分身が全て死んでいない限りダメージを受けない
	if (!(avatars.size() == 0)) { return; }

	//本体にダメージ
	mainBody->Damage(damageNum);

	//ボス全体にダメージ
	Damage(damageNum);
}

void Boss::OnCollisionAvatar(BossAvatar* avatar, const int damageNum)
{
	//衝突した分身にダメージ
	avatar->Damage(damageNum);

	//分身が全滅したかチェック
	CheckAllAvatarDead();

	//ボス全体にダメージ
	Damage(damageNum);
}

bool Boss::Fall()
{
	//降下状態でなければ抜ける
	if (!(phase == Phase::Fall)) { return false; }

	//タイマーを更新
	const float fallTime = 300;
	fallTimer++;
	const float time = fallTimer / fallTime;

	//ボス本体を降下させる
	mainBody->Fall(time);

	//タイマーが指定した時間になったら次のフェーズへ
	if (fallTimer >= fallTime) {
		phase = Phase::Attack;
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

bool Boss::AttackTypeASelect()
{
	//本体が攻撃状態でなければ抜ける
	if (!isMainBodyAttackMode) { return false; }

	//前回の攻撃内容がAだったら抜ける
	if (preAttackType == AttackType::A) { return false; }

	//攻撃内容Aをセット
	attackType = AttackType::A;
	//1つ前の攻撃内容を更新
	preAttackType = AttackType::A;

	return true;
}

bool Boss::AttackTypeBSelect()
{
	//本体が攻撃状態でなら抜ける
	if (isMainBodyAttackMode) { return false; }

	//前回の攻撃内容がBだったら抜ける
	if (preAttackType == AttackType::B) { return false; }

	//プレイヤー自機が画面右側にいたら抜ける
	if (player->GetPosition().x <= 0) { return false; }

	//攻撃内容Bをセット
	attackType = AttackType::B;
	//1つ前の攻撃内容を更新
	preAttackType = AttackType::B;

	return true;
}

bool Boss::AttackTypeCSelect()
{
	//本体が攻撃状態でなら抜ける
	if (isMainBodyAttackMode) { return false; }

	//前回の攻撃内容がCだったら抜ける
	if (preAttackType == AttackType::C) { return false; }

	//攻撃内容Cをセット
	attackType = AttackType::C;
	//1つ前の攻撃内容を更新
	preAttackType = AttackType::C;

	return true;
}

bool Boss::AttackTypeDSelect()
{
	//本体が攻撃状態でなら抜ける
	if (isMainBodyAttackMode) { return false; }

	//前回の攻撃内容がDだったら抜ける
	if (preAttackType == AttackType::D) { return false; }

	//攻撃内容Cをセット
	attackType = AttackType::D;
	//1つ前の攻撃内容を更新
	preAttackType = AttackType::D;

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

bool Boss::AttackTypeA()
{
	//攻撃内容がAでなければ抜ける
	if (!(attackType == AttackType::A)) { return false; }

	mainBody->AttackTypeA(player->GetWorldPos());

	return true;
}

bool Boss::AttackTypeB()
{
	//攻撃内容がBでなければ抜ける
	if (!(attackType == AttackType::B)) { return false; }

	//本体と分身を攻撃内容Bで動かす
	mainBody->AttackTypeB();
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		avatar->AttackTypeB(player->GetWorldPos());
	}

	return true;
}

bool Boss::AttackTypeC()
{
	//攻撃内容がCでなければ抜ける
	if (!(attackType == AttackType::C)) { return false; }

	//本体と分身を攻撃内容Cで動かす
	mainBody->AttackTypeC(player->GetWorldPos());
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		avatar->AttackTypeC();
	}

	return true;
}

bool Boss::AttackTypeD()
{
	//攻撃内容がDでなければ抜ける
	if (!(attackType == AttackType::D)) { return false; }

	//本体と分身を攻撃内容Dで動かす
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		avatar->AttackTypeD();
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

bool Boss::ReturnFixedPosition()
{
	//タイマーが固定位置に戻るために要する時間以下でないなら抜ける
	const float timer = waitModeTimer - changeModeTime;
	if (!(timer < returnFixedPositionTime)) { return false; }

	//イージング用に0～1の値を算出する
	const float time = timer / returnFixedPositionTime;

	//固定位置に戻す
	mainBody->ReturnFixedPosition(time);
	for (const std::unique_ptr<BossAvatar>& avatar : avatars) {
		avatar->ReturnFixedPosition(time);
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

void Boss::Damage(const int damageNum)
{
	//引数の数字の分ダメージを喰らう
	HP -= damageNum;

	//HPが0以下になったら死亡
	if (HP <= 0) {
		HP = 0;

		isDead = true;
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
