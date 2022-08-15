#include "Boss.h"
#include "Easing.h"
#include "Input.h"
#include "DebugText.h"

void (Boss::* Boss::actionFuncTable[])() = {
	&Boss::Fall,
	&Boss::Stay
};

bool (Boss::* Boss::spFuncTable[])() = {
	&Boss::Otamesi,
	&Boss::Otamesi2
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
	//体生成
	bossBody.reset(BossBody::Create(bodyModel, position));
	//頭生成
	bossHead.reset(BossHead::Create(headModel, bossBody.get()));


	//HPバー生成
	const Vector2 hpBarPosition = { 20, 120 };
	hpBar.reset(BossHPBar::Create(4, hpBarPosition, maxHP));
	//HPバーフレーム生成
	const float posDiff = 3.0f;	//HPバーの座標との差分
	const Vector2 hpFramePosition = { hpBarPosition.x - posDiff, hpBarPosition.y - posDiff };
	hpFrame.reset(BossHPFrame::Create(5, hpFramePosition));

	//ビヘイビアツリー生成
	//behaviorTree.reset(BossBehaviorTree::Create(this));

	return true;
}

void Boss::Update()
{
	//行動
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//ビヘイビアツリーによる行動遷移
	//behaviorTree->Root();


	for (int i = 0; i < 2; i++) {
		if ((this->*spFuncTable[i])()) { break; }
	}


	//更新
	bossBody->Update();//体
	bossHead->Update();//頭

	//HPバー更新
	hpBar->Update();
	//HPバーフレーム更新
	hpFrame->Update();
}

void Boss::Draw()
{
	//描画
	bossBody->Draw();//体
	bossHead->Draw();//頭
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

void Boss::Fall()
{
	//イージングで降下する
	const float fallTime = 300;
	fallTimer++;
	const float time = fallTimer / fallTime;

	//生成位置から降りたところで停止する
	Vector3 stayPos = startPos;
	stayPos.y = 0;
	bossBody->SetPosition(Easing::Lerp(startPos, stayPos, time));

	//イージングが終了したら次のフェーズへ
	if (fallTimer >= fallTime) {
		phase = Phase::Stay;
	}
}

void Boss::Stay()
{
	//デバッグ用キー操作
	Input* input = Input::GetInstance();

	Vector3 posBody = bossBody->GetPosition();
	Vector3 posHead = bossHead->GetPosition();
	const float moveSpeed = 1.0f;
	if (input->PushKey(DIK_LEFT)) {
		//posBody.x -= moveSpeed;
		posHead.x -= moveSpeed;
	}
	if (input->PushKey(DIK_RIGHT)) {
		//posBody.x += moveSpeed;
		posHead.x += moveSpeed;
	}
	if (input->PushKey(DIK_UP)) {
		//posBody.y += moveSpeed;
		posHead.y += moveSpeed;
	}
	if (input->PushKey(DIK_DOWN)) {
		//posBody.y -= moveSpeed;
		posHead.y -= moveSpeed;
	}
	bossBody->SetPosition(posBody);
	bossHead->SetPosition(posHead);


	if (input->TriggerKey(DIK_Q)) {
		HP--;
		//HPは0以下にならない
		if (HP <= 0) {

			HP = 0;
		}

		hpBar->ChangeLength(HP);
	}
}
