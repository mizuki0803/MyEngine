#include "Turret.h"
#include "Input.h"

Turret* Turret::Create(Model* turretModel, Model* bulletModel)
{
	//インスタンスを生成
	Turret* instance = new Turret();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(turretModel, bulletModel)) {
		delete instance;
		assert(0);
	}

	return instance;
}

Turret::~Turret()
{
	//オブジェクト解放
	delete turretObject;
	delete bulletObject;
}

bool Turret::Initialize(Model* turretModel, Model* bulletModel)
{
	//砲台オブジェクト生成
	turretObject = Object3d::Create(turretModel);
	if (turretObject == nullptr) {
		return false;
	}

	//初期地点と大きさと回転をセット
	turretObject->SetPosition(startPos);
	XMFLOAT3 turretScale = { 4, 4, 4 };
	turretObject->SetScale(turretScale);
	turretObject->SetRotation({ 0, 90, 0 });


	//弾オブジェクト生成
	bulletObject = Object3d::Create(bulletModel);
	if (bulletObject == nullptr) {
		return false;
	}

	//初期地点と大きさをセット
	bulletObject->SetPosition(startPos);
	XMFLOAT3 bulletScale = { 2, 2, 2 };
	bulletObject->SetScale(bulletScale);

	return true;
}

void Turret::Update()
{
	//入力のインスタンスを取得
	Input* input = Input::GetInstance();

	//Rキー入力でリセット
	if (input->TriggerKey(DIK_R))
	{
		Reset();
	}

	//地面についていなければ重力による落下を行う
	if (!isOnGround)
	{
		XMFLOAT3 pos = bulletObject->GetPosition();

		//X方向移動(等速直線運動)
		pos.x += vel.x;

		//自由落下
		vel.y += gravity;
		//弾に重力をかけ続ける
		pos.y -= vel.y;

		//座標が0以下になったら地面に着いたことにする
		if (pos.y <= 0)
		{
			isOnGround = true;

			//押し戻し
			pos.y = 0;
		}

		//更新した座標をセット
		bulletObject->SetPosition(pos);
	}

	//オブジェクト更新
	turretObject->Update();
	bulletObject->Update();
}

void Turret::Draw()
{
	//オブジェクト描画
	turretObject->Draw();
	bulletObject->Draw();
}

void Turret::Reset()
{
	//初期地点に戻す
	bulletObject->SetPosition(startPos);
	//速度を初期化
	vel = { 10, -10 };
	//地面についていない
	isOnGround = false;
}
