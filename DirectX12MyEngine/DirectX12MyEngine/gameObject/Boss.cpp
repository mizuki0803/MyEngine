#include "Boss.h"
#include "Easing.h"
#include "Input.h"
#include "DebugText.h"
#include "BossAvatarType01.h"
#include "BossAvatarType02.h"
#include "BossAvatarType03.h"
#include "BossAvatarType04.h"

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

	boss->startPos = position;

	return boss;
}

bool Boss::Initialize(ObjModel* bodyModel, ObjModel* headModel, const Vector3& position)
{
	//本体生成
	bossMainBody.reset(BossMainBody::Create(bodyModel, position));
	//ボス分身生成
	std::unique_ptr<BossAvatarType01> newbossAvatarType01;
	newbossAvatarType01.reset(BossAvatarType01::Create(headModel, bossMainBody.get(), {5, 0, 0}));
	bossAvatars.push_back(std::move(newbossAvatarType01));
	std::unique_ptr<BossAvatarType02> newbossAvatarType02;
	newbossAvatarType02.reset(BossAvatarType02::Create(headModel, bossMainBody.get(), { -5, 0, 0 }));
	bossAvatars.push_back(std::move(newbossAvatarType02));
	std::unique_ptr<BossAvatarType03> newbossAvatarType03;
	newbossAvatarType03.reset(BossAvatarType03::Create(headModel, bossMainBody.get(), { 0, 5, 0 }));
	bossAvatars.push_back(std::move(newbossAvatarType03));
	std::unique_ptr<BossAvatarType04> newbossAvatarType04;
	newbossAvatarType04.reset(BossAvatarType04::Create(headModel, bossMainBody.get(), { 0, -5, 0 }));
	bossAvatars.push_back(std::move(newbossAvatarType04));


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
	bossAvatars.remove_if([](std::unique_ptr<BossAvatar>& bossAvatar) {
		return bossAvatar->GetIsDead();
		});

	//ビヘイビアツリーによる行動遷移
	behaviorTree->Root();

	//更新
	bossMainBody->Update();//本体
	for (const std::unique_ptr<BossAvatar>& bossAvatar : bossAvatars) {
		bossAvatar->Update();//分身
	}

	//HPバー更新
	hpBar->Update();
	//HPバーフレーム更新
	hpFrame->Update();

	std::string avatarNum = std::to_string(bossAvatars.size());
	DebugText::GetInstance()->Print("AvatarNum : " + avatarNum, 100, 100);
}

void Boss::Draw()
{
	//描画
	bossMainBody->Draw();//本体
	for (const std::unique_ptr<BossAvatar>& bossAvatar : bossAvatars) {
		bossAvatar->Draw();//分身
	}
}

void Boss::DrawUI()
{
	//HPバーフレーム描画
	hpFrame->Draw();
	//HPバー描画
	hpBar->Draw();
}

bool Boss::Fall()
{
	//降下状態でなければ抜ける
	if (!(phase == Phase::Fall)) { return false; }

	//イージングで降下する
	const float fallTime = 300;
	fallTimer++;
	const float time = fallTimer / fallTime;

	//生成位置から降りたところで停止する
	Vector3 stayPos = startPos;
	stayPos.y = 0;
	bossMainBody->SetPosition(Easing::Lerp(startPos, stayPos, time));

	//イージングが終了したら次のフェーズへ
	if (fallTimer >= fallTime) {
		phase = Phase::Stay;
	}

	return true;
}

bool Boss::Otamesi()
{
	//デバッグ用キー操作
	Input* input = Input::GetInstance();
	if (input->PushKey(DIK_1)) {
		DebugText::GetInstance()->Print("1push", 300, 300);

		for (const std::unique_ptr<BossAvatar>& bossAvatar : bossAvatars) {
			bossAvatar->Otamesi();//分身
		}

		return true;
	}

	DebugText::GetInstance()->Print("1noPush", 300, 300);
	return false;
}

bool Boss::Otamesi2()
{
	//デバッグ用キー操作
	Input* input = Input::GetInstance();
	if (input->PushKey(DIK_2)) {
		DebugText::GetInstance()->Print("2push", 400, 300);

		for (const std::unique_ptr<BossAvatar>& bossAvatar : bossAvatars) {
			bossAvatar->Otamesi2();//分身
		}

		return true;
	}

	DebugText::GetInstance()->Print("2noPush", 400, 300);
	return false;
}

bool Boss::Otamesi3()
{
	//デバッグ用キー操作
	Input* input = Input::GetInstance();
	if (input->PushKey(DIK_3)) {
		DebugText::GetInstance()->Print("3push", 600, 300);

		for (const std::unique_ptr<BossAvatar>& bossAvatar : bossAvatars) {
			bossAvatar->Otamesi3();//分身
		}

		return true;
	}

	DebugText::GetInstance()->Print("3noPush", 600, 300);
	return false;
}

bool Boss::Otamesi4()
{
	//デバッグ用キー操作
	Input* input = Input::GetInstance();
	if (input->PushKey(DIK_4)) {
		DebugText::GetInstance()->Print("4push", 700, 300);

		for (const std::unique_ptr<BossAvatar>& bossAvatar : bossAvatars) {
			bossAvatar->Otamesi4();//分身
		}

		return true;
	}

	DebugText::GetInstance()->Print("4noPush", 700, 300);
	return false;
}
