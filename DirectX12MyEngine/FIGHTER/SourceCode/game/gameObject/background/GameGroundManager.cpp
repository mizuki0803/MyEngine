#include "GameGroundManager.h"
#include "Player.h"
#include "GameCamera.h"

Player* GameGroundManager::player = nullptr;
GameCamera* GameGroundManager::gameCamera = nullptr;
bool GameGroundManager::isScrollMode = false;

GameGroundManager* GameGroundManager::Create(ObjModel* model)
{
	//山のインスタンスを生成
	GameGroundManager* gameMountainManager = new GameGroundManager();
	if (gameMountainManager == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!gameMountainManager->Initialize(model)) {
		delete gameMountainManager;
		assert(0);
		return nullptr;
	}

	return gameMountainManager;
}

bool GameGroundManager::Initialize(ObjModel* model)
{
	//モデルをセット
	assert(model);
	this->model = model;

	//地面を生成
	for (int i = 0; i < 2; i++) {
		grounds[i].reset(Ground::Create(model));
	}
	grounds[1]->SetPosition({ 0, 0, grounds[0]->GetPosition().z + grounds[0]->GetScale().z * 100 });

	return true;
}

void GameGroundManager::Update()
{
	//スクロール状態
	ScrollMode();

	//後ろに行った地面を前に持ってくる
	SwapGroundPos();

	//オブジェクト更新
	for (int i = 0; i < 2; i++) {
		grounds[i]->Update();
	}
}

void GameGroundManager::Draw()
{
	//オブジェクト描画
	for (int i = 0; i < 2; i++) {
		grounds[i]->Draw();
	}
}
void GameGroundManager::SwapGroundPos()
{
	//カメラのインスタンスがなければ抜ける
	if (!gameCamera) { return; }

	//座標と大きさを足した値が自機より手前(画面外手前)まで行ったら手前に持ってくる
	const float flontOfScreenDiffence = 50;
	const float swapPos = gameCamera->GetPosition().z - flontOfScreenDiffence;
	for (int i = 0; i < 2; i++) {
		if (grounds[i]->GetPosition().z + grounds[i]->GetScale().z * 100 <= swapPos) {
			if (i == 0) { grounds[0]->SetPosition({ 0, 0, grounds[1]->GetPosition().z + grounds[1]->GetScale().z * 100 }); }
			else if (i == 1) { grounds[1]->SetPosition({ 0, 0, grounds[0]->GetPosition().z + grounds[0]->GetScale().z * 100 }); }
		}
	}
}

void GameGroundManager::ScrollMode()
{
	//スクロール状態でなければ抜ける
	if (!isScrollMode) { return; }
	//自機とカメラのインスタンスがなければ抜ける
	if (!player || !gameCamera) { return; }

	//自機の移動速度状態によって山をカメラで移動していた速度で動かす
	float moveSpeed = GameCamera::GetAdvanceSpeed();
	if (player->GetMoveSpeedPhase() == Player::MoveSpeedPhase::HighSpeed) { moveSpeed *= GameCamera::GetHighSpeedMagnification(); }
	else if (player->GetMoveSpeedPhase() == Player::MoveSpeedPhase::SlowSpeed) { moveSpeed *= GameCamera::GetSlowSpeedMagnification(); }

	//計算した移動速度で移動させる
	for (int i = 0; i < 2; i++) {
		Vector3 pos = grounds[i]->GetPosition();
		pos.z -= moveSpeed;
		grounds[i]->SetPosition(pos);
	}
}
