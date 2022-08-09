#include "Boss.h"
#include "Easing.h"

Boss* Boss::Create(ObjModel* model, const Vector3& position)
{
	//ボスのインスタンスを生成
	Boss* boss = new Boss();
	if (boss == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	boss->model = model;

	// 初期化
	if (!boss->Initialize()) {
		delete boss;
		assert(0);
		return nullptr;
	}

	//初期座標をセット
	boss->startPos = position;

	//座標をセット
	boss->position = position;

	boss->scale = { 3.0f, 3.0f, 3.0f };

	return boss;
}

void Boss::Update()
{
	//行動
	Action();

	//3Dオブジェクトの更新
	ObjObject3d::Update();
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
		position = Easing::Lerp(startPos, stayPos, time);

		//イージングが終了したら次のフェーズへ
		if (fallTimer >= fallTime) {
			phase = Phase::Stay;
		}
	}
	break;

	case Phase::Stay:
		//とりあえず何もしない

		break;
	}
}
