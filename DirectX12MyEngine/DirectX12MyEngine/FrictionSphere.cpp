#include "FrictionSphere.h"
#include "Input.h"


FrictionSphere* FrictionSphere::Create(Model* model)
{
	//インスタンスを生成
	FrictionSphere* instance = new FrictionSphere();
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

FrictionSphere::~FrictionSphere()
{
	//オブジェクト解放
	delete sphereObject;
}

bool FrictionSphere::Initialize(Model* model)
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

void FrictionSphere::Update()
{
	Input* input = Input::GetInstance();

	//スペースキーで初期化
	if (input->TriggerKey(DIK_SPACE))
	{
		Reset();
	}

	//摩擦をかける
	Friction();

	//オブジェクト更新
	sphereObject->Update();
}

void FrictionSphere::Draw()
{
	//オブジェクト描画
	sphereObject->Draw();
}

void FrictionSphere::Friction()
{
	const float gravity = 9.8f / 60;	//重力
	const float n = mass * gravity;		//垂直抗力
	const float frictionForce = frictionCoefficient * n; //動摩擦力
	const float accel = frictionForce / mass; //加速度

	//速度に加速度を加算
	vel += -accel;

	//右向き速度は0以下にならない
	if (vel <= 0) 
	{
		vel = 0;
	}

	//更新した座標をセット
	XMFLOAT3 pos = sphereObject->GetPosition();
	pos.x += vel;
	sphereObject->SetPosition(pos);
}

void FrictionSphere::Reset()
{
	//初期位置にセット
	sphereObject->SetPosition(initPos);

	//初期速度をセット
	vel = initVel;
}
