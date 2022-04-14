#include "FallSphere.h"
#include "Input.h"

FallSphere* FallSphere::Create(Model* model)
{
	//インスタンスを生成
	FallSphere* instance = new FallSphere();
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

FallSphere::~FallSphere()
{
	//オブジェクト解放
	delete fallSphereObject;
}

bool FallSphere::Initialize(Model* model)
{
	//落下球オブジェクト生成
	fallSphereObject = Object3d::Create(model);
	if (fallSphereObject == nullptr) {
		return false;
	}

	//初期地点と大きさをセット
	fallSphereObject->SetPosition(startPos);
	XMFLOAT3 scale = { 2, 2, 2 };
	fallSphereObject->SetScale(scale);

	return true;
}

void FallSphere::Update()
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
		//自由落下
		velY += gravity;

		//落下球に重力をかけ続ける
		XMFLOAT3 pos = fallSphereObject->GetPosition();
		pos.y -= velY;

		//座標が0以下になったら地面に着いたことにする
		if (pos.y <= 0)
		{
			isOnGround = true;

			//押し戻し
			pos.y = 0;
		}

		//更新した座標をセット
		fallSphereObject->SetPosition(pos);
	}

	//オブジェクト更新
	fallSphereObject->Update();
}

void FallSphere::Draw()
{
	//オブジェクト描画
	fallSphereObject->Draw();
}

void FallSphere::Reset()
{
	//初期地点に戻す
	fallSphereObject->SetPosition(startPos);
	//速度を初期化
	velY = 0;
	//地面についていない
	isOnGround = false;
}
