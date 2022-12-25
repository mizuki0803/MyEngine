#pragma once
#include "BaseStageScene.h"
#include "Stage02GameCamera.h"
#include "Collision.h"
#include "Stage02Player.h"
#include "Boss.h"
#include "BossDeadEffect.h"
#include "Meteorite.h"

/// <summary>
/// ステージ02シーン
/// </summary>
class Stage02Scene :public BaseStageScene
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
	/// 背景用隕石発生コマンドの更新
	/// </summary>
	void UpdateMeteoriteSetCommands(const Vector3& targetPosition);

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
	std::unique_ptr<Stage02GameCamera> gameCamera;

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
	std::unique_ptr<ObjModel> modelBossMainBody;
	std::unique_ptr<ObjModel> modelBossMainBodyDamage;
	std::unique_ptr<ObjModel> modelBossMainBodySleep;
	std::unique_ptr<ObjModel> modelBossMainBodyDead;
	std::unique_ptr<ObjModel> modelBossAvatar;
	std::unique_ptr<ObjModel> modelBossAvatarDamage;
	std::unique_ptr<ObjModel> modelBossAvatarSleep;
	std::unique_ptr<ObjModel> modelBossAvatarDead;
	std::unique_ptr<ObjModel> modelHealingItem;

	//自機
	std::unique_ptr<Stage02Player> player;
	//ボス
	std::unique_ptr<Boss> boss;
	//ボス死亡後の演出管理
	std::unique_ptr<BossDeadEffect> bossDeadEffect;

	//背景用隕石(破壊不可)
	std::list<std::unique_ptr<Meteorite>> meteorites;
	//背景用隕石発生コマンド
	std::stringstream meteoriteSetCommands;
	//背景用隕石発生コマンド待機中か
	bool isWaitMeteorite = false;
	//待機用 生成を行う自機座標(自機が生成を行う座標以上になったら背景用隕石生成を行う)
	float waitMeteoriteSetPlayerPosition = 0;

	//ステージクリア用の影状態か
	bool isStageClearShadow = false;
	//ターゲットと光源カメラの距離
	Vector3 lightCameraTargetDistance = { -300, 200, -150 };
};