﻿#include "Stage02Scene.h"
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
#include "MeteoriteEnemy.h"
#include "EnemyDefeatCounter.h"
#include "SceneChangeEffect.h"
#include "GamePostEffect.h"
#include <cassert>

#include <iomanip>

using namespace DirectX;


void Stage02Scene::Initialize()
{
	//ライト生成
	lightGroup.reset(LightGroup::Create());
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, false);
	lightGroup->SetDirLightActive(2, false);
	//光線方向初期値
	float lightDir0[3] = { 2,1,1 };
	float lightDir1[3] = { 0,0,1 };
	float lightDir2[3] = { -2,-1,1 };
	float lightColor[3] = { 1,1,1 };
	for (int i = 0; i < 3; i++) {
		this->lightDir0[i] = lightDir0[i];
		this->lightDir1[i] = lightDir1[i];
		this->lightDir2[i] = lightDir2[i];
		lightColor0[i] = lightColor[i];
		lightColor1[i] = lightColor[i];
		lightColor2[i] = lightColor[i];
	}

	//objからモデルデータを読み込む
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydomeStage02"));
	modelSphere.reset(ObjModel::LoadFromOBJ("sphere", true));
	modelMeteoriteWhite.reset(ObjModel::LoadFromOBJ("meteoriteWhite", true));
	modelMeteoriteBrown.reset(ObjModel::LoadFromOBJ("meteoriteBrown", true));
	modelSpaceDust.reset(ObjModel::LoadFromOBJ("spaceDust", true));
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
	modelMedamanMainBody.reset(ObjModel::LoadFromOBJ("medamanMainBody", true));
	modelHealingItem.reset(ObjModel::LoadFromOBJ("healingItem"));

	//ポストエフェクトのブラーを解除しておく
	GamePostEffect::GetPostEffect()->SetRadialBlur(false);

	//自機に必要な情報をセット
	BasePlayer::SetStageScene(this);
	BasePlayer::SetBulletModel(modelPlayerBullet.get()); //通常弾用モデルをセット
	PlayerBullet::SetIsGroundMode(false); //自機弾の地面あり行動をOFFにする
	HomingBullet::SetBulletModel(modelSphere.get()); //ホーミング弾用モデルをセット
	//自機生成
	const int maxHP = 101; //最大HP
	const int startHP = maxHP / 2 + 1; //開始時HP
	player.reset(Stage02Player::Create(modelFighter.get(), startHP, maxHP, false));
	player->SetIsCameraFollow(true);

	//ゲームカメラ初期化
	gameCamera.reset(new Stage02GameCamera());
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

	//ボス(ボス2)に必要な情報をセット
	Boss2::SetPlayer(player.get());
	Boss2Body::SetStageScene(this);
	Boss2Body::SetBodyModel(modelMedamanMainBody.get());

	//回復アイテムに必要な情報をセット
	HealingItem::SetPlayer(player.get());

	//天球生成
	skydome.reset(Skydome::Create(modelSkydome.get()));
	//見栄えがいい角度に変更しておく
	skydome->SetRotation({ 0, 50, 0 });

	//背景用隕石配置スクリプトの読み込み
	LoadSetData(meteoriteSetCommands, "Resources/csv/MeteoriteSetStage02.csv");
	//背景用隕石に必要な情報をセット
	Meteorite::SetMeteoriteModel(modelMeteoriteWhite.get());
	Meteorite::SetGameCamera(gameCamera.get());

	//宇宙塵エフェクトに必要な情報をセット
	SpaceDustEffect::SetSpaceDustModel(modelSpaceDust.get());
	SpaceDustEffect::SetPlayer(player.get());
	SpaceDustEffect::SetGameCamera(gameCamera.get());
	SpaceDustEffect::SetIsScrollMode(false);

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

	//ステージ開始UI生成
	stageStartUI.reset(StageStartUI::Create(1));

	//敵を倒した数カウンターリセット
	EnemyDefeatCounter::CounterReset();
}

void Stage02Scene::Update()
{
	//入力のインスタンスを取得
	Input* input = Input::GetInstance();
	//デバッグテキストのインスタンスを取得
	DebugText* debugText = DebugText::GetInstance();

	//オブジェクト解放
	ObjectRelease();
	//敵発生コマンド更新
	UpdateEnemySetCommands(gameCamera->GetPosition());
	//背景用隕石発生コマンド更新
	UpdateMeteoriteSetCommands(gameCamera->GetPosition());
	//宇宙塵エフェクト生成
	SpaceDustEffectCreateManager();
	//ボスバトル開始判定処理
	BossBattleStart();
	//ステージクリア
	StageClear();
	//ゲームオーバー
	GameOver();

	//カメラ更新
	gameCamera->Update();

	//ライト更新
	lightGroup->SetAmbientColor(XMFLOAT3(ambientColor0));
	lightGroup->SetDirLightDir(0, XMVECTOR({ lightDir0[0], lightDir0[1], lightDir0[2], 0 }));
	lightGroup->SetDirLightColor(0, XMFLOAT3(lightColor0));
	lightGroup->SetDirLightDir(1, XMVECTOR({ lightDir1[0], lightDir1[1], lightDir1[2], 0 }));
	lightGroup->SetDirLightColor(1, XMFLOAT3(lightColor1));
	lightGroup->SetDirLightDir(2, XMVECTOR({ lightDir2[0], lightDir2[1], lightDir2[2], 0 }));
	lightGroup->SetDirLightColor(2, XMFLOAT3(lightColor2));
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
	//ボス(ボス2)
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
	skydome->SetPosition({ 0, 0, gameCamera->GetPosition().z }); //天球をカメラに追従させる
	skydome->Update();
	//背景用隕石
	for (const std::unique_ptr<Meteorite>& meteorite : meteorites) {
		meteorite->Update();
	}
	//宇宙塵エフェクト
	for (const std::unique_ptr<SpaceDustEffect>& spaceDustEffect : spaceDustEffects) {
		spaceDustEffect->Update();
	}

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

void Stage02Scene::DrawBackSprite()
{
}

void Stage02Scene::Draw3D()
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
	//ボス(ボス2)
	if (boss) {
		boss->Draw();
	}
	//回復アイテム
	for (const std::unique_ptr<HealingItem>& healingItem : healingItems) {
		healingItem->Draw();
	}
	//天球
	skydome->Draw();
	//背景用隕石
	for (const std::unique_ptr<Meteorite>& meteorite : meteorites) {
		meteorite->Draw();
	}
	//宇宙塵エフェクト
	for (const std::unique_ptr<SpaceDustEffect>& spaceDustEffect : spaceDustEffects) {
		spaceDustEffect->Draw();
	}

	///-------Object3d描画ここまで-------///

	///-------パーティクル描画ここから-------///

	//パーティクル描画
	ParticleEmitter::GetInstance()->DrawAll();

	///-------パーティクル描画ここまで-------///
}

void Stage02Scene::Draw3DLightView()
{
	//Object3d共通コマンド
	ObjObject3d::DrawLightViewPrev();
	///-------Object3d描画ここから-------///



	///-------Object3d描画ここまで-------///
}

void Stage02Scene::Draw3DTopLightView()
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
	//回復アイテム
	for (const std::unique_ptr<HealingItem>& healingItem : healingItems) {
		healingItem->DrawTopLightCameraView();
	}
	//背景用隕石
	for (const std::unique_ptr<Meteorite>& meteorite : meteorites) {
		meteorite->DrawTopLightCameraView();
	}

	///-------Object3d描画ここまで-------///
}

void Stage02Scene::DrawFrontSprite()
{
	//スプライト共通コマンド
	SpriteCommon::GetInstance()->DrawPrev();
	///-------スプライト描画ここから-------///

	//自機のUI
	player->DrawUI();
	//ボス(ボス2)のUI
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

void Stage02Scene::SpaceDustEffectCreateManager()
{
	//ランダムでエフェクトの座標をセット
	const Vector3 randPos = { 90, 70, 0 };
	Vector3 position = {};
	position.x = (float)((rand() % (int)randPos.x) - randPos.x / 2);
	position.y = (float)((rand() % (int)randPos.y) - randPos.y / 2);
	//ゲームカメラから一定距離離れた場所に生成
	const float cameraDistance = 100;
	position.z = gameCamera->GetPosition().z + cameraDistance;

	//ランダムでエフェクトの回転の速さをセット
	const Vector3 randRot = { 360, 360, 360 };
	Vector3 rotation;
	rotation.x = (float)((rand() % (int)randRot.x));
	rotation.y = (float)((rand() % (int)randRot.y));
	rotation.z = (float)((rand() % (int)randRot.z));

	//宇宙塵エフェクト生成
	std::unique_ptr<SpaceDustEffect> newSpaceDustEffect;
	newSpaceDustEffect.reset(SpaceDustEffect::Create(position, rotation));
	spaceDustEffects.push_back(std::move(newSpaceDustEffect));
}

void Stage02Scene::ObjectRelease()
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

	//削除状態の背景用隕石の削除
	meteorites.remove_if([](std::unique_ptr<Meteorite>& meteorite) {
		return meteorite->GetIsDelete();
		});

	//削除状態の宇宙塵エフェクトの削除
	spaceDustEffects.remove_if([](std::unique_ptr<SpaceDustEffect>& spaceDustEffect) {
		return spaceDustEffect->GetIsDelete();
		});

	//削除状態のボス(ボス2)の削除
	if (boss) {
		if (boss->GetIsDelete()) {
			//カメラのボス情報を解除させる
			gameCamera->BossDelete();
			//死亡後演出を生成
			bossDeadEffect.reset(BossDeadEffect::Create(boss->GetBody()->GetWorldPos(), false));
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

	//爆発終了状態のボス死亡後演出の削除
	if (bossDeadEffect) {
		if (bossDeadEffect->GetIsExplosionEnd()) {
			//自機を前進させる
			player->SetStageClearModePhase(Stage02Player::StageClearModePhase::Advance);
			////カメラのステージクリア後行動を自機の正面に移動に変更
			gameCamera->StageClearPlayerFrontStart();

			//解放
			bossDeadEffect.reset();
		}
	}
}

void Stage02Scene::CollisionCheck3d()
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

#pragma region 背景用隕石と自機弾の衝突判定
	//全ての背景用隕石と全ての自機弾の衝突判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//自機弾座標
		posA = bullet->GetWorldPos();
		//自機弾半径
		radiusA = bullet->GetScale().x;

		for (const std::unique_ptr<Meteorite>& meteorite : meteorites) {
			//隕石座標
			posB = meteorite->GetPosition();
			//隕石半径
			radiusB = meteorite->GetScale().x;

			//球と球の衝突判定を行う
			bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);
			//衝突していなければ飛ばす
			if (!isCollision) { continue; }

			//自機弾のコールバック関数を呼び出す
			bullet->OnCollision(posB, radiusB, false);
		}
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

#pragma region 自機と背景用隕石の衝突判定
	//自機が緊急回避をしていない && ダメージノックバック状態でなければ判定する
	if (!player->GetIsRoll() && !player->GetIsDamageKnockback()) {
		//自機座標
		posA = player->GetWorldPos();
		//自機半径
		radiusA = player->GetScale().x;

		//自機と全ての背景用隕石の衝突判定
		for (const std::unique_ptr<Meteorite>& meteorite : meteorites) {
			//隕石座標
			posB = meteorite->GetPosition();
			//隕石半径
			radiusB = meteorite->GetScale().x;

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

	//ボスの存在がなければこの先の処理は行わない
	if (!boss) { return; }

#pragma region 自機弾とボス(ボス2)胴体の衝突判定
	//本体座標
	posA = boss->GetBody()->GetWorldPos();
	//本体半径
	radiusA = boss->GetBody()->GetScale().x;

	//全て自機弾とボス(ボス2)本体の衝突判定
	for (const std::unique_ptr<PlayerBullet>& bullet : playerBullets) {
		//自機弾座標
		posB = bullet->GetWorldPos();
		//自機弾弾半径
		radiusB = bullet->GetScale().x;

		//球と球の衝突判定を行う
		bool isCollision = Collision::CheckSphereToSphere(posA, posB, radiusA, radiusB);
		//衝突していなければ飛ばす
		if (!isCollision) { continue; }

		//ボス(ボス2)のコールバック関数を呼び出す
		boss->OnCollisionBody(bullet->GetDamageNum(), posB, bullet->GetVelocity());
		//自機弾のコールバック関数を呼び出す
		bullet->OnCollision(posA, radiusA);

		break;
	}
#pragma endregion
}

void Stage02Scene::CollisionCheck2d()
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

void Stage02Scene::InitializeEnemy()
{
	//敵配置スクリプトの読み込み
	LoadSetData(enemySetCommands, "Resources/csv/EnemySetStage02.csv");

	//全敵に必要な情報をセット
	Enemy::SetStageScene(this); //全敵にステージシーンを教える
	Enemy::SetPlayer(player.get()); //自機をセット
	Enemy::SetGameCamera(gameCamera.get()); //ゲームカメラをセット
	Enemy::SetBulletModel(modelEnemyBullet.get()); //弾のモデルをセット
	Enemy::SetIsGroundMode(false); //地面あり行動をOFFにする
	EnemyBreakEffect::SetIsGroundMode(false); //破壊エフェクトの地面あり行動をOFFにする
	EnemyBreakEffect::SetIsGravityMode(false); //破壊エフェクトの重力あり行動をOFFにする
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

	//破壊可能隕石
	MeteoriteEnemy::SetModel(modelMeteoriteBrown.get()); //モデルをセット
}

void Stage02Scene::UpdateMeteoriteSetCommands(const Vector3& targetPosition)
{
	//待機処理
	if (isWaitMeteorite) {
		//カメラのZ座標が生成座標以上なら
		if (waitMeteoriteSetPlayerPosition <= targetPosition.z) {
			//待機終了
			isWaitMeteorite = false;
		}
		return;
	}

	//1行分の文字列を入れる変数
	std::string line;

	//コマンドを実行するループ
	while (getline(meteoriteSetCommands, line)) {
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
			float posX = (float)std::atof(word.c_str());
			//y座標
			getline(line_stream, word, ',');
			float posY = (float)std::atof(word.c_str());
			//z座標
			getline(line_stream, word, ',');
			float posZ = (float)std::atof(word.c_str());

			//回転角x
			getline(line_stream, word, ',');
			float rotX = (float)std::atof(word.c_str());
			//回転角y
			getline(line_stream, word, ',');
			float rotY = (float)std::atof(word.c_str());
			//回転角z
			getline(line_stream, word, ',');
			float rotZ = (float)std::atof(word.c_str());

			//サイズ
			getline(line_stream, word, ',');
			float size = (float)std::atof(word.c_str());

			//速度x
			getline(line_stream, word, ',');
			float velX = (float)std::atof(word.c_str());
			//速度y
			getline(line_stream, word, ',');
			float velY = (float)std::atof(word.c_str());
			//速度z
			getline(line_stream, word, ',');
			float velZ = (float)std::atof(word.c_str());

			//回転速度x
			getline(line_stream, word, ',');
			float rotSpeedX = (float)std::atof(word.c_str());
			//回転速度y
			getline(line_stream, word, ',');
			float rotSpeedY = (float)std::atof(word.c_str());
			//回転速度z
			getline(line_stream, word, ',');
			float rotSpeedZ = (float)std::atof(word.c_str());

			//背景用隕石を発生
			std::unique_ptr<Meteorite> newMeteorite;
			newMeteorite.reset(Meteorite::Create({ posX, posY, posZ }, { rotX, rotY, rotZ }, size, { velX, velY, velZ }, { rotSpeedX, rotSpeedY, rotSpeedZ }));
			meteorites.push_back(std::move(newMeteorite));
		}

		//WAITコマンド
		else if (word.find("WAIT") == 0) {
			getline(line_stream, word, ',');

			//生成対象座標更新
			float waitPosition = (float)atoi(word.c_str());

			//待機開始
			isWaitMeteorite = true;
			waitMeteoriteSetPlayerPosition = waitPosition;

			//コマンドループを抜ける
			break;
		}
	}
}

void Stage02Scene::BossBattleStart()
{
	//既にボス(ボス2)バトルなら抜ける
	if (isBossBattle) { return; }
	//自機が墜落状態なら抜ける
	if (player->GetIsCrash()) { return; }

	//ボスバトル開始座標
	const float bossBattleStartPos = 3200;

	//警告開始判定
	if (!bossWarning) {
		//カメラがボスバトル開始とする座標まで進んだら開始
		const bool isBossBattleStart = gameCamera->GetPosition().z >= bossBattleStartPos;
		if (!isBossBattleStart) { return; }

		//カメラの前進を止める
		gameCamera->SetIsAdvance(false);

		//宇宙塵エフェクトをスクロール状態にする
		SpaceDustEffect::SetIsScrollMode(true);

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

		//ボス(ボス2)生成
		const float distance = 100;
		const Vector3 bossBasePos = { 0, 3, bossBattleStartPos + distance };
		boss.reset(Boss2::Create(bossBasePos));

		//ボスバトル状態にする
		isBossBattle = true;
	}
}

void Stage02Scene::StageClear()
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
		gameCamera->StageClearModeStart(boss->GetBody());
		//倒した数カウンターを増やす(大きいボスなので10)
		const int defeatNum = 10;
		EnemyDefeatCounter::AddCounter(defeatNum);
		//ハイスコア更新
		EnemyDefeatCounter::CheckHighScore(1);
		//宇宙塵エフェクトのスクロール状態を解除
		SpaceDustEffect::SetIsScrollMode(false);
	}
	//ステージクリア後
	else {
		//ステージリザルト
		StageResult();
	}
}

void Stage02Scene::StageResult()
{
	//ステージクリアテキスト生成と解放
	StageClearTextCreate();

	//ステージリザルトUI生成と解放
	StageResultUICreateAndRelease();

	//ステージクリア後自機のブースト行動開始判定
	StageClearPlayerBoostStart();

	//タイトルシーンに戻る
	ReturnTitleScene();
}

void Stage02Scene::StageClearTextCreate()
{
	//既にステージクリアテキストのインスタンスがあったら抜ける
	if (stageClearText) { return; }
	//カメラのステージクリア後行動が自機の周りを回るでなければ抜ける
	if (!(gameCamera->GetStageClearModePhase() == Stage02GameCamera::StageClearModePhase::PlayerAround)) { return; }

	//ステージクリアテキスト生成
	stageClearText.reset(StageClearText::Create());
}

void Stage02Scene::StageResultUICreateAndRelease()
{
	//ステージリザルトUIのインスタンスがないとき
	if (!stageResultUI) {
		//カメラの自機の周りを回る回転角が指定した値以上なら抜ける
		const float angleLimit = -120;
		if (gameCamera->GetPlayerAroundRotAngle() >= angleLimit) { return; }
		//カメラのステージクリア後行動が自機の周りを回るでなければ抜ける
		if (!(gameCamera->GetStageClearModePhase() == Stage02GameCamera::StageClearModePhase::PlayerAround)) { return; }

		//ステージリザルトUI生成
		stageResultUI.reset(StageResultUI::Create(EnemyDefeatCounter::GetDefeatCount(), false));
	}
	//ステージリザルトUIのインスタンスがあるとき
	else {
		//リザルト表示が完了していなければ抜ける
		if (!stageResultUI->GetIsResultEnd()) { return; }

		//リザルト終了のためボタンスプライトを表示する
		if (!stageResultUI->GetIsDrawButtonSprite()) {
			stageResultUI->SetIsDrawButtonSprite(true);
		}

		//指定の入力をしなければ抜ける
		if (!(Input::GetInstance()->TriggerKey(DIK_SPACE) || Input::GetInstance()->TriggerGamePadButton(Input::PAD_B))) { return; }

		//カメラのステージクリア後行動を自機のブーストを見る位置に移動に変更する
		gameCamera->StageClearPlayerBoostLookPosStart();

		//ステージリザルトUIの解放
		stageResultUI.reset();
	}
}

void Stage02Scene::StageClearPlayerBoostStart()
{
	//自機のステージクリア後行動が前進でなければ抜ける
	if (!(player->GetStageClearModePhase() == Stage02Player::StageClearModePhase::Advance)) { return; }
	//カメラのステージクリア後行動が自機のブーストを見る位置に移動でなければ抜ける
	if (!(gameCamera->GetStageClearModePhase() == Stage02GameCamera::StageClearModePhase::PlayerBoostPos)) { return; }
	//カメラのステージクリア後行動が完了していなければ抜ける
	if (!gameCamera->GetIsStageClearModeCompletion()) { return; }

	//自機のステージクリア後ブーストを開始する
	player->SetStageClearModePhase(Stage02Player::StageClearModePhase::Boost);

	//ポストエフェクトのブラーをかける
	GamePostEffect::GetPostEffect()->SetRadialBlur(true);
	const float blurStrength = 0.8f;
	GamePostEffect::GetPostEffect()->SetRadialBlurStrength(blurStrength);
}

void Stage02Scene::ReturnTitleScene()
{
	//自機のステージクリア後行動が完了していなければ抜ける
	if (!player->GetIsStageClearModeCompletion()) { return; }

	//ステージ選択へシーン変更を開始する
	SceneChangeStart({ 0,0,0,0 }, 120, 60, 60, "STAGESELECT");
}

void Stage02Scene::GameOver()
{
	//ゲームオーバーでないとき
	if (!isGameOver) {
		//自機が墜落状態でなければ抜ける
		if (!player->GetIsCrash()) { return; }

		//ゲームオーバー
		isGameOver = true;
		//カメラを墜落状態にする
		gameCamera->CrashStart();
		//宇宙塵エフェクトのスクロール状態を解除
		SpaceDustEffect::SetIsScrollMode(false);
	}
	//ゲームオーバーのとき
	else {
		//自機が死亡していなければ抜ける
		if (!player->GetIsDead()) { return; }

		//タイマー更新
		const float gameOverTime = 150;
		gameOverTimer++;

		//タイマーが指定した時間になったらステージ02をやり直す
		if (gameOverTimer >= gameOverTime) {
			SceneChangeStart({ 0,0,0,0 }, 60, 60, 20, "STAGE02");
		}
	}
}
