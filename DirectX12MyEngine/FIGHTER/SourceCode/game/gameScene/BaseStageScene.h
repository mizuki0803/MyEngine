#pragma once
#include "BaseGameScene.h"
#include "LightGroup.h"
#include "GameCamera.h"
#include "LightCamera.h"
#include "CollisionShape.h"
#include "Collision.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "BossWarning.h"
#include "BossDeadEffect.h"
#include "HealingItem.h"
#include "Skydome.h"
#include "GameGroundManager.h"
#include "GameBuildingManager.h"
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

protected: //メンバ変数
	//自機弾
	std::list<std::unique_ptr<PlayerBullet>> playerBullets;
	//敵
	std::list<std::unique_ptr<Enemy>> enemys;
	//敵弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets;
	//敵破壊エフェクト
	std::list<std::unique_ptr<EnemyBreakEffect>> enemyBreakEffects;
};