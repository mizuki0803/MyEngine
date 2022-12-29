#include "Stage02SortieScene.h"
#include "SceneManager.h"
#include "Input.h"
#include "Audio.h"
#include "SpriteCommon.h"
#include "DebugText.h"
#include "Easing.h"
#include "ParticleEmitter.h"
#include "SceneChangeEffect.h"
#include "GamePostEffect.h"
#include "MeteoriteEnemy.h"
#include <cassert>
#include <fstream>
#include <iomanip>

void Stage02SortieScene::Initialize()
{
	//ライト生成
	lightGroup.reset(LightGroup::Create());
	lightGroup->SetDirLightActive(0, true);
	lightGroup->SetDirLightActive(1, true);
	lightGroup->SetDirLightActive(2, false);

	//objからモデルデータを読み込む
	modelSkydome.reset(ObjModel::LoadFromOBJ("skydomeStage02"));
	modelFighter.reset(ObjModel::LoadFromOBJ("fighter"));
	modelPlayerBullet.reset(ObjModel::LoadFromOBJ("playerBullet", true));
	modelMeteoriteBrown.reset(ObjModel::LoadFromOBJ("meteoriteBrown", true));

	//ポストエフェクトのブラーを解除しておく
	GamePostEffect::GetPostEffect()->SetRadialBlur(false);

	//自機生成
	player.reset(Stage02SortiePlayer::Create(modelFighter.get(), { 0, 0, -360 }));
	//自機に必要な情報をセット
	Stage02SortiePlayer::SetStageScene(this);
	Stage02SortiePlayer::SetBulletModel(modelPlayerBullet.get()); //通常弾用モデルをセット
	PlayerBullet::SetIsGroundMode(false); //自機弾の地面あり行動をOFFにする


	//カメラ初期化
	sortieCamera.reset(new Stage02SortieCamera());
	sortieCamera->SetSortiePlayer(player.get());
	sortieCamera->Initialize();
	//影用光源カメラ初期化
	lightCamera.reset(new LightCamera());
	lightCamera->Initialize({ 0, 500, 0 });
	lightCamera->SetProjectionNum({ 500, 500 }, { -500, -500 });
	//頭上からの影用光源カメラ初期化
	topLightCamera.reset(new LightCamera());
	topLightCamera->Initialize({ 0, 500, 0 });
	topLightCamera->SetProjectionNum({ 100, 100 }, { -100, -100 });

	//全敵初期化処理
	InitializeEnemy();
	//敵発生コマンド更新
	UpdateEnemySetCommands(player->GetPosition());

	//天球生成
	skydome.reset(Skydome::Create(modelSkydome.get()));
	//見栄えがいい角度に変更しておく
	skydome->SetRotation({ 0, 215, 0 });

	//objオブジェクトにカメラをセット
	ObjObject3d::SetCamera(sortieCamera.get());
	ObjObject3d::SetLightCamera(lightCamera.get());
	ObjObject3d::SetTopLightCamera(topLightCamera.get());
	//objオブジェクトにライトをセット
	ObjObject3d::SetLightGroup(lightGroup.get());

	//パーティクルにカメラをセット
	ParticleManager::SetCamera(sortieCamera.get());
	//画面にパーティクルが残ることがあるので全て削除しておく
	ParticleEmitter::GetInstance()->AllDelete();
}

void Stage02SortieScene::Update()
{
	//デバッグテキストのインスタンスを取得
	DebugText* debugText = DebugText::GetInstance();

	//オブジェクト解放
	ObjectRelease();
	//出撃挙動管理
	SortieAction();

	//カメラ更新
	sortieCamera->Update();

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
	//敵破壊エフェクト
	for (const std::unique_ptr<EnemyBreakEffect>& breakEffect : enemyBreakEffects) {
		breakEffect->Update();
	}
	//天球
	skydome->Update();

	//3D衝突判定管理
	CollisionCheck3d();

	//パーティクル更新
	ParticleEmitter::GetInstance()->Update();


	//自機の出撃行動が完了したら
	if (player->GetIsSortieEnd()) {
		//ステージ02へシーン変更を開始する
		SceneChangeStart({ 1,1,1,0 }, 10, 0, 20, "STAGE02");
	}
	//ムービースキップ機能
	MovieSkip();

	//シーン変更状態
	SceneChangeMode();

	//シーン変更演出更新
	SceneChangeEffect::Update();
}

void Stage02SortieScene::DrawBackSprite()
{
}

void Stage02SortieScene::Draw3D()
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
	//敵破壊エフェクト
	for (const std::unique_ptr<EnemyBreakEffect>& breakEffect : enemyBreakEffects) {
		breakEffect->Draw();
	}
	//天球
	skydome->Draw();

	///-------Object3d描画ここまで-------///

	///-------パーティクル描画ここから-------///

	//パーティクル描画
	ParticleEmitter::GetInstance()->DrawAll();

	///-------パーティクル描画ここまで-------///
}

void Stage02SortieScene::Draw3DLightView()
{
}

void Stage02SortieScene::Draw3DTopLightView()
{
	//Object3d共通コマンド
	ObjObject3d::DrawLightViewPrev();
	///-------Object3d描画ここから-------///

	//自機
	player->DrawTopLightCameraView();

	///-------Object3d描画ここまで-------///
}

void Stage02SortieScene::DrawFrontSprite()
{
	//スプライト共通コマンド
	SpriteCommon::GetInstance()->DrawPrev();
	///-------スプライト描画ここから-------///

	//シーン変更演出描画
	SceneChangeEffect::Draw();


	///-------スプライト描画ここまで-------///
}

void Stage02SortieScene::ObjectRelease()
{
	//死亡した自機弾の削除
	playerBullets.remove_if([](std::unique_ptr<PlayerBullet>& bullet) {
		return bullet->GetIsDead();
		});

	//削除状態の敵の削除
	enemys.remove_if([](std::unique_ptr<Enemy>& enemy) {
		return enemy->GetIsDelete();
		});

	//削除状態の敵破壊エフェクトの削除
	enemyBreakEffects.remove_if([](std::unique_ptr<EnemyBreakEffect>& breakEffect) {
		return breakEffect->GetIsDelete();
		});
}

void Stage02SortieScene::CollisionCheck3d()
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
}

void Stage02SortieScene::InitializeEnemy()
{
	//敵配置スクリプトの読み込み
	LoadSetData(enemySetCommands, "Resources/csv/EnemySetSortie02.csv");

	//全敵に必要な情報をセット
	Enemy::SetStageScene(this); //全敵にステージシーンを教える
	Enemy::SetPlayer(nullptr); //自機をセット
	Enemy::SetGameCamera(nullptr); //ゲームカメラをセット
	Enemy::SetBulletModel(nullptr); //弾のモデルをセット
	Enemy::SetIsGroundMode(false); //地面あり行動をOFFにする
	EnemyBreakEffect::SetIsGroundMode(false); //破壊エフェクトの地面あり行動をOFFにする
	EnemyBreakEffect::SetIsGravityMode(false); //破壊エフェクトの重力あり行動をOFFにする
	EnemyBreakEffect::SetGameCamera(nullptr); //破壊エフェクトにゲームカメラをセット

	//破壊可能隕石
	MeteoriteEnemy::SetModel(modelMeteoriteBrown.get()); //モデルをセット
}

void Stage02SortieScene::SortieAction()
{
	//既にブースト状態なら抜ける
	if (player->GetSortieActionPhase() == Stage02SortiePlayer::Stage02SortieActionPhase::Boost) { return; }
	//カメラの自機ズームが終了していなければ抜ける
	if (!sortieCamera->GetIsZoomEnd()) { return; }

	//自機をブースト状態にする
	player->BoostStart();
}

void Stage02SortieScene::MovieSkip()
{
	//スキップ入力がなければ抜ける
	if (!(Input::GetInstance()->TriggerKey(DIK_RETURN) || Input::GetInstance()->TriggerGamePadButton(Input::PAD_START))) { return; }

	//ステージ02へシーン変更を開始する
	SceneChangeStart({ 0,0,0,0 }, 15, 0, 15, "STAGE02");
}
