#include "GameGroundManager.h"
#include "BasePlayer.h"
#include "BaseGameCamera.h"

BasePlayer* GameGroundManager::player = nullptr;
BaseGameCamera* GameGroundManager::gameCamera = nullptr;
const float GameGroundManager::modelScaleMagnification = 100.0f;

GameGroundManager* GameGroundManager::Create(ObjModel* model)
{
	//ゲームで使う地面管理のインスタンスを生成
	GameGroundManager* gameGroundManager = new GameGroundManager();
	if (gameGroundManager == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!gameGroundManager->Initialize(model)) {
		delete gameGroundManager;
		assert(0);
		return nullptr;
	}

	return gameGroundManager;
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
	//2枚目の地面は一枚目に地面の後ろに接地
	grounds[1]->SetPosition({ 0, 0, grounds[0]->GetPosition().z + grounds[0]->GetScale().z * modelScaleMagnification });

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
		if (grounds[i]->GetPosition().z + grounds[i]->GetScale().z * modelScaleMagnification <= swapPos) {
			if (i == 0) { grounds[0]->SetPosition({ 0, 0, grounds[1]->GetPosition().z + grounds[1]->GetScale().z * modelScaleMagnification }); }
			else if (i == 1) { grounds[1]->SetPosition({ 0, 0, grounds[0]->GetPosition().z + grounds[0]->GetScale().z * modelScaleMagnification }); }
		}
	}
}

void GameGroundManager::ScrollMode()
{
	//スクロール状態でなければ抜ける
	if (!isScrollMode) { return; }
	//自機とカメラのインスタンスがなければ抜ける
	if (!player || !gameCamera) { return; }

	//自機の移動速度状態によってビルをカメラで移動していた速度で動かす
	float moveSpeed = gameCamera->GetAdvanceSpeed();
	if (player->GetMoveSpeedPhase() == BasePlayer::MoveSpeedPhase::HighSpeed) { moveSpeed *= BaseGameCamera::GetHighSpeedMagnification(); }
	else if (player->GetMoveSpeedPhase() == BasePlayer::MoveSpeedPhase::SlowSpeed) { moveSpeed *= BaseGameCamera::GetSlowSpeedMagnification(); }

	//計算した移動速度で移動させる
	for (int i = 0; i < 2; i++) {
		Vector3 pos = grounds[i]->GetPosition();
		pos.z -= moveSpeed;
		grounds[i]->SetPosition(pos);
	}
}
