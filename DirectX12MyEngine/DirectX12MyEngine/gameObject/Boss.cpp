#include "Boss.h"
#include "Easing.h"
#include "Input.h"

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

	return true;
}

void Boss::Update()
{
	//行動
	Action();

	//更新
	bossBody->Update();//体
	bossHead->Update();//頭
}

void Boss::Draw()
{
	//描画
	bossBody->Draw();//体
	bossHead->Draw();//頭
}

void Boss::Action()
{
	switch (phase)
	{
	case Phase::Fall:
	{
		//イージングで降下する
		const float fallTime = 300;
		fallTimer++;
		const float time = fallTimer / fallTime;

		//生成位置から降りたところで停止する
		Vector3 stayPos = startPos;
		stayPos.y = 0;
		bossBody->SetPosition(Easing::Lerp(startPos, stayPos, time)) ;

		//イージングが終了したら次のフェーズへ
		if (fallTimer >= fallTime) {
			phase = Phase::Stay;
		}
	}
	break;

	case Phase::Stay:
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

		break;
	}
}
