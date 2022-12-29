#include "Stage01Scene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Easing.h"
#include "ParticleEmitter.h"
#include "HomingBullet.h"
#include "CannonEnemy.h"
#include "CircularEnemy.h"
#include "FallEnemy.h"
#include "UpDownEnemy.h"
#include "ComeGoEnemy.h"
#include "EnemyDefeatCounter.h"
#include "SceneChangeEffect.h"
#include "GamePostEffect.h"
#include <cassert>
#include <fstream>
#include <iomanip>

using namespace DirectX;


void Stage01Scene::Initialize()
{
	//ライト生成
	lightGroup.reset(LightGroup::Create());
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, false);
	//光線方向初期値
	float lightDir0[3] = { 2,-1,1 };
	float lightColor0[3] = { 0.9f,0.9f,0.9f };
	float lightDir1[3] = { -2,-1,-6 };
	float lightColor1[3] = { 0.3f,0.3f,0.3f };
	for (int i = 0; i < 3; i++) {
		this->lightDir0[i] = lightDir0[i];
		this->lightColor0[i] = lightColor0[i];
		this->lightDir1[i] = lightDir1[i];
		this->lightColor1[i] = lightColor1[i];
	}

	//objからモデルデータを読み込む
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydomeStage01"));
	modelGround.reset(ObjModel::LoadFromOBJ("ground"));
	modelBuilding[0].reset(ObjModel::LoadFromOBJ("building01"));
	modelBuilding[1].reset(ObjModel::LoadFromOBJ("building02"));
	modelSphere.reset(ObjModel::LoadFromOBJ("sphere", true));
	modelPlayerBullet.reset(ObjModel::LoadFromOBJ("playerBullet", true));
	modelFighter.reset(ObjModel::LoadFromOBJ("fighter"));
	modelEnemyBullet.reset(ObjModel::LoadFromOBJ("enemyBullet", true));
	modelEnemyFighter.reset(ObjModel::LoadFromOBJ("enemyFighter"));
	modelEnemyFighterBreak[0].reset(ObjModel::LoadFromOBJ("enemyFighterBreak01"));
	modelEnemyFighterBreak[1].reset(ObjModel::LoadFromOBJ("enemyFighterBreak02"));
	modelEnemyFighterBreak[2].reset(ObjModel::LoadFromOBJ("enemyFighterBreak03"));
	modelEnemyFighterBreak[3].reset(ObjModel::LoadFromOBJ("enemyFighterBreak04"));
	modelEnemyFighterBreak[4].reset(ObjModel::LoadFromOBJ("enemyFighterBreak05"));
	modelEnemyMiniRobot.reset(ObjModel::LoadFromOBJ("enemyMiniRobot", true));
	modelEnemyMiniRobotBreak[0].reset(ObjModel::LoadFromOBJ("enemyMiniRobotBreak01"));
	modelEnemyMiniRobotBreak[1].reset(ObjModel::LoadFromOBJ("enemyMiniRobotBreak02"));
	modelEnemyMiniRobotBreak[2].reset(ObjModel::LoadFromOBJ("enemyMiniRobotBreak03"));
	modelEnemyMiniRobotBreak[3].reset(ObjModel::LoadFromOBJ("enemyMiniRobotBreak04"));
	modelEnemyMiniRobotBreak[4].reset(ObjModel::LoadFromOBJ("enemyMiniRobotBreak05"));
	modelBossMainBody.reset(ObjModel::LoadFromOBJ("bossMainBody", true));
	modelBossMainBodyDamage.reset(ObjModel::LoadFromOBJ("bossMainBodyDamage", true));
	modelBossMainBodySleep.reset(ObjModel::LoadFromOBJ("bossMainBodySleep", true));
	modelBossMainBodyDead.reset(ObjModel::LoadFromOBJ("bossMainBodyDead", true));
	modelBossAvatar.reset(ObjModel::LoadFromOBJ("bossAvatar", true));
	modelBossAvatarDamage.reset(ObjModel::LoadFromOBJ("bossAvatarDamage", true));
	modelBossAvatarSleep.reset(ObjModel::LoadFromOBJ("bossAvatarSleep", true));
	modelBossAvatarDead.reset(ObjModel::LoadFromOBJ("bossAvatarDead", true));
	modelHealingItem.reset(ObjModel::LoadFromOBJ("healingItem"));

	//ポストエフェクトのブラーを解除しておく
	GamePostEffect::GetPostEffect()->SetRadialBlur(false);

	//自機に必要な情報をセット
	BasePlayer::SetStageScene(this);
	BasePlayer::SetBulletModel(modelPlayerBullet.get()); //通常弾用モデルをセット
	PlayerBullet::SetIsGroundMode(true); //自機弾の地面あり行動をONにする
	HomingBullet::SetBulletModel(modelSphere.get()); //ホーミング弾用モデルをセット
	//自機生成
	const int maxHP = 101; //最大HP
	const int startHP = maxHP / 2 + 1; //開始時HP
	player.reset(Stage01Player::Create(modelFighter.get(), startHP, maxHP));
	player->SetIsCameraFollow(true);
	

	//ゲームカメラ初期化
	gameCamera.reset(new Stage01GameCamera());
	gameCamera->Initialize(player.get());
	//影用光源カメラ初期化
	lightCamera.reset(new LightCamera());
	lightCamera->Initialize({ -300, 100, -300 });
	lightCamera->SetProjectionNum({ 100, 200 }, { -600, -200 });
	//頭上からの影用光源カメラ初期化
	topLightCamera.reset(new LightCamera());
	topLightCamera->Initialize({ -300, 100, -300 });
	topLightCamera->SetProjectionNum({ 150, 150 }, { -150, -150 });


	//全敵初期化処理
	InitializeEnemy();

	//ボスに必要な情報をセット
	Boss::SetPlayer(player.get());
	BossMainBody::SetStageScene(this);
	BossMainBody::SetBossMainBodyModel(modelBossMainBody.get());
	BossMainBody::SetBossMainBodyDamageModel(modelBossMainBodyDamage.get());
	BossMainBody::SetBossMainBodySleepModel(modelBossMainBodySleep.get());
	BossMainBody::SetBossMainBodyDeadModel(modelBossMainBodyDead.get());
	BossMainBody::SetBulletModel(modelEnemyBullet.get());
	BossAvatar::SetStageScene(this);
	BossAvatar::SetAvatarModel(modelBossAvatar.get());
	BossAvatar::SetAvatarDamageModel(modelBossAvatarDamage.get());
	BossAvatar::SetAvatarSleepModel(modelBossAvatarSleep.get());
	BossAvatar::SetAvatarDeadModel(modelBossAvatarDead.get());
	BossAvatar::SetBulletModel(modelEnemyBullet.get());

	//回復アイテムに必要な情報をセット
	HealingItem::SetPlayer(player.get());

	//天球生成
	skydome.reset(Skydome::Create(modelSkydome.get()));
	//見栄えがいい角度に変更しておく
	skydome->SetRotation({ 0, 190, 0 });

	//地面生成
	gameGroundManager.reset(GameGroundManager::Create(modelGround.get()));
	//地面に必要な情報をセット
	GameGroundManager::SetPlayer(player.get());
	GameGroundManager::SetGameCamera(gameCamera.get());

	//ビルに必要な情報をセット
	//モデルをセット
	for (int i = 0; i < modelBuilding.size(); i++) {
		//モデルが未設定なら飛ばす
		if (!modelBuilding[i]) { continue; }
		GameBuildingManager::SetBuidingModel(i, modelBuilding[i].get());
	}
	GameBuildingManager::SetPlayer(player.get());
	GameBuildingManager::SetGameCamera(gameCamera.get());
	//背景用(ゲーム用ビル管理)生成
	gameBuildingManager.reset(GameBuildingManager::Create(85, 25, 25, { 0, 0, -20 }));

	//objオブジェクトにカメラをセット
	ObjObject3d::SetCamera(gameCamera.get());
	ObjObject3d::SetLightCamera(lightCamera.get());
	ObjObject3d::SetTopLightCamera(topLightCamera.get());
	//objオブジェクトにライトをセット
	ObjObject3d::SetLightGroup(lightGroup.get());


	//パーティクルにカメラをセット
	ParticleManager::SetCamera(gameCamera.get());
	//画面にパーティクルが残ることがあるので全て削除しておく
	ParticleEmitter::GetInstance()->AllDelete();

	//遊び方UI生成
	howToPlayUI.reset(HowToPlayUI::Create());
	//ステージ開始UI生成
	stageStartUI.reset(StageStartUI::Create(1));

	//敵を倒した数カウンターリセット
	EnemyDefeatCounter::CounterReset();
}

void Stage01Scene::Update()
{
	//入力のインスタンスを取得
	Input* input = Input::GetInstance();
	//デバッグテキストのインスタンスを取得
	DebugText* debugText = DebugText::GetInstance();

	//オブジェクト解放
	ObjectRelease();
	//敵発生コマンド更新
	UpdateEnemySetCommands(gameCamera->GetPosition());
	//遊び方UI表示
	HowToPlay();
	//ボスバトル開始判定処理
	BossBattleStart();
	//ステージクリア
	StageClear();
	//ゲームオーバー
	GameOver();

	//カメラ更新
	gameCamera->Update();
	//影生成用ライトカメラ
	LightCameraUpdate();

	//ライト更新
	lightGroup->SetAmbientColor(XMFLOAT3(ambientColor0));
	lightGroup->SetDirLightDir(0, XMVECTOR({ lightDir0[0], lightDir0[1], lightDir0[2], 0 }));
	lightGroup->SetDirLightColor(0, XMFLOAT3(lightColor0));
	lightGroup->SetDirLightDir(1, XMVECTOR({ lightDir1[0], lightDir1[1], lightDir1[2], 0 }));
	lightGroup->SetDirLightColor(1, XMFLOAT3(lightColor1));
	lightGroup->Update();

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
	//敵破壊エフェクト
	for (const std::unique_ptr<EnemyBreakEffect>& breakEffect : enemyBreakEffects) {
		breakEffect->Update();
	}
	//回復アイテム
	for (const std::unique_ptr<HealingItem>& healingItem : healingItems) {
		healingItem->Update();
	}
	//天球
	skydome->Update();
	//地面
	gameGroundManager->Update();
	//背景用(ビル)
	gameBuildingManager->Update();

	//3D衝突判定管理
	CollisionCheck3d();

	//スプライト更新
	//自機のUI
	player->UpdateUI();
	//複数体処理用UI 
	for (const std::unique_ptr<MultiHitUI>& multiHitUI : multiHitUIs) {
		multiHitUI->Update();
	}
	//ステージ開始UI
	if (stageStartUI) {
		stageStartUI->Update();
		//死亡したら解放
		if (stageStartUI->GetIsDead()) {
			stageStartUI.reset();
		}
	}
	//遊び方UI 
	if (howToPlayUI) {
		howToPlayUI->Update();
	}
	//ボス登場警告演
	if (bossWarning) {
		bossWarning->Update();
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


	//ボス死亡後の演出
	if (bossDeadEffect) {
		bossDeadEffect->Update();
	}
	//パーティクル更新
	ParticleEmitter::GetInstance()->Update();

	//シーン変更状態
	SceneChangeMode();

	//シーン変更演出更新
	SceneChangeEffect::Update();
}

void Stage01Scene::DrawBackSprite()
{
}

void Stage01Scene::Draw3D()
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
	//敵破壊エフェクト
	for (const std::unique_ptr<EnemyBreakEffect>& breakEffect : enemyBreakEffects) {
		breakEffect->Draw();
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
	gameGroundManager->Draw();
	//背景用(ビル)
	gameBuildingManager->Draw();

	///-------Object3d描画ここまで-------///

	///-------パーティクル描画ここから-------///

	//パーティクル描画
	ParticleEmitter::GetInstance()->DrawAll();

	///-------パーティクル描画ここまで-------///
}

void Stage01Scene::Draw3DLightView()
{
	//Object3d共通コマンド
	ObjObject3d::DrawLightViewPrev();
	///-------Object3d描画ここから-------///

	//背景用(ビル)
	gameBuildingManager->DrawLightCameraView();

	///-------Object3d描画ここまで-------///
}

void Stage01Scene::Draw3DTopLightView()
{
	//Object3d共通コマンド
	ObjObject3d::DrawLightViewPrev();
	///-------Object3d描画ここから-------///

	//自機
	player->DrawTopLightCameraView();
	//自機弾
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		bullet->DrawTopLightCameraView();
	}
	//敵
	for (const std::unique_ptr<Enemy>& enemy : enemys) {
		enemy->DrawTopLightCameraView();
	}
	//敵弾
	for (const std::unique_ptr<EnemyBullet>& bullet : enemyBullets) {
		bullet->DrawTopLightCameraView();
	}
	//敵破壊エフェクト
	for (const std::unique_ptr<EnemyBreakEffect>& breakEffect : enemyBreakEffects) {
		breakEffect->DrawTopLightCameraView();
	}
	//ボス
	if (boss) {
		boss->DrawTopLightCameraView();
	}
	//回復アイテム
	for (const std::unique_ptr<HealingItem>& healingItem : healingItems) {
		healingItem->DrawTopLightCameraView();
	}

	///-------Object3d描画ここまで-------///
}

void Stage01Scene::DrawFrontSprite()
{
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
	//ステージ開始UI
	/*if (stageStartUI) {
		stageStartUI->Draw();
	}*/
	//遊び方UI 
	if (howToPlayUI) {
		howToPlayUI->Draw();
	}
	//ステージクリアテキスト
	if (stageClearText) {
		stageClearText->Draw();
	}
	//ステージリザルトUI
	if (stageResultUI) {
		stageResultUI->Draw();
	}
	//ボス登場警告演出
	if (bossWarning) {
		bossWarning->Draw();
	}

	//シーン変更演出描画
	SceneChangeEffect::Draw();

	///-------スプライト描画ここまで-------///
}

void Stage01Scene::LightCameraUpdate()
{
	//ターゲットになる座標
	const Vector3 targetPos = gameCamera->GetPosition();
	//ライトカメラ用の視点
	const Vector3 lightEye = targetPos + lightCameraTargetDistance;
	lightCamera->SetEyeTarget(lightEye, targetPos);
	lightCamera->Update();


	//頭上からのライトカメラ用ターゲットと視点の距離
	const Vector3 topCameraTargetDistance = { 0, 500, -350 };
	//頭上からのライトカメラ用の視点
	const Vector3 topLightEye = targetPos + topCameraTargetDistance;
	topLightCamera->SetEyeTarget(topLightEye, targetPos);
	topLightCamera->Update();
}

void Stage01Scene::ObjectRelease()
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
		const float healingItemSize = 2.0f;
		Vector3 healingItemPos = enemy->GetPosition();
		//生成位置を地面にめり込まないようにする
		if (healingItemPos.y < healingItemSize) { healingItemPos.y = healingItemSize; }
		healingItem.reset(HealingItem::Create(modelHealingItem.get(), healingItemPos, healingItemSize));
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

	//削除状態の敵破壊エフェクトの削除
	enemyBreakEffects.remove_if([](std::unique_ptr<EnemyBreakEffect>& breakEffect) {
		return breakEffect->GetIsDelete();
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
			//地面のスクロール状態を解除
			gameGroundManager->SetIsScroll(false);
			//ビルのスクロール状態を解除
			gameBuildingManager->SetIsScroll(false);
			//後ろ側にビルを大量設置
			gameBuildingManager->CreateBehindObjects();
			//死亡後演出を生成
			bossDeadEffect.reset(BossDeadEffect::Create(boss->GetMainBody()->GetWorldPos()));
			//カメラをシェイクさせる
			const float shakePower = 25;
			const float shakeTime = 80;
			gameCamera->ShakeStart(shakePower, shakeTime);

			//解放
			boss.reset();
		}
	}

	//死亡した複数体処理用UIの削除
	multiHitUIs.remove_if([](std::unique_ptr<MultiHitUI>& multiHitUI) {
		return multiHitUI->GetIsDead();
		});
}

void Stage01Scene::CollisionCheck3d()
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
			enemy->OnCollision(bullet->GetDamageNum());
			//自機弾のコールバック関数を呼び出す
			bullet->OnCollision(posB, radiusB);

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
			const int damageNum = 1; //破裂のダメージ量は1に固定
			enemy->OnCollision(damageNum);
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
	//自機が緊急回避をしていない && ダメージノックバック状態でなければ判定する
	if (!player->GetIsRoll() && !player->GetIsDamageKnockback()) {
		//自機座標
		posA = player->GetWorldPos();
		//自機半径
		radiusA = player->GetScale().x;

		//自機と全ての敵の衝突判定
		for (const std::unique_ptr<Enemy>& enemy : enemys) {
			//敵が死亡していたら飛ばす
			if (enemy->GetIsDead()) { continue; }

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
			//自機がダメージノックバック状態なら飛ばす
			if (player->GetIsDamageKnockback()) { continue; }

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
			//回復アイテム半径 + 触れやすくするための余白
			const float addScale = 1.0f;
			radiusB = healingItem->GetScale().x + addScale;

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
			//自機がダメージノックバック状態なら飛ばす
			if (player->GetIsDamageKnockback()) { continue; }

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
		boss->OnCollisionMainBody(bullet->GetDamageNum(), posB, bullet->GetVelocity());
		//自機弾のコールバック関数を呼び出す
		//ダメージが通ったとき
		if (boss->GetMainBody()->GetIsDamageTrigger()) { bullet->OnCollision(posA, radiusA); }
		//ダメージが通らなかったとき
		else { bullet->OnCollision(posA, radiusA, false); }

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
			//ボス分身半径
			radiusB = bossAvatar->GetScale().x;
			//分身生存時は親子構造の為、本体の大きさを乗算して正しい大きさが分かる
			if (!bossAvatar->GetIsDead()) { radiusB *= boss->GetMainBody()->GetScale().x; }

			//球と球の衝突判定を行う
			bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);
			//衝突していなければ飛ばす
			if (!isCollision) { continue; }

			//ボスのコールバック関数を呼び出す
			boss->OnCollisionAvatar(bossAvatar.get(), bullet->GetDamageNum(), posA, bullet->GetVelocity());
			//自機弾のコールバック関数を呼び出す
			//ダメージが通ったとき
			if (bossAvatar->GetIsDamageTrigger()) { bullet->OnCollision(posB, radiusB); }
			//ダメージが通らなかったとき
			else { bullet->OnCollision(posB, radiusB, false); }

			break;
		}
	}
#pragma endregion
}

void Stage01Scene::CollisionCheck2d()
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

void Stage01Scene::InitializeEnemy()
{
	//敵配置スクリプトの読み込み
	LoadEnemySetData(enemySetCommands, "Resources/csv/EnemySetStage01.csv");

	//全敵に必要な情報をセット
	Enemy::SetStageScene(this); //全敵にステージシーンを教える
	Enemy::SetPlayer(player.get()); //自機をセット
	Enemy::SetGameCamera(gameCamera.get()); //ゲームカメラをセット
	Enemy::SetBulletModel(modelEnemyBullet.get()); //弾のモデルをセット
	Enemy::SetIsGroundMode(true); //地面あり行動をONにする
	EnemyBreakEffect::SetIsGroundMode(true); //破壊エフェクトの地面あり行動をONにする
	EnemyBreakEffect::SetIsGravityMode(true); //破壊エフェクトの重力あり行動をONにする
	EnemyBreakEffect::SetGameCamera(gameCamera.get()); //破壊エフェクトにゲームカメラをセット

	//各種類の敵に必要な情報をセット
	//大砲敵
	CannonEnemy::SetModel(modelEnemyMiniRobot.get()); //モデルをセット
	//破壊時に出すモデルをセット
	for (int i = 0; i < modelEnemyMiniRobotBreak.size(); i++) {
		//モデルが未設定なら飛ばす
		if (!modelEnemyMiniRobotBreak[i]) { continue; }
		CannonEnemy::SetBreakModel(i, modelEnemyMiniRobotBreak[i].get());
	}

	//円運動敵
	CircularEnemy::SetModel(modelEnemyFighter.get()); //モデルをセット
	//破壊時に出すモデルをセット
	for (int i = 0; i < modelEnemyFighterBreak.size(); i++) {
		//モデルが未設定なら飛ばす
		if (!modelEnemyFighterBreak[i]) { continue; }
		CircularEnemy::SetBreakModel(i, modelEnemyFighterBreak[i].get());
	}

	//降下敵
	FallEnemy::SetModel(modelEnemyMiniRobot.get()); //モデルをセット
	//破壊時に出すモデルをセット
	for (int i = 0; i < modelEnemyMiniRobotBreak.size(); i++) {
		//モデルが未設定なら飛ばす
		if (!modelEnemyMiniRobotBreak[i]) { continue; }
		FallEnemy::SetBreakModel(i, modelEnemyMiniRobotBreak[i].get());
	}

	//上下移動敵
	UpDownEnemy::SetModel(modelEnemyFighter.get()); //モデルをセット
	//破壊時に出すモデルをセット
	for (int i = 0; i < modelEnemyFighterBreak.size(); i++) {
		//モデルが未設定なら飛ばす
		if (!modelEnemyFighterBreak[i]) { continue; }
		UpDownEnemy::SetBreakModel(i, modelEnemyFighterBreak[i].get());
	}

	//到着出発敵
	ComeGoEnemy::SetAttackMoveSpeed(BaseGameCamera::GetAdvanceSpeed()); //移動スピードをセット
	ComeGoEnemy::SetModel(modelEnemyFighter.get()); //モデルをセット
	//破壊時に出すモデルをセット
	for (int i = 0; i < modelEnemyFighterBreak.size(); i++) {
		//モデルが未設定なら飛ばす
		if (!modelEnemyFighterBreak[i]) { continue; }
		ComeGoEnemy::SetBreakModel(i, modelEnemyFighterBreak[i].get());
	}
}

void Stage01Scene::HowToPlay()
{
	//次に描画する遊び方UIがない場合は抜ける
	if (howToPlayUI->GetNextDrawUI() == HowToPlayUI::DrawUI::None) { return; }

	//次に描画する遊び方UIが「ショット」の場合
	if (howToPlayUI->GetNextDrawUI() == HowToPlayUI::DrawUI::Shot) {

		//カメラのZ座標が指定した値以下なら抜ける
		const float createPos = 15;
		if (gameCamera->GetPosition().z <= createPos) { return; }

		//遊び方UI(ショット)生成
		howToPlayUI->CreateUI(HowToPlayUI::DrawUI::Shot, HowToPlayUI::DrawUI::Charge);
	}
	//次に描画する遊び方UIが「チャージ」の場合
	else if (howToPlayUI->GetNextDrawUI() == HowToPlayUI::DrawUI::Charge) {
		//カメラのZ座標が指定した値以下なら抜ける
		const float createPos = 550;
		if (gameCamera->GetPosition().z <= createPos) { return; }

		//遊び方UI(チャージ)生成
		howToPlayUI->CreateUI(HowToPlayUI::DrawUI::Charge, HowToPlayUI::DrawUI::Rolling);
	}
	//次に描画する遊び方UIが「ローリング」の場合
	else if (howToPlayUI->GetNextDrawUI() == HowToPlayUI::DrawUI::Rolling) {
		//カメラのZ座標が指定した値以下なら抜ける
		const float createPos = 1020;
		if (gameCamera->GetPosition().z <= createPos) { return; }

		//遊び方UI(ローリング)生成
		howToPlayUI->CreateUI(HowToPlayUI::DrawUI::Rolling, HowToPlayUI::DrawUI::Boost);
	}
	//次に描画する遊び方UIが「ブースト」の場合
	else if (howToPlayUI->GetNextDrawUI() == HowToPlayUI::DrawUI::Boost) {
		//カメラのZ座標が指定した値以下なら抜ける
		const float createPos = 1780;
		if (gameCamera->GetPosition().z <= createPos) { return; }

		//遊び方UI(ブースト)生成
		howToPlayUI->CreateUI(HowToPlayUI::DrawUI::Boost, HowToPlayUI::DrawUI::Brake);
	}
	//次に描画する遊び方UIが「ブレーキ」の場合
	else if (howToPlayUI->GetNextDrawUI() == HowToPlayUI::DrawUI::Brake) {
		//カメラのZ座標が指定した値以下なら抜ける
		const float createPos = 2460;
		if (gameCamera->GetPosition().z <= createPos) { return; }

		//遊び方UI(ブレーキ)生成
		howToPlayUI->CreateUI(HowToPlayUI::DrawUI::Brake, HowToPlayUI::DrawUI::None);
	}
}

void Stage01Scene::BossBattleStart()
{
	//既にボスバトルなら抜ける
	if (isBossBattle) { return; }

	//ボスバトル開始座標
	const float bossBattleStartPos = 3200;

	//警告開始判定
	if (!bossWarning) {
		//カメラがボスバトル開始とする座標まで進んだら開始
		const bool isBossBattleStart = gameCamera->GetPosition().z >= bossBattleStartPos;
		if (!isBossBattleStart) { return; }

		//カメラの前進を止める
		gameCamera->SetIsAdvance(false);

		//地面をスクロール状態にする
		gameGroundManager->SetIsScroll(true);
		//ビルをスクロール状態にする
		gameBuildingManager->SetIsScroll(true);

		//ボス登場警告演出生成
		const int warningTime = 300;
		bossWarning.reset(BossWarning::Create(warningTime));
	}
	//警告演出中の処理
	else {
		//ボス登場警告演出が削除状態でなければ抜ける
		if (!bossWarning->GetIsDelete()) { return; }

		//ボス登場警告演出はもう使用しないので解放
		bossWarning.reset();

		//ボス生成
		const float distance = 75;
		const Vector3 bossBasePos = { 0, 23, bossBattleStartPos + distance };
		boss.reset(Boss::Create(bossBasePos));

		//ボスバトル状態にする
		isBossBattle = true;
	}
}

void Stage01Scene::StageClear()
{
	//ステージクリアでないとき
	if (!isStageClear) {
		//天球をカメラに追従させる
		skydome->SetPosition({ 0, 0, gameCamera->GetPosition().z });

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
		EnemyDefeatCounter::CheckHighScore(0);
	}
	//ステージクリア後
	else {
		//ステージリザルト
		StageResult();
	}
}

void Stage01Scene::StageResult()
{
	//ステージクリア時の影の向きに変更
	StageClearSetLightCameraPos();

	//ステージクリアテキスト生成と解放
	StageClearTextCreateAndRelease();

	//ステージリザルトUI生成と解放
	StageResultUICreateAndRelease();

	//タイトルシーンに戻る
	ReturnTitleScene();

}

void Stage01Scene::StageClearSetLightCameraPos()
{
	//既にステージクリア用影状態なら抜ける
	if (isStageClearShadow) { return; }
	//自機のステージクリア後行動が上昇でなければ抜ける
	if (!(player->GetStageClearModePhase() == Stage01Player::StageClearModePhase::Up)) { return; }

	//ステージクリア用影状態にする
	isStageClearShadow = true;
	//影用光源カメラを遠ざける
	lightCameraTargetDistance = { -400, 450, -400 };
	lightCamera->SetProjectionNum({ 1300, 1300 }, { -200, -1300 });
}

void Stage01Scene::StageClearTextCreateAndRelease()
{
	//ステージクリアテキストのインスタンスがないとき
	if (!stageClearText) {
		//カメラのステージクリア後行動が自機をズームでなければ抜ける
		if (!(gameCamera->GetStageClearModePhase() == Stage01GameCamera::StageClearModePhase::PlayerZoom)) { return; }

		//ステージクリアテキスト生成
		stageClearText.reset(StageClearText::Create());
	}
	//ステージクリアテキストのインスタンスがあるとき
	else {
		//カメラのステージクリア後行動が自機の方向をずっと向くでなければ抜ける
		if (!(gameCamera->GetStageClearModePhase() == Stage01GameCamera::StageClearModePhase::PlayerFollow)) { return; }

		//ステージクリアテキストの解放
		stageClearText.reset();
	}
}

void Stage01Scene::StageResultUICreateAndRelease()
{
	//ステージリザルトUIのインスタンスがないとき
	if (!stageResultUI) {
		//カメラのステージクリア後行動が自機横に移動でなければ抜ける
		if (!(gameCamera->GetStageClearModePhase() == Stage01GameCamera::StageClearModePhase::PlayerSideMove)) { return; }
		//自機のステージクリア後行動が停止でなければ抜ける
		if (!(player->GetStageClearModePhase() == Stage01Player::StageClearModePhase::Stay)) { return; }

		//ステージリザルトUI生成
		stageResultUI.reset(StageResultUI::Create(EnemyDefeatCounter::GetDefeatCount(), false));
	}
	//ステージリザルトUIのインスタンスがあるとき
	else {
		//リザルト表示が完了していなければ抜ける
		if (!stageResultUI->GetIsResultEnd()) { return; }
		//カメラのステージクリア後行動が自機の方向をずっと向くでなければ抜ける
		if (!(gameCamera->GetStageClearModePhase() == Stage01GameCamera::StageClearModePhase::PlayerKeepLook)) { return; }

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

void Stage01Scene::ReturnTitleScene()
{
	//自機のステージクリア後行動が完了していなければ抜ける
	if (!player->GetIsStageClearModeCompletion()) { return; }

	//ステージ選択へシーン変更を開始する
	SceneChangeStart({ 0,0,0,0 }, 120, 60, 60, "STAGESELECT");
}

void Stage01Scene::GameOver()
{
	//ゲームオーバーでないとき
	if (!isGameOver) {
		//自機が墜落状態でなければ抜ける
		if (!player->GetIsCrash()) { return; }

		//ゲームオーバー
		isGameOver = true;
		//カメラを墜落状態にする
		gameCamera->CrashStart();
		//地面のスクロール状態を解除
		gameGroundManager->SetIsScroll(false);
		//ビルのスクロール状態を解除
		gameBuildingManager->SetIsScroll(false);
	}
	//ゲームオーバーのとき
	else {
		//自機が死亡していなければ抜ける
		if (!player->GetIsDead()) { return; }

		//タイマー更新
		const float gameOverTime = 120;
		gameOverTimer++;

		//タイマーが指定した時間になったらステージ01をやり直す
		if (gameOverTimer >= gameOverTime) {
			SceneChangeStart({ 0,0,0,0 }, 60, 60, 20, "STAGE01");
		}
	}
}
