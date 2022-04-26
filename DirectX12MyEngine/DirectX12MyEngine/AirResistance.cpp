#include "AirResistance.h"
#include "Input.h"


AirResistance* AirResistance::Create(Model* model)
{
	//インスタンスを生成
	AirResistance* instance = new AirResistance();
	if (instance == nullptr) {
		return nullptr;
	}

	//初期化
	if (!instance->Initialize(model)) {
		delete instance;
		assert(0);
	}

	return instance;
}

AirResistance::~AirResistance()
{
	//オブジェクト解放
	delete sphereObject;
}

bool AirResistance::Initialize(Model* model)
{
	//弾オブジェクト生成
	sphereObject = Object3d::Create(model);
	if (sphereObject == nullptr) {
		return false;
	}

	//初期位置をセット
	sphereObject->SetPosition(initPos);

	//初期速度をセット
	vel = initVel;

	return true;
}

void AirResistance::Update()
{
	Input* input = Input::GetInstance();

	//スペースキーで初期化
	if (input->TriggerKey(DIK_SPACE))
	{
		Reset();
	}

	//砲台の動き
	if (!isOnGround)
	{
		Turret();
	}

	//オブジェクト更新
	sphereObject->Update();
}

void AirResistance::Draw()
{
	//オブジェクト描画
	sphereObject->Draw();
}

void AirResistance::Turret()
{
	XMFLOAT3 pos = sphereObject->GetPosition();
	const float gravity = 9.8f / 60;
	const float gPower = gravity * mass;

	//空気抵抗を計算
	float resistance = vel.y;
	float resiAccel = resistance / mass / 60;

	//重力-空気抵抗で加速度を算出する
	accel.y = -gPower - resiAccel;

	//速度に加速度を加算
	vel.y += accel.y;

	//座標に速度を加算
	pos.x += vel.x;
	pos.y += vel.y;

	//y軸0以下は行かない
	if (pos.y < 0)
	{
		pos.y = 0;

		//地面についた
		isOnGround = true;
	}

	//更新した座標をセット
	sphereObject->SetPosition(pos);
}

void AirResistance::Reset()
{
	//初期位置にセット
	sphereObject->SetPosition(initPos);

	//初期速度をセット
	vel = initVel;

	//地面についていない
	isOnGround = false;
}
