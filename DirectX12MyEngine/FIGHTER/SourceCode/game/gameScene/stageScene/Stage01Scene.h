#pragma once
#include "BaseStageScene.h"
#include "Stage01GameCamera.h"
#include "Collision.h"
#include "Stage01Player.h"
#include "Medaman.h"
#include "BossDeadEffect.h"
#include "GameGroundManager.h"
#include "GameBuildingManager.h"
#include "HowToPlayUI.h"

/// <summary>
/// ステージ01シーン
/// </summary>
class Stage01Scene :public BaseStageScene
{
public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 背景スプライト描画
	/// </summary>
	void DrawBackSprite() override;

	/// <summary>
	/// 3Dシーン描画
	/// </summary>
	void Draw3D() override;

	/// <summary>
	/// 3Dシーン描画(ライトから見た視点)
	/// </summary>
	void Draw3DLightView() override;

	/// <summary>
	/// 3Dシーン描画(頭上からのライトから見た視点)
	/// </summary>
	void Draw3DTopLightView() override;

	/// <summary>
	/// 前景スプライト描画
	/// </summary>
	void DrawFrontSprite() override;

	/// <summary>
	/// 影生成用ライトカメラ更新
	/// </summary>
	void LightCameraUpdate();

	/// <summary>
	/// オブジェクトの解放
	/// </summary>
	void ObjectRelease();

	/// <summary>
	/// 3Dオブジェクトの衝突判定
	/// </summary>
	void CollisionCheck3d();

	/// <summary>
	/// 2Dオブジェクトの衝突判定
	/// </summary>
	void CollisionCheck2d();

	/// <summary>
	/// 敵初期化処理
	/// </summary>
	void InitializeEnemy();

	/// <summary>
	/// 遊び方
	/// </summary>
	void HowToPlay();

	/// <summary>
	/// ボスバトル開始判定処理
	/// </summary>
	void BossBattleStart();

	/// <summary>
	/// ステージクリア
	/// </summary>
	void StageClear();

	/// <summary>
	/// ステージリザルト
	/// </summary>
	void StageResult();

	/// <summary>
	/// ステージクリア時の影の向きに変更
	/// </summary>
	void StageClearSetLightCameraPos();

	/// <summary>
	/// ステージクリアテキスト生成と解放
	/// </summary>
	void StageClearTextCreateAndRelease();

	/// <summary>
	/// ステージリザルトUI生成と解放
	/// </summary>
	void StageResultUICreateAndRelease();

	/// <summary>
	/// タイトルシーンに戻る処理
	/// </summary>
	void ReturnTitleScene();

	/// <summary>
	/// ゲームオーバー
	/// </summary>
	void GameOver();

private: //メンバ変数
	//ゲームカメラ
	std::unique_ptr<Stage01GameCamera> gameCamera;

	//objモデルデータ
	std::unique_ptr<ObjModel> modelSkydome;
	std::unique_ptr<ObjModel> modelGround;
	std::array<std::unique_ptr<ObjModel>, 2> modelBuilding;
	std::unique_ptr<ObjModel> modelSphere;
	std::unique_ptr<ObjModel> modelPlayerBullet;
	std::unique_ptr<ObjModel> modelFighter;
	std::unique_ptr<ObjModel> modelEnemyBullet;
	std::unique_ptr<ObjModel> modelEnemyFighter;
	std::array<std::unique_ptr<ObjModel>, 5> modelEnemyFighterBreak;
	std::unique_ptr<ObjModel> modelEnemyMiniRobot;
	std::array<std::unique_ptr<ObjModel>, 5> modelEnemyMiniRobotBreak;
	std::unique_ptr<ObjModel> modelMedamanMainBody;
	std::unique_ptr<ObjModel> modelMedamanMainBodyDamage;
	std::unique_ptr<ObjModel> modelMedamanMainBodySleep;
	std::unique_ptr<ObjModel> modelMedamanMainBodyDead;
	std::unique_ptr<ObjModel> modelMedamanAvatar;
	std::unique_ptr<ObjModel> modelMedamanAvatarDamage;
	std::unique_ptr<ObjModel> modelMedamanAvatarSleep;
	std::unique_ptr<ObjModel> modelMedamanAvatarDead;
	std::unique_ptr<ObjModel> modelHealingItem;

	//自機
	std::unique_ptr<Stage01Player> player;

	//ボス(メダマーン)
	std::unique_ptr<Medaman> boss;
	//ボス死亡後の演出管理
	std::unique_ptr<BossDeadEffect> bossDeadEffect;

	//地面
	std::unique_ptr<GameGroundManager> gameGroundManager;
	//背景用(ビル管理)
	std::unique_ptr<GameBuildingManager> gameBuildingManager;
	//遊び方UI
	std::unique_ptr<HowToPlayUI> howToPlayUI;
	//ステージクリア用の影状態か
	bool isStageClearShadow = false;
	//ターゲットと光源カメラの距離
	Vector3 lightCameraTargetDistance = { -300, 200, -150 };
};