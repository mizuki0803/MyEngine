#pragma once
#include "BaseGameScene.h"
#include "Sprite.h"
#include "ObjObject3d.h"
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
#include "HowToPlayUI.h"
#include "MultiHitUI.h"
#include "StageStartUI.h"
#include "StageClearText.h"
#include "StageResultUI.h"

#include <array>
#include <sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene :public BaseGameScene
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

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
	/// 敵初期化処理
	/// </summary>
	void InitializeEnemy();

	/// <summary>
	/// 敵発生データ読み込み
	/// </summary>
	void LoadEnemySetData(const std::string& fileName);

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemySetCommands();

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
	std::unique_ptr<GameCamera> gameCamera;
	//影用光源カメラ
	std::unique_ptr<LightCamera> lightCamera;
	//頭上からの影用光源カメラ
	std::unique_ptr<LightCamera> topLightCamera;

	//ライト
	std::unique_ptr<LightGroup> lightGroup;
	float ambientColor0[3] = { 1,1,1 };
	// 光線方向初期値
	float lightDir0[3] = { 2,-1,1 };
	float lightColor0[3] = { 1,1,1 };
	float lightDir1[3] = { -2,-1,-6 };
	float lightColor1[3] = { 0.3f,0.3f,0.3f };

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
	std::unique_ptr<Player> player;
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
	//ボス
	std::unique_ptr<Boss> boss;
	//ボス戦中か
	bool isBossBattle = false;
	//ボス登場警告
	std::unique_ptr<BossWarning> bossWarning;
	//ボス死亡後の演出管理
	std::unique_ptr<BossDeadEffect> bossDeadEffect;
	//回復アイテム
	std::list<std::unique_ptr<HealingItem>> healingItems;
	//天球
	std::unique_ptr<Skydome> skydome;
	//地面
	std::unique_ptr<GameGroundManager> gameGroundManager;
	//背景用(ビル管理)
	std::unique_ptr<GameBuildingManager> gameBuildingManager;
	//遊び方UI
	std::unique_ptr<HowToPlayUI> howToPlayUI;
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