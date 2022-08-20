#include "Boss.h"
#include "Input.h"
#include "DebugText.h"
#include "BossAvatarType01.h"
#include "BossAvatarType02.h"
#include "BossAvatarType03.h"
#include "BossAvatarType04.h"

Player* Boss::player = nullptr;
const float Boss::attackModeTime = 600;
const float Boss::waitModeTime = 600;

Boss* Boss::Create(ObjModel* bodyModel, ObjModel* headModel, const Vector3& position)
{
	//ボスのインスタンスを生成
	Boss* boss = new Boss();
	if (boss == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!boss->Initialize(bodyModel, headModel, position)) {
		delete boss;
		assert(0);
		return nullptr;
	}

	return boss;
}

bool Boss::Initialize(ObjModel* bodyModel, ObjModel* headModel, const Vector3& position)
{
	//本体生成
	mainBody.reset(BossMainBody::Create(bodyModel, position));
	//ボス分身生成
	std::unique_ptr<BossAvatarType01> newAvatarType01;
	newAvatarType01.reset(BossAvatarType01::Create(headModel, mainBody.get(), { 5, 0, 0 }));
	avatars.push_back(std::move(newAvatarType01));
	std::unique_ptr<BossAvatarType02> newAvatarType02;
	newAvatarType02.reset(BossAvatarType02::Create(headModel, mainBody.get(), { -5, 0, 0 }));
	avatars.push_back(std::move(newAvatarType02));
	std::unique_ptr<BossAvatarType03> newAvatarType03;
	newAvatarType03.reset(BossAvatarType03::Create(headModel, mainBody.get(), { 0, 5, 0 }));
	avatars.push_back(std::move(newAvatarType03));
	std::unique_ptr<BossAvatarType04> newAvatarType04;
	newAvatarType04.reset(BossAvatarType04::Create(headModel, mainBody.get(), { 0, -5, 0 }));
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

void Boss::OnCollisionAvator(BossAvatar* avatar, const int damageNum)
{
	//衝突した分身にダメージ
	avatar->Damage(damageNum);

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

bool Boss::AttackMode()
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

		//攻撃状態を終えるのでfalseを返す
		return false;
	}

	return true;
}

bool Boss::ChangeRotAttackMode()
{
	//モードチェンジ回転に要する時間
	const float changeModeTime = 60;
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

bool Boss::WaitMode()
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

bool Boss::ChangeRotWaitMode()
{
	//モードチェンジ回転に要する時間
	const float changeModeTime = 60;
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
