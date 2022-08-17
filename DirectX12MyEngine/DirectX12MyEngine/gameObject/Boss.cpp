#include "Boss.h"
#include "Easing.h"
#include "Input.h"
#include "DebugText.h"

void (Boss::* Boss::actionFuncTable[])() = {
	&Boss::Fall,
	&Boss::Stay
};

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
	const int avatarNum = 2;
	for (int i = 0; i < avatarNum; i++) {
		std::unique_ptr<BossAvatar> newBossAvatar;
		newBossAvatar.reset(BossAvatar::Create(headModel, bossMainBody.get()));
		bossAvatars.push_back(std::move(newBossAvatar));
	}

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
	//行動
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//ビヘイビアツリーによる行動遷移
	behaviorTree->Root();

	//更新
	bossMainBody->Update();//本体
	for (int i = 0; i < (signed)bossAvatars.size(); i++) {
		bossAvatars[i]->Update();//分身
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
	for (int i = 0; i < (signed)bossAvatars.size(); i++) {
		bossAvatars[i]->Draw();//分身
	}
}

void Boss::DrawUI()
{
	//HPバーフレーム描画
	hpFrame->Draw();
	//HPバー描画
	hpBar->Draw();
}

bool Boss::Otamesi()
{
	//デバッグ用キー操作
	Input* input = Input::GetInstance();
	if (input->PushKey(DIK_1)) {
		DebugText::GetInstance()->Print("1push", 300, 300);
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

		Vector3 posAvatar = bossAvatars[0]->GetPosition();
		const float moveSpeed = 0.25f;
		posAvatar.y -= moveSpeed;
		bossAvatars[0]->SetPosition(posAvatar);

		return true;
	}

	DebugText::GetInstance()->Print("4noPush", 700, 300);
	return false;
}

void Boss::Fall()
{
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
}

void Boss::Stay()
{
	//デバッグ用キー操作
	Input* input = Input::GetInstance();

	Vector3 posAvatar = bossAvatars[0]->GetPosition();
	const float moveSpeed = 1.0f;
	if (input->PushKey(DIK_LEFT)) {
		posAvatar.x -= moveSpeed;
	}
	if (input->PushKey(DIK_RIGHT)) {
		posAvatar.x += moveSpeed;
	}
	if (input->PushKey(DIK_UP)) {
		posAvatar.y += moveSpeed;
	}
	if (input->PushKey(DIK_DOWN)) {
		posAvatar.y -= moveSpeed;
	}
	bossAvatars[0]->SetPosition(posAvatar);


	if (input->TriggerKey(DIK_Q)) {
		HP--;
		//HPは0以下にならない
		if (HP <= 0) {

			HP = 0;
		}

		hpBar->ChangeLength(HP);
	}
}
