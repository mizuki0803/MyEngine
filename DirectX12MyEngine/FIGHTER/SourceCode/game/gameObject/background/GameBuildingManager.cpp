#include "GameBuildingManager.h"
#include "Player.h"
#include "GameCamera.h"

std::array<ObjModel*, 2> GameBuildingManager::buildingModels;
Player* GameBuildingManager::player = nullptr;
GameCamera* GameBuildingManager::gameCamera = nullptr;
bool GameBuildingManager::isScrollMode = false;

GameBuildingManager* GameBuildingManager::Create(float centerDistance, float objectDistance, int startNum, const Vector3& basePos)
{
	//ゲームで使うビル管理のインスタンスを生成
	GameBuildingManager* gameBuildingManager = new GameBuildingManager();
	if (gameBuildingManager == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!gameBuildingManager->Initialize(centerDistance, objectDistance, startNum, basePos)) {
		delete gameBuildingManager;
		assert(0);
		return nullptr;
	}

	return gameBuildingManager;
}

void GameBuildingManager::SetBuidingModel(int modelNum, ObjModel* model)
{
	//ビルモデルをセット
	assert(model);
	buildingModels[modelNum] = model;
}

bool GameBuildingManager::Initialize(float centerDistance, float objectDistance, int startNum, const Vector3& basePos)
{
	//基準の座標をセット
	this->basePos = basePos;
	//中心からの距離をセット
	this->centerDistance = centerDistance;
	//オブジェクト同士の距離をセット
	this->objectDistance = objectDistance;
	//ビルの番号をセット
	buildingNum = startNum;

	//初期からあるビルをセット
	for (int i = 0; i < buildingNum; i++) {
		std::unique_ptr<Building> newBuilding;
		const int leftModelNum = NextCreateModelNum(lastLeftSetModelNum); //左側に設置するモデル番号を取得
		newBuilding.reset(Building::Create(buildingModels[leftModelNum], { basePos.x - centerDistance, basePos.y, basePos.z + (float)i * objectDistance }));
		//左側はオブジェクトを反転させる
		newBuilding->SetRotation({ 0, 180, 0 });
		buildings.push_back(std::move(newBuilding));
	}
	for (int i = 0; i < buildingNum; i++) {
		std::unique_ptr<Building> newBuilding;
		const int rightModelNum = NextCreateModelNum(lastRightSetModelNum); //右側に設置するモデル番号を取得
		newBuilding.reset(Building::Create(buildingModels[rightModelNum], { basePos.x + centerDistance, basePos.y, basePos.z + (float)i * objectDistance }));
		buildings.push_back(std::move(newBuilding));
	}

	return true;
}

void GameBuildingManager::Update()
{
	//ビルの設置を可能にする
	isCanCreate = true;

	//ビルの死亡前処理
	for (const std::unique_ptr<Building>& building : buildings) {
		//新たにビルの設置が可能でなければ抜ける
		if (!isCanCreate) { break; }
		//ビルが死亡していなければ飛ばす
		if (!building->GetIsDead()) { continue; }

		//新たなビル設置
		CreateNewBuilding();
	}
	//死亡したビルの削除
	buildings.remove_if([](std::unique_ptr<Building>& sprite) {
		return sprite->GetIsDead();
		});

	//スクロール状態
	ScrollMode();

	for (const std::unique_ptr<Building>& building : buildings) {
		//オブジェクト更新
		building->Update();

		//ビルと対象の距離を比較し手前まで行ったら削除
		building->FrontOfScreenDelete(gameCamera->GetPosition());
	}
}

void GameBuildingManager::Draw()
{
	//オブジェクト描画
	for (const std::unique_ptr<Building>& building : buildings) {
		building->Draw();
	}
}

void GameBuildingManager::DrawLightCameraView()
{
	//影用光源ライトから見た視点での描画
	for (const std::unique_ptr<Building>& building : buildings) {
		building->DrawLightCameraView();
	}
}

void GameBuildingManager::CreateNewBuilding()
{
	//一番後ろのビルの座標を取得
	std::list<std::unique_ptr<Building>>::iterator itr = buildings.end();
	--itr;
	Building* lastBuilding = itr->get();

	//新たなビル生成
	std::unique_ptr<Building> newBuildingLeft;
	const int leftModelNum = NextCreateModelNum(lastLeftSetModelNum);
	newBuildingLeft.reset(Building::Create(buildingModels[leftModelNum], { basePos.x - centerDistance, basePos.y, lastBuilding->GetPosition().z + objectDistance }));
	//左側はオブジェクトを反転させる
	newBuildingLeft->SetRotation({ 0, 180, 0 });
	buildings.push_back(std::move(newBuildingLeft));
	std::unique_ptr<Building> newBuildingRight;
	const int rightModelNum = NextCreateModelNum(lastRightSetModelNum);
	newBuildingRight.reset(Building::Create(buildingModels[rightModelNum], { basePos.x + centerDistance, basePos.y, lastBuilding->GetPosition().z + objectDistance }));
	buildings.push_back(std::move(newBuildingRight));

	//ビルの番号を更新
	buildingNum++;

	//ビルの設置を不可能にする
	isCanCreate = false;
}

int GameBuildingManager::NextCreateModelNum(int& lastSetModelNum)
{
	//設置するモデル番号
	int createModelNum = lastSetModelNum;

	//設置するモデル番号をランダムで設定(前回と同じモデルは選ばないようにする)
	while (createModelNum == lastSetModelNum) {
		createModelNum = rand() % (int)buildingModels.size();
	}

	//今回選択されたモデル番号を前回のモデル番号にセットし更新
	lastSetModelNum = createModelNum;

	return createModelNum;
}

void GameBuildingManager::ScrollMode()
{
	//スクロール状態でなければ抜ける
	if (!isScrollMode) { return; }
	//自機とカメラのインスタンスがなければ抜ける
	if (!player || !gameCamera) { return; }

	//自機の移動速度状態によってビルをカメラで移動していた速度で動かす
	float moveSpeed = GameCamera::GetAdvanceSpeed();
	if (player->GetMoveSpeedPhase() == Player::MoveSpeedPhase::HighSpeed) { moveSpeed *= GameCamera::GetHighSpeedMagnification(); }
	else if (player->GetMoveSpeedPhase() == Player::MoveSpeedPhase::SlowSpeed) { moveSpeed *= GameCamera::GetSlowSpeedMagnification(); }

	//計算した移動速度で移動させる
	for (const std::unique_ptr<Building>& building : buildings) {
		Vector3 pos = building->GetPosition();
		pos.z -= moveSpeed;
		building->SetPosition(pos);
	}
}
