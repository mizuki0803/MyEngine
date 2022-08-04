#include "GameScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Easing.h"
#include "DemoEnemy.h"
#include "CannonEnemy.h"
#include "CircularEnemy.h"
#include "FallEnemy.h"
#include "UpDownEnemy.h"
#include "ComeGoEnemy.h"
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

	//レールカメラに自機のポインタをセット
	RailCamera::SetPlayer(player.get());

	//全敵に必要な情報をセット
	Enemy::SetGameScene(this);
	Enemy::SetPlayer(player.get());
	Enemy::SetBulletModel(modelSphere.get());

	//天球生成
	objSkydome.reset(Skydome::Create(modelSkydome.get()));

	//objオブジェクトにカメラをセット
	ObjObject3d::SetCamera(railCamera.get());
	//objオブジェクトにライトをセット
	ObjObject3d::SetLightGroup(lightGroup.get());

	//敵配置スクリプトの読み込み
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

	//死亡した敵の削除前処理
	for (const std::unique_ptr<Enemy>& enemy : enemys) {
		//死亡していなければ飛ばす
		if (!enemy->GetIsDead()) { continue; }

		//レティクルのロックオン対象だった場合、ロックオン解除
		if (player->GetReticle()->GetReticle2D()->GetLockonEnemy() == enemy.get()) {
			player->GetReticle()->GetReticle2D()->UnlockonEnemy();
		}
		//自機弾のホーミング対象だった場合、ホーミング解除
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
			if (bullet->GetEnemy() == enemy.get()) {
				bullet->SetEnemy(nullptr);
			}
		}
	}
	//死亡した敵の削除
	enemys.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->GetIsDead();
		});

	//死亡した敵弾の削除
	enemyBullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->GetIsDead();
		});


	//敵発生コマンド更新
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
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		bullet->Update();
	}
	//敵
	for (const std::unique_ptr<Enemy>& enemy : enemys) {
		enemy->Update();
	}
	//敵弾
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		bullet->Update();
	}

	//衝突判定管理
	CollisionCheck3d();
	CollisionCheck2d();

	//デバックテキスト
	//X座標,Y座標,縮尺を指定して表示
	//debugText->Print("GAME SCENE", 1000, 50);
	std::string enemyNum = std::to_string(enemys.size());
	DebugText::GetInstance()->Print("EnemyNum : " + enemyNum, 200, 200);
	/*std::string playerHP = std::to_string(player->GetHP());
	if (!player->GetIsDead()) {
		DebugText::GetInstance()->Print("PlayerHP : " + playerHP, 200, 200);
	}
	else {
		DebugText::GetInstance()->Print("PlayerDead", 200, 200);
	}*/


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

void GameScene::CollisionCheck3d()
{
	//判定対象の座標
	Vector3 posA, posB;
	float radiusA, radiusB;

#pragma region 自機と敵の衝突判定
	//自機座標
	posA = player->GetWorldPos();
	//自機半径
	radiusA = player->GetScale().x;

	//自機と全ての敵の衝突判定
	for (const std::unique_ptr<Enemy>& enemy : enemys) {
		//敵座標
		posB = enemy->GetWorldPos();
		//敵半径
		radiusB = enemy->GetScale().x;

		//球と球の衝突判定を行う
		bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);

		//衝突していたら
		if (isCollision) {
			//自機のコールバック関数を呼び出す
			player->OnCollision(posB);
		}
	}
#pragma endregion

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
			player->OnCollision(posB);
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

void GameScene::CollisionCheck2d()
{
	//判定対象の座標
	Vector2 posA, posB;
	float radiusA, radiusB;

#pragma region レティクルと敵の衝突判定
	//プレイヤーがチャージショット状態なら
	if (player->GetIsChargeShotMode()) {
		//レティクルがロックオン状態なら抜ける
		if (player->GetReticle()->GetReticle2D()->GetIsLockon()) { return; }

		//レティクル座標
		posA = player->GetReticle()->GetReticle2D()->GetPosition();
		//レティクル半径
		radiusA = player->GetReticle()->GetReticle2D()->GetSize().x;

		//レティクルと全ての敵の衝突判定
		for (const std::unique_ptr<Enemy>& enemy : enemys) {
			//敵のワールド座標が自機のワールド座標より手前にいたら判定しないで抜ける
			const float enemyToPlayerPosZ = enemy->GetWorldPos().z - player->GetWorldPos().z;
			if (enemyToPlayerPosZ < 0) { continue; }

			//敵座標
			posB = enemy->GetScreenPos();
			//敵半径
			radiusB = enemy->GetScale().x;

			//敵座標が画面外なら処理は飛ばす
			const bool isOutsideScreen = (posB.x > WindowApp::window_width || posB.x < 0 || posB.y > WindowApp::window_height || posB.y < 0);
			if (isOutsideScreen) { continue; }

			//球と球の衝突判定を行う
			bool isCollision = Collision::CheckCircleToCircle(posA, posB, radiusA, radiusB);

			//衝突していたら
			if (isCollision) {
				//レティクルのロックオン対象を確定させる
				player->GetReticle()->GetReticle2D()->LockonEnemy(enemy.get());
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
			//敵の種類
			getline(line_stream, word, ',');
			int type = (int)std::atof(word.c_str());

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
			if (type == Enemy::EnemyType::Demo) {
				//x方向速度
				getline(line_stream, word, ',');
				float velX = (float)std::atof(word.c_str());

				//y方向速度
				getline(line_stream, word, ',');
				float velY = (float)std::atof(word.c_str());

				//z方向速度
				getline(line_stream, word, ',');
				float velZ = (float)std::atof(word.c_str());

				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(DemoEnemy::Create(modelSphere.get(), { x, y, z }, { velX, velY, velZ }));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::Cannon) {
				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(CannonEnemy::Create(modelFighter.get(), { x, y, z }));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::Circular) {
				//角度
				getline(line_stream, word, ',');
				float angle = (float)std::atof(word.c_str());

				//半径の長さ
				getline(line_stream, word, ',');
				float length = (float)std::atof(word.c_str());

				//回転速度
				getline(line_stream, word, ',');
				float rotSpeed = (float)std::atof(word.c_str());

				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(CircularEnemy::Create(modelSphere.get(), { x, y, z }, angle, length, rotSpeed));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::Fall) {
				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(FallEnemy::Create(modelSphere.get(), { x, y, z }));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::UpDown) {
				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(UpDownEnemy::Create(modelSphere.get(), { x, y, z }));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::ComeGo) {
				//x到着座標
				getline(line_stream, word, ',');
				float comeX = (float)std::atof(word.c_str());
				//y到着座標
				getline(line_stream, word, ',');
				float comeY = (float)std::atof(word.c_str());
				//z到着座標
				getline(line_stream, word, ',');
				float comeZ = (float)std::atof(word.c_str());

				//x出発座標
				getline(line_stream, word, ',');
				float goX = (float)std::atof(word.c_str());
				//y出発座標
				getline(line_stream, word, ',');
				float goY = (float)std::atof(word.c_str());
				//z出発座標
				getline(line_stream, word, ',');
				float goZ = (float)std::atof(word.c_str());

				//攻撃時間
				getline(line_stream, word, ',');
				int time = (int)std::atof(word.c_str());

				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(ComeGoEnemy::Create(modelSphere.get(), { x, y, z }, {comeX, comeY, comeZ}, {goX, goY, goZ}, time));
				enemys.push_back(std::move(newEnemy));
			}
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
