#include "GameMountainManager.h"

GameMountainManager* GameMountainManager::Create(ObjModel* model, float distance, int startNum)
{
	//山のインスタンスを生成
	GameMountainManager* gameMountainManager = new GameMountainManager();
	if (gameMountainManager == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!gameMountainManager->Initialize(model, distance, startNum)) {
		delete gameMountainManager;
		assert(0);
		return nullptr;
	}

	return gameMountainManager;
}

bool GameMountainManager::Initialize(ObjModel* model, float distance, int startNum)
{
	//モデルをセット
	assert(model);
	this->model = model;
	//中心からの距離をセット
	this->distance = distance;
	//山の番号をセット
	mountainNum = startNum;

	//初期からある山をセット
	for (int i = 0; i < mountainNum; i++) {
		std::unique_ptr<Mountain> newMountain;
		newMountain.reset(Mountain::Create(model, { -distance, 0, 0 + (float)i * 40 }));
		mountains.push_back(std::move(newMountain));
	}
	for (int i = 0; i < mountainNum; i++) {
		std::unique_ptr<Mountain> newMountain;
		newMountain.reset(Mountain::Create(model, { distance, 0, 0 + (float)i * 40 }));
		mountains.push_back(std::move(newMountain));
	}
	
	return true;
}

void GameMountainManager::Update(const Vector3& targetPos)
{
	//山の設置を可能にする
	isCanCreate = true;

	//山の死亡前処理
	for (const std::unique_ptr<Mountain>& mountain : mountains) {
		//新たに山の設置が可能でなければ抜ける
		if (!isCanCreate) { break; }
		//山が死亡していなければ飛ばす
		if (!mountain->GetIsDead()) { continue; }

		//新たな山設置
		CreateNewMountain();
	}
	//死亡した山の削除
	mountains.remove_if([](std::unique_ptr<Mountain>& sprite) {
		return sprite->GetIsDead();
		});

	//オブジェクト更新
	for (const std::unique_ptr<Mountain>& mountain : mountains) {
		mountain->Update();

		//山と対象の距離を比較し手前まで行ったら削除
		mountain->FrontOfScreenDelete(targetPos);
	}
}

void GameMountainManager::Draw()
{
	//オブジェクト描画
	for (const std::unique_ptr<Mountain>& mountain : mountains) {
		mountain->Draw();
	}
}

void GameMountainManager::DrawLightCameraView()
{
	//影用光源ライトから見た視点での描画
	for (const std::unique_ptr<Mountain>& mountain : mountains) {
		mountain->DrawLightCameraView();
	}
}

void GameMountainManager::CreateNewMountain()
{
	//新たな山生成
	std::unique_ptr<Mountain> newMountainLeft;
	newMountainLeft.reset(Mountain::Create(model, { -distance, 0, 0 + (float)mountainNum * 40 }));
	mountains.push_back(std::move(newMountainLeft));
	std::unique_ptr<Mountain> newMountainRight;
	newMountainRight.reset(Mountain::Create(model, { distance, 0, 0 + (float)mountainNum * 40 }));
	mountains.push_back(std::move(newMountainRight));

	//山の番号を更新
	mountainNum++;

	//山の設置を不可能にする
	isCanCreate = false;
}
