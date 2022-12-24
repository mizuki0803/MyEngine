#pragma once
#include "BaseGameScene.h"
#include "PlayerBullet.h"
#include "LightGroup.h"
#include "LightCamera.h"
#include "Enemy.h"
#include "BossWarning.h"
#include "HealingItem.h"
#include "Skydome.h"
#include "MultiHitUI.h"
#include "StageStartUI.h"
#include "StageClearText.h"
#include "StageResultUI.h"

#include <array>
#include <sstream>

/// <summary>
/// 全ステージの基底シーン
/// </summary>
class BaseStageScene :public BaseGameScene
{
public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	virtual void Initialize() = 0;

	/// <summary>
	/// 更新
	/// </summary>
	virtual void Update() = 0;

	/// <summary>
	/// 背景スプライト描画
	/// </summary>
	virtual void DrawBackSprite() = 0;

	/// <summary>
	/// 3Dシーン描画
	/// </summary>
	virtual void Draw3D() = 0;

	/// <summary>
	/// 3Dシーン描画(ライトから見た視点)
	/// </summary>
	virtual void Draw3DLightView() = 0;

	/// <summary>
	/// 3Dシーン描画(頭上からのライトから見た視点)
	/// </summary>
	virtual void Draw3DTopLightView() = 0;

	/// <summary>
	/// 前景スプライト描画
	/// </summary>
	virtual void DrawFrontSprite() = 0;

	/// <summary>
	/// 自機弾を追加する
	/// </summary>
	/// <param name="playerBullet">自機弾</param>
	void AddPlayerBullet(std::unique_ptr<PlayerBullet> playerBullet);

	/// <summary>
	/// 敵弾を追加する
	/// </summary>
	/// <param name="enemyBullet">敵弾</param>
	void AddEnemyBullet(std::unique_ptr<EnemyBullet> enemyBullet);

	/// <summary>
	/// 敵破壊エフェクトを追加する
	/// </summary>
	/// <param name="enemyBreakEffect">敵破壊エフェクト</param>
	void AddEnemyBreakEffect(std::unique_ptr<EnemyBreakEffect> enemyBreakEffect);

	/// <summary>
	/// 敵発生データ読み込み
	/// </summary>
	void LoadEnemySetData(const std::string& fileName);

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemySetCommands(const Vector3& targetPosition);

protected: //メンバ変数
	//影用光源カメラ
	std::unique_ptr<LightCamera> lightCamera;
	//頭上からの影用光源カメラ
	std::unique_ptr<LightCamera> topLightCamera;

	//ライト
	std::unique_ptr<LightGroup> lightGroup;
	float ambientColor0[3] = { 1,1,1 };
	// 光線方向初期値
	float lightDir0[3] = { 2,-1,1 };
	float lightColor0[3] = { 0.9f,0.9f,0.9f };
	float lightDir1[3] = { -2,-1,-6 };
	float lightColor1[3] = { 0.3f,0.3f,0.3f };

	//自機弾
	std::list<std::unique_ptr<PlayerBullet>> playerBullets;
	//敵
	std::list<std::unique_ptr<Enemy>> enemys;
	//敵弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets;
	//敵破壊エフェクト
	std::list<std::unique_ptr<EnemyBreakEffect>> enemyBreakEffects;
	//敵発生コマンド
	std::stringstream enemySetCommands;
	//待機中か
	bool isWait = false;
	//待機用 生成を行う自機座標(自機が生成を行う座標以上になったら敵生成を行う)
	float waitEnemySetPlayerPosition = 0;
	//ボス戦中か
	bool isBossBattle = false;
	//ボス登場警告
	std::unique_ptr<BossWarning> bossWarning;
	//回復アイテム
	std::list<std::unique_ptr<HealingItem>> healingItems;
	//天球
	std::unique_ptr<Skydome> skydome;
	//一撃で複数体処理用のUI
	std::list<std::unique_ptr<MultiHitUI>> multiHitUIs;
	//ステージ開始UI
	std::unique_ptr<StageStartUI> stageStartUI;
	//ステージクリアテキスト
	std::unique_ptr<StageClearText> stageClearText;
	//ステージリザルトUI
	std::unique_ptr<StageResultUI> stageResultUI;
	//ステージクリアか
	bool isStageClear = false;
	//ゲームオーバーか
	bool isGameOver = false;
	//ゲームオーバータイマー
	int32_t gameOverTimer = 0;
};