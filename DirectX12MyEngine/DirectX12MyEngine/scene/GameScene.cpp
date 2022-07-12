#include "GameScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Easing.h"
#include <cassert>
#include <sstream>
#include <iomanip>

using namespace DirectX;


void GameScene::Initialize()
{
	//カメラ初期化
	camera = new Camera();
	camera->Initialize();


	//ライト生成
	lightGroup = LightGroup::Create();

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


	//objからモデルデータを読み込む
	modelMan = ObjModel::LoadFromOBJ("man");
	modelSkydome = ObjModel::LoadFromOBJ("skydome");
	modelGround = ObjModel::LoadFromOBJ("ground");
	modelSphere = ObjModel::LoadFromOBJ("sphere", true);
	modelFighter = ObjModel::LoadFromOBJ("fighter", true);

	//objオブジェクト生成
	objMan = ObjObject3d::Create(modelMan);
	objGround = ObjObject3d::Create(modelGround);
	objSkydome = ObjObject3d::Create(modelSkydome);
	objSphere = ObjObject3d::Create(modelSphere);

	//初期座標
	objMan->SetPosition({ 0, 1, 0 });
	objGround->SetPosition({ 0, -1, 0 });
	objSphere->SetPosition({ -1, 0, 0 });

	player.reset(Player::Create(modelSphere));

	//敵の速度を設定
	const Vector3 position(5, 0, 50);
	const float enemySpeed = 0.1f;
	Vector3 velocity(0, 0, enemySpeed);
	enemy.reset(Enemy::Create(modelSphere, position, velocity));

	//敵に自機のアドレスを渡す
	enemy->SetPlayer(player.get());

	//角度初期値
	objMan->SetRotation({ 0, 90, 0 });

	//objオブジェクトにカメラをセット
	ObjObject3d::SetCamera(camera);
	//objオブジェクトにライトをセット
	ObjObject3d::SetLightGroup(lightGroup);
}

void GameScene::Finalize()
{
	//カメラ解放
	delete camera;

	//ライト解放
	delete lightGroup;

	//モデル解放
	delete modelMan;
	delete modelSkydome;
	delete modelGround;
	delete modelSphere;
	delete modelFighter;

	//objオブジェクト解放
	delete objMan;
	delete objGround;
	delete objSkydome;
	delete objSphere;
}

void GameScene::Update()
{
	//入力のインスタンスを取得
	Input* input = Input::GetInstance();
	//デバッグテキストのインスタンスを取得
	DebugText* debugText = DebugText::GetInstance();

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

	//カメラ更新
	camera->Update();

	//Object3d更新
	objMan->Update();
	objGround->Update();
	objSkydome->Update();
	objSphere->Update();
	player->Update();

	if (enemy)
	{
		enemy->Update();
	}

	//衝突判定管理
	CollisionCheck();

	//デバックテキスト
	//X座標,Y座標,縮尺を指定して表示
	debugText->Print("GAME SCENE", 1000, 50);

	//if (input->TriggerKey(DIK_RETURN))
	//{
	//	//シーン切り替え
	//	SceneManager::GetInstance()->ChangeScene("TITLE");
	//}
}

void GameScene::Draw()
{
	//Object3d共通コマンド
	ObjObject3d::DrawPrev();
	///-------Object3d描画ここから-------///


	//objMan->Draw();
	//objGround->Draw();
	//objSkydome->Draw();
	//objSphere->Draw();
	player->Draw();

	if (enemy)
	{
		enemy->Draw();
	}


	///-------Object3d描画ここまで-------///


	//スプライト共通コマンド
	//SpriteCommon::GetInstance()->DrawPrev();
	///-------スプライト描画ここから-------///




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

	//自機弾リスト取得
	const std::list<std::unique_ptr<PlayerBullet>>& playerBullets = player->GetBullets();
	//敵弾リスト取得
	const std::list<std::unique_ptr<EnemyBullet>>& enemyBullets = enemy->GetBullets();

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
	//敵座標
	posA = enemy->GetWorldPos();
	//敵半径
	radiusA = enemy->GetScale().x;

	//敵と全ての自機弾の衝突判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//敵弾座標
		posB = bullet->GetWorldPos();
		//敵弾半径
		radiusB = bullet->GetScale().x;

		//球と球の衝突判定を行う
		bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);

		//衝突していたら
		if (isCollision) {
			//敵のコールバック関数を呼び出す
			enemy->OnCollision();
			//自機弾のコールバック関数を呼び出す
			bullet->OnCollision();
		}
	}
#pragma endregion
}
