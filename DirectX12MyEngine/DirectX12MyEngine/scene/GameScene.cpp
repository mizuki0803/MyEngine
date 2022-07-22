#include "GameScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Easing.h"
#include "DemoEnemy.h"
#include "Cannon.h"
#include <cassert>
#include <fstream>
#include <iomanip>

using namespace DirectX;


void GameScene::Initialize()
{
	//カメラ初期化
	normalCamera.reset(new Camera());
	normalCamera->Initialize();
	railCamera.reset(new RailCamera());
	railCamera->Initialize();

	//ライト生成
	lightGroup.reset(LightGroup::Create());

	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, true);
	pointLightPos[0] = 0.5f;
	pointLightPos[1] = 1.0f;
	pointLightPos[2] = 0.0f;
	lightGroup->SetPointLightActive(0, false);
	lightGroup->SetPointLightActive(1, false);
	lightGroup->SetPointLightActive(2, false);
	//lightGroup->SetSpotLightActive(0, true);
	lightGroup->SetCircleShadowActive(0, true);

	//スプライト共通部分のインスタンスを取得
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	//スプライト用テクスチャ読み込み
	spriteCommon->LoadTexture(1, "reticle.png");


	//objからモデルデータを読み込む
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydome"));
	modelSphere.reset(ObjModel::LoadFromOBJ("sphere", true));
	modelFighter.reset(ObjModel::LoadFromOBJ("fighter", true));

	//自機に必要な情報をセット
	Player::SetGameScene(this);
	Player::SetBulletModel(modelSphere.get());
	//自機生成
	player.reset(Player::Create(modelFighter.get()));
	player->SetIsCameraFollow(true);

	//全敵に必要な情報をセット
	Enemy::SetGameScene(this);
	Enemy::SetPlayer(player.get());
	Enemy::SetBulletModel(modelSphere.get());
	//敵の速度を設定
	const Vector3 position(5, 0, 50);
	std::unique_ptr<Enemy> newEnemy;
	newEnemy.reset(Cannon::Create(modelFighter.get(), position));
	enemys.push_back(std::move(newEnemy));

	//天球生成
	objSkydome.reset(Skydome::Create(modelSkydome.get()));

	//objオブジェクトにカメラをセット
	ObjObject3d::SetCamera(railCamera.get());
	//objオブジェクトにライトをセット
	ObjObject3d::SetLightGroup(lightGroup.get());

	LoadEnemySetData();
}

void GameScene::Update()
{
	//入力のインスタンスを取得
	Input* input = Input::GetInstance();
	//デバッグテキストのインスタンスを取得
	DebugText* debugText = DebugText::GetInstance();

	//死亡した自機弾の削除
	playerBullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->GetIsDead();
		});

	//死亡した敵の削除
	enemys.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->GetIsDead();
		});

	//死亡した敵弾の削除
	enemyBullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->GetIsDead();
		});

	////キー入力でプレイヤーの位置を変更
	//if (input->PushKey(DIK_1) || input->PushKey(DIK_2) || input->PushKey(DIK_3) || input->PushKey(DIK_4) || input->PushKey(DIK_5) || input->PushKey(DIK_6))
	//{
	//	XMFLOAT3 move = { 0, 0, 0 };
	//	if (input->PushKey(DIK_1)) { move.x += 0.1f; }
	//	if (input->PushKey(DIK_2)) { move.x -= 0.1f; }
	//	if (input->PushKey(DIK_3)) { move.y += 0.1f; }
	//	if (input->PushKey(DIK_4)) { move.y -= 0.1f; }
	//	if (input->PushKey(DIK_5)) { move.z += 0.1f; }
	//	if (input->PushKey(DIK_6)) { move.z -= 0.1f; }
	//	XMFLOAT3 playerPos = objMan->GetPosition();
	//	playerPos.x += move.x;
	//	playerPos.y += move.y;
	//	playerPos.z += move.z;
	//	objMan->SetPosition(playerPos);
	//}
	//

	////ライト更新
	//lightGroup->Update();


	//// カメラ移動
	//if (input->PushKey(DIK_W) || input->PushKey(DIK_S) || input->PushKey(DIK_D) || input->PushKey(DIK_A) || input->PushKey(DIK_E) || input->PushKey(DIK_C))
	//{
	//	XMFLOAT3 move = { 0, 0, 0 };
	//	if (input->PushKey(DIK_W)) { move.y += 0.1f; }
	//	else if (input->PushKey(DIK_S)) { move.y -= 0.1f; }
	//	if (input->PushKey(DIK_D)) { move.x += 0.1f; }
	//	else if (input->PushKey(DIK_A)) { move.x -= 0.1f; }
	//	if (input->PushKey(DIK_E)) { move.z += 0.1f; }
	//	else if (input->PushKey(DIK_C)) { move.z -= 0.1f; }
	//	camera->MoveVector(move);
	//}

	////カメラのアングルを変更する
	//if (input->PushKey(DIK_LEFT) || input->PushKey(DIK_RIGHT) || input->PushKey(DIK_UP) || input->PushKey(DIK_DOWN))
	//{
	//	XMFLOAT3 angle = camera->GetAngle();
	//	if (input->PushKey(DIK_LEFT)) { angle.y += 1.0f; }
	//	if (input->PushKey(DIK_RIGHT)) { angle.y -= 1.0f; }
	//	if (input->PushKey(DIK_UP)) { angle.x += 1.0f; }
	//	if (input->PushKey(DIK_DOWN)) { angle.x -= 1.0f; }
	//	camera->SetAngle(angle);
	//}

	////カメラの距離を変更する
	//if (input->PushKey(DIK_M) || input->PushKey(DIK_N))
	//{
	//	float dis = camera->GetDistance();
	//	if (input->PushKey(DIK_M)) { dis += 0.1f; }
	//	if (input->PushKey(DIK_N)) { dis -= 0.1f; }
	//	camera->SetDistance(dis);
	//}

	if (input->TriggerKey(DIK_1))
	{
		const Vector3 position(5, 0, 50);
		std::unique_ptr<Enemy> newEnemy;
		newEnemy.reset(Cannon::Create(modelFighter.get(), position));
		enemys.push_back(std::move(newEnemy));
	}

	if (input->TriggerKey(DIK_2))
	{
		const Vector3 position(-5, 0, 50);
		std::unique_ptr<Enemy> newEnemy;
		const float enemySpeed = 0.5f;
		Vector3 velocity(0, 0, enemySpeed);
		newEnemy.reset(DemoEnemy::Create(modelFighter.get(), position, velocity));
		enemys.push_back(std::move(newEnemy));
	}

	UpdateEnemySetCommands();

	//カメラ更新
	normalCamera->Update();
	railCamera->Update();

	//オブジェクト更新
	//天球
	objSkydome->Update();
	//自機
	player->Update();
	//自機弾
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		bullet->Update();
	}
	//敵
	for (const std::unique_ptr<Enemy>& enemy : enemys)
	{
		enemy->Update();
	}
	//敵弾
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		bullet->Update();
	}

	//衝突判定管理
	CollisionCheck();

	//デバックテキスト
	//X座標,Y座標,縮尺を指定して表示
	debugText->Print("GAME SCENE", 1000, 50);
	std::string enemyNum = std::to_string(enemys.size());
	DebugText::GetInstance()->Print("EnemyNum : " + enemyNum, 200, 200);

	if (input->TriggerKey(DIK_RETURN))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("TITLE");
	}
}

void GameScene::Draw()
{
	//Object3d共通コマンド
	ObjObject3d::DrawPrev();
	///-------Object3d描画ここから-------///


	//天球
	objSkydome->Draw();
	//自機
	player->Draw();
	//自機弾
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets)
	{
		bullet->Draw();
	}
	//敵
	for (const std::unique_ptr<Enemy>& enemy : enemys)
	{
		enemy->Draw();
	}
	//敵弾
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets)
	{
		bullet->Draw();
	}


	///-------Object3d描画ここまで-------///


	//スプライト共通コマンド
	SpriteCommon::GetInstance()->DrawPrev();
	///-------スプライト描画ここから-------///

	player->DrawUI();


	///-------スプライト描画ここまで-------///


	//パーティクル共通コマンド
	//ParticleManager::DrawPrev();
	///-------パーティクル描画ここから-------///


	//パーティクル描画
	//particleMan->Draw();


	///-------パーティクル描画ここまで-------///
}

void GameScene::CollisionCheck()
{
	//判定対象の座標
	Vector3 posA, posB;
	float radiusA, radiusB;

#pragma region 自機と敵弾の衝突判定
	//自機座標
	posA = player->GetWorldPos();
	//自機半径
	radiusA = player->GetScale().x;

	//自機と全ての敵弾の衝突判定
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		//敵弾座標
		posB = bullet->GetWorldPos();
		//敵弾半径
		radiusB = bullet->GetScale().x;

		//球と球の衝突判定を行う
		bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);

		//衝突していたら
		if (isCollision) {
			//自機のコールバック関数を呼び出す
			player->OnCollision();
			//敵弾のコールバック関数を呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion

#pragma region 敵と自機弾の衝突判定
	//全ての敵と全ての自機弾の衝突判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//自機弾座標
		posA = bullet->GetWorldPos();
		//自機弾半径
		radiusA = bullet->GetScale().x;

		for (const std::unique_ptr<Enemy>& enemy : enemys) {
			//敵座標
			posB = enemy->GetWorldPos();
			//敵半径
			radiusB = enemy->GetScale().x;

			//球と球の衝突判定を行う
			bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);

			//衝突していたら
			if (isCollision) {
				//敵のコールバック関数を呼び出す
				enemy->OnCollision();
				//自機弾のコールバック関数を呼び出す
				bullet->OnCollision();

				break;
			}
		}
	}
#pragma endregion
}

void GameScene::AddPlayerBullet(std::unique_ptr<PlayerBullet> playerBullet)
{
	//自機弾リストに登録
	playerBullets.push_back(std::move(playerBullet));
}

void GameScene::AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet)
{
	//敵弾リストに登録
	enemyBullets.push_back(std::move(enemyBullet));
}

void GameScene::LoadEnemySetData()
{
	//ファイルを開く
	std::ifstream file;
	file.open("Resources/EnemySet.csv");
	assert(file.is_open());

	//ファイルの内容を文字列ストリームにコピー
	enemySetCommands << file.rdbuf();

	//ファイルを閉じる
	file.close();
}

void GameScene::UpdateEnemySetCommands()
{
	//待機処理
	if (isWait) {
		waitTimer--;
		if (waitTimer <= 0) {
			//待機終了
			isWait = false;
		}
		return;
	}

	//1行分の文字列を入れる変数
	std::string line;

	//コマンドを実行するループ
	while (getline(enemySetCommands, line)) {
		//1行分の文字列をストリーム変換して解析しやすく
		std::istringstream line_stream(line);

		std::string word;
		//「,」区切りで行の先頭文字を取得
		getline(line_stream, word, ',');

		//"//"から始める行はコメント
		if (word.find("//") == 0) {
			//コメント行を飛ばす
			continue;
		}

		//POPコマンド
		if (word.find("POP") == 0) {
			//x座標
			getline(line_stream, word, ',');
			float x = (float)std::atof(word.c_str());

			//y座標
			getline(line_stream, word, ',');
			float y = (float)std::atof(word.c_str());

			//z座標
			getline(line_stream, word, ',');
			float z = (float)std::atof(word.c_str());

			//敵を発生
			std::unique_ptr<Enemy> newEnemy;
			newEnemy.reset(Cannon::Create(modelFighter.get(), { x, y, z }));
			enemys.push_back(std::move(newEnemy));
		}

		//WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			//待ち時間
			int32_t waitTime = atoi(word.c_str());

			//待機開始
			isWait = true;
			waitTimer = waitTime;

			//コマンドループを抜ける
			break;
		}
	}
}
