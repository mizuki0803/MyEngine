#pragma once
#include "BaseStageScene.h"
#include "Stage02GameCamera.h"
#include "Collision.h"
#include "Stage02Player.h"
#include "Galaxy.h"
#include "BossDeadEffect.h"
#include "Meteorite.h"
#include "SpaceDustEffect.h"

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
	/// 宇宙塵エフェクト生成管理
	/// </summary>
	void SpaceDustEffectCreateManager();

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
	/// <param name="targetPosition">生成対象の座標</param>
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
	/// ステージクリアテキスト生成
	/// </summary>
	void StageClearTextCreate();

	/// <summary>
	/// ステージリザルトUI生成と解放
	/// </summary>
	void StageResultUICreateAndRelease();

	/// <summary>
	/// ステージクリア後自機のブースト行動開始判定
	/// </summary>
	void StageClearPlayerBoostStart();

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
	std::unique_ptr<ObjModel> modelSphere;
	std::unique_ptr<ObjModel> modelMeteoriteWhite;
	std::unique_ptr<ObjModel> modelMeteoriteBrown;
	std::unique_ptr<ObjModel> modelSpaceDust;
	std::unique_ptr<ObjModel> modelPlayerBullet;
	std::unique_ptr<ObjModel> modelFighter;
	std::unique_ptr<ObjModel> modelEnemyBullet;
	std::unique_ptr<ObjModel> modelEnemyFighter;
	std::array<std::unique_ptr<ObjModel>, 5> modelEnemyFighterBreak;
	std::unique_ptr<ObjModel> modelEnemyMiniRobot;
	std::array<std::unique_ptr<ObjModel>, 5> modelEnemyMiniRobotBreak;
	std::unique_ptr<ObjModel> modelGalaxyBody;
	std::unique_ptr<ObjModel> modelGalaxyPropeller;
	std::unique_ptr<ObjModel> modelGalaxyCannon;
	std::unique_ptr<ObjModel> modelGalaxyCannonDead;
	std::array<std::unique_ptr<ObjModel>, 5> modelGalaxyCannonBreak;
	std::unique_ptr<ObjModel> modelMedamanAvatar;
	std::unique_ptr<ObjModel> modelMedamanAvatarSleep;
	std::unique_ptr<ObjModel> modelMedamanAvatarDead;
	std::unique_ptr<ObjModel> modelHealingItem;

	//自機
	std::unique_ptr<Stage02Player> player;
	//ボス
	std::unique_ptr<Galaxy> boss;
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

	//宇宙塵エフェクト
	std::list<std::unique_ptr<SpaceDustEffect>> spaceDustEffects;
};