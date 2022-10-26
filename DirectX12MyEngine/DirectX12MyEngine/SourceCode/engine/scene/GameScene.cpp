#include "GameScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Easing.h"
#include "ParticleEmitter.h"
#include "HomingBullet.h"
#include "DemoEnemy.h"
#include "CannonEnemy.h"
#include "CircularEnemy.h"
#include "FallEnemy.h"
#include "UpDownEnemy.h"
#include "ComeGoEnemy.h"
#include "EnemyDefeatCounter.h"
#include <cassert>
#include <fstream>
#include <iomanip>

using namespace DirectX;


void GameScene::Initialize()
{
	//ゲームカメラ初期化
	gameCamera.reset(new GameCamera());
	gameCamera->Initialize();

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


	//objからモデルデータを読み込む
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydome"));
	modelGround.reset(ObjModel::LoadFromOBJ("ground"));
	modelMountain.reset(ObjModel::LoadFromOBJ("mountain"));
	modelSphere.reset(ObjModel::LoadFromOBJ("sphere", true));
	modelFighter.reset(ObjModel::LoadFromOBJ("fighter", true));
	modelEnemyFighter.reset(ObjModel::LoadFromOBJ("enemyFighter", true));
	modelBossMainBody.reset(ObjModel::LoadFromOBJ("bossMainBody", true));
	modelBossMainBodyDamage.reset(ObjModel::LoadFromOBJ("bossMainBodyDamage", true));
	modelBossMainBodySleep.reset(ObjModel::LoadFromOBJ("bossMainBodySleep", true));
	modelBossAvatar.reset(ObjModel::LoadFromOBJ("bossAvatar", true));
	modelBossAvatarDamage.reset(ObjModel::LoadFromOBJ("bossAvatarDamage", true));
	modelBossAvatarSleep.reset(ObjModel::LoadFromOBJ("bossAvatarSleep", true));
	modelHealingItem.reset(ObjModel::LoadFromOBJ("healingItem"));

	//自機に必要な情報をセット
	Player::SetGameScene(this);
	Player::SetBulletModel(modelSphere.get());
	//自機生成
	player.reset(Player::Create(modelFighter.get()));
	player->SetIsCameraFollow(true);

	//ゲームカメラに自機のポインタをセット
	gameCamera->SetPlayer(player.get());

	//全敵に必要な情報をセット
	Enemy::SetGameScene(this);
	Enemy::SetPlayer(player.get());
	Enemy::SetBulletModel(modelSphere.get());
	//各種類の敵に必要な情報をセット
	CannonEnemy::SetBreakModel(modelSphere.get());
	//敵配置スクリプトの読み込み
	LoadEnemySetData();

	//ボスに必要な情報をセット
	Boss::SetPlayer(player.get());
	BossMainBody::SetGameScene(this);
	BossMainBody::SetBossMainBodyModel(modelBossMainBody.get());
	BossMainBody::SetBossMainBodyDamageModel(modelBossMainBodyDamage.get());
	BossMainBody::SetBossMainBodySleepModel(modelBossMainBodySleep.get());
	BossMainBody::SetBulletModel(modelSphere.get());
	BossAvatar::SetGameScene(this);
	BossAvatar::SetAvatarModel(modelBossAvatar.get());
	BossAvatar::SetAvatarDamageModel(modelBossAvatarDamage.get());
	BossAvatar::SetAvatarSleepModel(modelBossAvatarSleep.get());
	BossAvatar::SetBulletModel(modelSphere.get());

	//回復アイテムに必要な情報をセット
	HealingItem::SetPlayer(player.get());

	//天球生成
	skydome.reset(Skydome::Create(modelSkydome.get()));

	//地面生成
	ground.reset(Ground::Create(modelGround.get()));

	//背景用(山)生成
	for (int i = 0; i < 20; i++) {
		std::unique_ptr<Mountain> newMountain;
		newMountain.reset(Mountain::Create(modelMountain.get(), { -75, 0, 0 + (float)i * 40 }));
		mountains.push_back(std::move(newMountain));
	}
	for (int i = 0; i < 20; i++) {
		std::unique_ptr<Mountain> newMountain;
		newMountain.reset(Mountain::Create(modelMountain.get(), { 75, 0, 0 + (float)i * 40 }));
		mountains.push_back(std::move(newMountain));
	}

	//objオブジェクトにカメラをセット
	ObjObject3d::SetCamera(gameCamera.get());
	//objオブジェクトにライトをセット
	ObjObject3d::SetLightGroup(lightGroup.get());


	//パーティクルにカメラをセット
	ParticleManager::SetCamera(gameCamera.get());

	//敵を倒した数カウンターリセット
	EnemyDefeatCounter::CounterReset();
}

void GameScene::Update()
{
	//入力のインスタンスを取得
	Input* input = Input::GetInstance();
	//デバッグテキストのインスタンスを取得
	DebugText* debugText = DebugText::GetInstance();

	//オブジェクト解放
	ObjectRelease();
	//敵発生コマンド更新
	UpdateEnemySetCommands();
	//ボスバトル開始判定処理
	BossBattleStart();
	//ステージクリア
	StageClear();
	//ゲームオーバー
	GameOver();

	//カメラ更新
	gameCamera->Update();

	//オブジェクト更新
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
	//ボス
	if (boss) {
		boss->Update();
	}
	//敵弾
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		bullet->Update();
	}
	//回復アイテム
	for (const std::unique_ptr<HealingItem>& healingItem : healingItems) {
		healingItem->Update();
	}
	//天球
	skydome->Update();
	//地面
	ground->Update();
	//背景用(山)
	for (const std::unique_ptr<Mountain>& mountain : mountains) {
		mountain->Update();
	}
	//3D衝突判定管理
	CollisionCheck3d();

	//スプライト更新
	//複数体処理用UI 
	for (const std::unique_ptr<MultiHitUI>& multiHitUI : multiHitUIs) {
		multiHitUI->Update();
	}
	//ステージクリアテキスト
	if (stageClearText) {
		stageClearText->Update();
	}
	//ステージリザルトUI
	if (stageResultUI) {
		stageResultUI->Update();
	}
	//2D衝突判定管理
	CollisionCheck2d();

	//パーティクル更新
	ParticleEmitter::GetInstance()->Update();

	//デバックテキスト
	{
		std::string itemNum = std::to_string(healingItems.size());
		DebugText::GetInstance()->Print("ItemNum : " + itemNum, 100, 200);
		//X座標,Y座標,縮尺を指定して表示
		//debugText->Print("GAME SCENE", 1000, 50);
		/*std::string enemyDefeat = std::to_string(EnemyDefeatCounter::GetDefeatCount());
		DebugText::GetInstance()->Print("EnemyDefeatCount : " + enemyDefeat, 100, 160);
		std::string highScore = std::to_string(EnemyDefeatCounter::GetHighScore());
		DebugText::GetInstance()->Print("HighScore : " + highScore, 100, 180);
		std::string enemyNum = std::to_string(enemys.size());
		DebugText::GetInstance()->Print("EnemyNum : " + enemyNum, 100, 200);
		if (isStageClear) {
			DebugText::GetInstance()->Print("STAGE CLEAR", 100, 250);
		}*/
		/*std::string playerHP = std::to_string(player->GetHP());
		if (!player->GetIsDead()) {
			DebugText::GetInstance()->Print("PlayerHP : " + playerHP, 200, 200);
		}
		else {
			DebugText::GetInstance()->Print("PlayerDead", 200, 200);
		}*/
	}

	if (input->TriggerKey(DIK_RETURN))
	{
		//シーン切り替え
		SceneManager::GetInstance()->ChangeScene("GAME");
	}
}

void GameScene::Draw()
{
	//Object3d共通コマンド
	ObjObject3d::DrawPrev();
	///-------Object3d描画ここから-------///

	//自機
	player->Draw();
	//自機弾
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		if (!(bullet->GetBulletType() == PlayerBullet::BulletType::Straight)) { continue; }
		bullet->Draw();
	}
	//敵
	for (const std::unique_ptr<Enemy>& enemy : enemys) {
		enemy->Draw();
	}
	//敵弾
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		bullet->Draw();
	}
	//ボス
	if (boss) {
		boss->Draw();
	}
	//回復アイテム
	for (const std::unique_ptr<HealingItem>& healingItem : healingItems) {
		healingItem->Draw();
	}
	//天球
	skydome->Draw();
	//地面
	ground->Draw();
	//背景用(山)
	for (const std::unique_ptr<Mountain>& mountain : mountains) {
		mountain->Draw();
	}

	///-------Object3d描画ここまで-------///


	//パーティクル共通コマンド
	ParticleManager::DrawPrev();
	///-------パーティクル描画ここから-------///

	//パーティクル描画
	ParticleEmitter::GetInstance()->DrawAll();

	///-------パーティクル描画ここまで-------///


	//スプライト共通コマンド
	SpriteCommon::GetInstance()->DrawPrev();
	///-------スプライト描画ここから-------///

	//自機のUI
	player->DrawUI();
	//ボスのUI
	if (boss) {
		boss->DrawUI();
	}
	//複数体処理用UI 
	for (const std::unique_ptr<MultiHitUI>& multiHitUI : multiHitUIs) {
		multiHitUI->Draw();
	}
	//ステージクリアテキスト
	if (stageClearText) {
		stageClearText->Draw();
	}
	//ステージリザルトUI
	if (stageResultUI) {
		stageResultUI->Draw();
	}

	///-------スプライト描画ここまで-------///
}

void GameScene::ObjectRelease()
{
	//死亡した自機弾の削除前処理
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//死亡していなければ飛ばす
		if (!bullet->GetIsDead()) { continue; }

		//レティクルがロックオン中だった場合、ホーミング弾がロックオン以外の敵に当たってしまう可能性もあるのでロックオン解除しておく
		if (!(bullet->GetBulletType() == PlayerBullet::BulletType::Homing)) { continue; }
		if (!player->GetReticles()->GetIsLockon()) { continue; }

		player->GetReticles()->UnlockonEnemy();
	}
	//死亡した自機弾の削除
	playerBullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->GetIsDead();
		});

	//死亡した敵の処理
	for (const std::unique_ptr<Enemy>& enemy : enemys) {
		//死亡していなければ飛ばす
		if (!enemy->GetIsDead()) { continue; }

		//レティクルのロックオン対象だった場合、ロックオン解除
		if (player->GetReticles()->GetLockonEnemy() == enemy.get()) {
			player->GetReticles()->UnlockonEnemy();
		}
		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
			//ホーミング弾でなければ飛ばす
			if (bullet->GetBulletType() != PlayerBullet::BulletType::Homing) { continue; }
			//自機弾のホーミング対象でなければ飛ばす
			if (bullet->GetEnemy() != enemy.get()) { continue; }
			//ホーミング解除
			bullet->SetEnemy(nullptr);
		}
	}
	//削除状態の敵の前処理
	for (const std::unique_ptr<Enemy>& enemy : enemys) {
		//削除状態でなければ飛ばす
		if (!enemy->GetIsDelete()) { continue; }

		for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
			//ホーミング弾でなければ飛ばす
			if (bullet->GetBulletType() != PlayerBullet::BulletType::Homing) { continue; }
			//自機弾のホーミング対象でなければ飛ばす
			if (bullet->GetEnemy() != enemy.get()) { continue; }
			//ホーミング解除
			bullet->SetEnemy(nullptr);
		}

		//被弾して削除状態でなければ飛ばす
		if (!enemy->GetIsHitDelete()) { continue; }

		//死亡させるだけでなく削除状態まで弾を当てた敵の座標に回復アイテム生成
		std::unique_ptr<HealingItem> healingItem;
		healingItem.reset(HealingItem::Create(modelHealingItem.get(), enemy->GetPosition()));
		healingItems.push_back(std::move(healingItem));
	}
	//削除状態の敵の削除
	enemys.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->GetIsDelete();
		});

	//死亡した敵弾の削除
	enemyBullets.remove_if([](std::unique_ptr<EnemyBullet>& bullet) {
		return bullet->GetIsDead();
		});

	//削除状態の回復アイテムの削除
	healingItems.remove_if([](std::unique_ptr<HealingItem>& healingItem) {
		return healingItem->GetIsDelete();
		});

	//削除状態のボスの削除
	if (boss) {
		if (boss->GetIsDelete()) {
			//自機を帰還させる
			player->StageClearReturnStart(gameCamera->GetPosition());
			//カメラのボス本体情報を解除させる
			gameCamera->BossDelete();

			boss.reset();
		}
	}

	//死亡した複数体処理用UIの削除
	multiHitUIs.remove_if([](std::unique_ptr<MultiHitUI>& multiHitUI) {
		return multiHitUI->GetIsDead();
		});
}

void GameScene::CollisionCheck3d()
{
	//判定対象の座標
	Vector3 posA, posB;
	float radiusA, radiusB;

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
			//衝突していなければ飛ばす
			if (!isCollision) { continue; }

			//敵のコールバック関数を呼び出す
			enemy->OnCollision();
			//自機弾のコールバック関数を呼び出す
			bullet->OnCollision();

			//弾は一つの敵しか倒せないのでenemyループを抜ける
			break;
		}
	}
#pragma endregion

#pragma region 敵と死亡したホーミング弾破裂の衝突判定
	//全ての敵と全ての自機弾の衝突判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//ホーミング弾でなければ飛ばす
		if (!(bullet->GetBulletType() == PlayerBullet::BulletType::Homing)) { continue; }
		//死亡していなければ飛ばす
		if (!bullet->GetIsDead()) { continue; }

		//死亡した自機弾座標
		posA = bullet->GetWorldPos();
		//死亡した自機弾の破裂判定半径
		radiusA = HomingBullet::GetBlastSize();

		//一撃で倒した敵の数
		int hitNum = 0;

		for (const std::unique_ptr<Enemy>& enemy : enemys) {
			//既にこのフレームで自機弾との当たり判定が作用していたら飛ばす
			if (enemy->GetIsCollisionFrame()) { continue; }

			//敵座標
			posB = enemy->GetWorldPos();
			//敵半径
			radiusB = enemy->GetScale().x;

			//球と球の衝突判定を行う
			bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);
			//衝突していなければ飛ばす
			if (!isCollision) { continue; }

			//まだ死亡していない(この被弾で死亡する)場合は一撃で倒した敵の数を増やす
			if (!enemy->GetIsDead()) { hitNum++; }

			//敵のコールバック関数を呼び出す
			enemy->OnCollision();
		}

		//一撃で倒した敵の数が0なら飛ばす
		if (hitNum == 0) { continue; }

		//複数体撃破用UI生成
		std::unique_ptr<MultiHitUI> newMultiHitUI;
		newMultiHitUI.reset(MultiHitUI::Create(posA, gameCamera.get(), hitNum));
		multiHitUIs.push_back(std::move(newMultiHitUI));
	}
#pragma endregion

	//自機が死亡していたらこの先の処理は行わない
	if (player->GetIsDead()) { return; }

#pragma region 自機と敵の衝突判定
	//自機が緊急回避をしていなければ判定する
	if (!player->GetIsRoll()) {
		//自機座標
		posA = player->GetWorldPos();
		//自機半径
		radiusA = player->GetScale().x;

		//自機と全ての敵の衝突判定
		for (const std::unique_ptr<Enemy>& enemy : enemys) {
			//自機がダメージ状態なら飛ばす
			if (player->GetIsDamage()) { continue; }

			//敵座標
			posB = enemy->GetWorldPos();
			//敵半径
			radiusB = enemy->GetScale().x;

			//球と球の衝突判定を行う
			bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);
			//衝突していなければ飛ばす
			if (!isCollision) { continue; }

			//自機のダメージ用コールバック関数を呼び出す
			player->OnCollisionDamage(posB);
			//カメラをシェイクさせる
			gameCamera->ShakeStart();
		}
	}
#pragma endregion

#pragma region 自機と敵弾の衝突判定
	//自機が緊急回避をしていなければ判定する
	if (!player->GetIsRoll()) {
		//自機座標
		posA = player->GetWorldPos();
		//自機半径
		radiusA = player->GetScale().x;

		//自機と全ての敵弾の衝突判定
		for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
			//自機がダメージ状態なら飛ばす
			if (player->GetIsDamage()) { continue; }

			//敵弾座標
			posB = bullet->GetWorldPos();
			//敵弾半径
			radiusB = bullet->GetScale().x;

			//球と球の衝突判定を行う
			bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);
			//衝突していなければ飛ばす
			if (!isCollision) { continue; }

			//自機のダメージ用コールバック関数を呼び出す
			player->OnCollisionDamage(posB);
			//敵弾のコールバック関数を呼び出す
			bullet->OnCollision();
			//カメラをシェイクさせる
			gameCamera->ShakeStart();
		}
	}
#pragma endregion

#pragma region 自機と回復アイテムの衝突判定
	//自機が墜落状態でなければ判定する
	if (!player->GetIsCrash()) {
		//自機座標
		posA = player->GetWorldPos();
		//自機半径
		radiusA = player->GetScale().x;

		//自機と全ての回復アイテムの衝突判定
		for (const std::unique_ptr<HealingItem>& healingItem : healingItems) {
			//既に接触していたら飛ばす
			if (healingItem->GetIsTouched()) { continue; }

			//回復アイテム座標
			posB = healingItem->GetWorldPos();
			//回復アイテム半径
			radiusB = healingItem->GetScale().x;

			//球と球の衝突判定を行う
			bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);
			//衝突していなければ飛ばす
			if (!isCollision) { continue; }

			//自機の回復用コールバック関数を呼び出す
			player->OnCollisionHeal();
			//回復アイテムのコールバック関数を呼び出す
			healingItem->OnCollision();
		}
	}
#pragma endregion

	//ボスの存在がなければこの先の処理は行わない
	if (!boss) { return; }

#pragma region 自機とボス分身の衝突判定
	//自機が緊急回避をしていなければ判定する
	if (!player->GetIsRoll()) {
		//自機座標
		posA = player->GetWorldPos();
		//自機半径
		radiusA = player->GetScale().x;

		//ボス分身のリストを持ってくる
		const std::list<std::unique_ptr<BossAvatar>>& bossAvatars = boss->GetAvatars();
		for (const std::unique_ptr<BossAvatar>& bossAvatar : bossAvatars) {
			//自機がダメージ状態なら飛ばす
			if (player->GetIsDamage()) { continue; }

			//ボス分身座標
			posB = bossAvatar->GetWorldPos();
			//ボス分身半径
			radiusB = bossAvatar->GetScale().x;

			//球と球の衝突判定を行う
			bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);
			//衝突していなければ飛ばす
			if (!isCollision) { continue; }

			//自機のダメージ用コールバック関数を呼び出す
			player->OnCollisionDamage(posB);
			//カメラをシェイクさせる
			gameCamera->ShakeStart();
		}
	}
#pragma endregion

#pragma region 自機弾とボス本体の衝突判定
	//本体座標
	posA = boss->GetMainBody()->GetWorldPos();
	//本体半径
	radiusA = boss->GetMainBody()->GetScale().x;

	//全て自機弾とボス本体の衝突判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//自機弾座標
		posB = bullet->GetWorldPos();
		//自機弾弾半径
		radiusB = bullet->GetScale().x;

		//球と球の衝突判定を行う
		bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);
		//衝突していなければ飛ばす
		if (!isCollision) { continue; }

		//ボスのコールバック関数を呼び出す
		const int attackPower = 2;
		boss->OnCollisionMainBody(attackPower);
		//自機弾のコールバック関数を呼び出す
		bullet->OnCollision();

		break;
	}
#pragma endregion

#pragma region 自機弾とボス分身の衝突判定
	//全てのボス分身と全ての自機弾の衝突判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//自機弾座標
		posA = bullet->GetWorldPos();
		//自機弾半径
		radiusA = bullet->GetScale().x;

		//ボス分身のリストを持ってくる
		const std::list<std::unique_ptr<BossAvatar>>& bossAvatars = boss->GetAvatars();
		for (const std::unique_ptr<BossAvatar>& bossAvatar : bossAvatars) {
			//ボス分身座標
			posB = bossAvatar->GetWorldPos();
			//ボス分身半径(親子構造の為、本体の大きさ×分身の大きさで正しい大きさが分かる)
			radiusB = boss->GetMainBody()->GetScale().x * bossAvatar->GetScale().x;

			//球と球の衝突判定を行う
			bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);
			//衝突していなければ飛ばす
			if (!isCollision) { continue; }

			//ボスのコールバック関数を呼び出す
			const int attackPower = 2;
			boss->OnCollisionAvatar(bossAvatar.get(), attackPower);
			//自機弾のコールバック関数を呼び出す
			bullet->OnCollision();

			break;
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
	//自機がチャージショット状態なら
	if (player->GetIsChargeShotMode()) {
		//レティクルがロックオン状態なら抜ける
		if (player->GetReticles()->GetIsLockon()) { return; }

		//レティクル座標
		posA = player->GetReticles()->GetFarReticleScreenPosition();
		//レティクル半径
		radiusA = player->GetReticles()->GetFarReticleSize();

		//レティクルと全ての敵の衝突判定
		for (const std::unique_ptr<Enemy>& enemy : enemys) {
			//敵が死亡状態なら飛ばす
			if (enemy->GetIsDead()) { continue; }

			//敵のワールド座標が自機のワールド座標より手前にいたら判定しないで飛ばす
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
			//衝突していなければ飛ばす
			if (!isCollision) { continue; }

			//レティクルのロックオン対象を確定させる
			player->GetReticles()->LockonEnemy(enemy.get());
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
				newEnemy.reset(DemoEnemy::Create(modelEnemyFighter.get(), { x, y, z }, { velX, velY, velZ }));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::Cannon) {
				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(CannonEnemy::Create(modelEnemyFighter.get(), { x, y, z }));
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
				newEnemy.reset(CircularEnemy::Create(modelEnemyFighter.get(), { x, y, z }, angle, length, rotSpeed));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::Fall) {
				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(FallEnemy::Create(modelEnemyFighter.get(), { x, y, z }));
				enemys.push_back(std::move(newEnemy));
			}
			else if (type == Enemy::EnemyType::UpDown) {
				std::unique_ptr<Enemy> newEnemy;
				newEnemy.reset(UpDownEnemy::Create(modelEnemyFighter.get(), { x, y, z }));
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
				newEnemy.reset(ComeGoEnemy::Create(modelEnemyFighter.get(), { x, y, z }, { comeX, comeY, comeZ }, { goX, goY, goZ }, time));
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

void GameScene::BossBattleStart()
{
	//既にボスバトルなら抜ける
	if (isBossBattle) { return; }

	//自機がボスバトル開始とする座標まで進んだら開始
	const float isBossBattleStartPos = 350;
	const bool isBossBattleStart = player->GetWorldPos().z >= isBossBattleStartPos;
	if (!isBossBattleStart) { return; }

	//ボス生成
	const float distance = 60;
	const Vector3 bossBasePos = { 0, 20, isBossBattleStartPos + distance };
	boss.reset(Boss::Create(bossBasePos));

	//カメラの前進を止める
	gameCamera->SetIsAdvance(false);

	//ボスバトル状態にする
	isBossBattle = true;
}

void GameScene::StageClear()
{
	//ステージクリアでないとき
	if (!isStageClear) {
		//そもそもボスがいなければ抜ける
		if (!boss) { return; }
		//ボスが死亡していなければ抜ける
		if (!boss->GetIsDead()) { return; }

		//ステージクリア
		isStageClear = true;
		//自機をステージクリアの動きに変更
		player->StageClearModeStart();
		//カメラをステージクリアの動きに変更
		gameCamera->StageClearModeStart(boss->GetMainBody());
		//ハイスコア更新
		EnemyDefeatCounter::CheckHighScore();
	}
	//ステージクリア後
	else {
		//ステージリザルト
		StageResult();
	}
}

void GameScene::StageResult()
{
	//ステージクリアテキスト生成と解放
	StageClearTextCreateAndRelease();

	//ステージリザルトUI生成と解放
	StageResultUICreateAndRelease();

	//タイトルシーンに戻る
	ReturnTitleScene();

}

void GameScene::StageClearTextCreateAndRelease()
{
	//ステージクリアテキストのインスタンスがないとき
	if (!stageClearText) {
		//カメラのステージクリア後行動が自機をズームでなければ抜ける
		if (!(gameCamera->GetStageClearModePhase() == GameCamera::StageClearModePhase::PlayerZoom)) { return; }

		//ステージクリアテキスト生成
		stageClearText.reset(StageClearText::Create());
	}
	//ステージクリアテキストのインスタンスがあるとき
	else {
		//カメラのステージクリア後行動が自機の方向をずっと向くでなければ抜ける
		if (!(gameCamera->GetStageClearModePhase() == GameCamera::StageClearModePhase::PlayerFollow)) { return; }

		//ステージクリアテキストの解放
		stageClearText.reset();
	}
}

void GameScene::StageResultUICreateAndRelease()
{
	//ステージリザルトUIのインスタンスがないとき
	if (!stageResultUI) {
		//カメラのステージクリア後行動が自機横に移動でなければ抜ける
		if (!(gameCamera->GetStageClearModePhase() == GameCamera::StageClearModePhase::PlayerSideMove)) { return; }
		//自機のステージクリア後行動が停止でなければ抜ける
		if (!(player->GetStageClearModePhase() == Player::StageClearModePhase::Stay)) { return; }

		//ステージリザルトUI生成
		stageResultUI.reset(StageResultUI::Create(EnemyDefeatCounter::GetDefeatCount(), false));
	}
	//ステージリザルトUIのインスタンスがあるとき
	else {
		//リザルト表示が完了していなければ抜ける
		if (!stageResultUI->GetIsResultEnd()) { return; }
		//カメラのステージクリア後行動が自機の方向をずっと向くでなければ抜ける
		if (!(gameCamera->GetStageClearModePhase() == GameCamera::StageClearModePhase::PlayerKeepLook)) { return; }

		//リザルト終了のためボタンスプライトを表示する
		if (!stageResultUI->GetIsDrawButtonSprite()) {
			stageResultUI->SetIsDrawButtonSprite(true);
		}

		//指定の入力をしなければ抜ける
		if (!(Input::GetInstance()->TriggerKey(DIK_SPACE) || Input::GetInstance()->TriggerGamePadButton(Input::PAD_B))) { return; }

		//自機のステージクリア後ブーストを開始する
		player->StageClearBoostStart();

		//ステージリザルトUIの解放
		stageResultUI.reset();
	}
}

void GameScene::ReturnTitleScene()
{
	//自機のステージクリア後行動が完了していなければ抜ける
	if (!player->GetIsStageClearModeCompletion()) { return; }

	//タイトルシーンへ
	SceneManager::GetInstance()->ChangeScene("TITLE");
}

void GameScene::GameOver()
{
	//ゲームオーバーでないとき
	if (!isGameOver) {
		//自機が墜落状態でなければ抜ける
		if (!player->GetIsCrash()) { return; }

		//ゲームオーバー
		isGameOver = true;
		//カメラを墜落状態にする
		gameCamera->CrashStart();
	}
	//ゲームオーバーのとき
	else {
		//自機が死亡していなければ抜ける
		if (!player->GetIsDead()) { return; }

		//タイマー更新
		const float gameOverTime = 120;
		gameOverTimer++;

		//タイマーが指定した時間になったらゲームシーンをやり直す
		if (gameOverTimer >= gameOverTime) {
			SceneManager::GetInstance()->ChangeScene("GAME");
		}
	}
}
