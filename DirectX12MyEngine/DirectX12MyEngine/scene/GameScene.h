#pragma once
#include "BaseScene.h"
#include "Sprite.h"
#include "ObjObject3d.h"
#include "ParticleManager.h"
#include "LightGroup.h"
#include "Camera.h"
#include "RailCamera.h"
#include "CollisionShape.h"
#include "Collision.h"
#include "Player.h"
#include "Enemy.h"
#include "Boss.h"
#include "Skydome.h"
#include "HealingItem.h"

#include <sstream>

/// <summary>
/// ゲームシーン
/// </summary>
class GameScene :public BaseScene
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
	/// 描画
	/// </summary>
	void Draw() override;

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
	/// 敵発生データ読み込み
	/// </summary>
	void LoadEnemySetData();

	/// <summary>
	/// 敵発生コマンドの更新
	/// </summary>
	void UpdateEnemySetCommands();

	/// <summary>
	/// ボスバトル開始判定処理
	/// </summary>
	void BossBattleStart();

private: //メンバ変数
	//カメラ
	std::unique_ptr<Camera> normalCamera;
	//レールカメラ
	std::unique_ptr<RailCamera> railCamera;

	//ライト
	std::unique_ptr<LightGroup> lightGroup;
	float ambientColor0[3] = { 1,1,1 };
	// 光線方向初期値
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	float pointLightPos[3] = { 0,0,0 };
	float pointLightColor[3] = { 1,1,1 };
	float pointLightAtten[3] = { 0.3f, 0.1f, 0.1f };

	float spotLightDir[3] = { 0, -1, 0 };
	float spotLightPos[3] = { 0, 5, 0 };
	float spotLightColor[3] = { 1, 1, 1 };
	float spotLightAtten[3] = { 0.0f, 0.0f, 0.0f };
	float spotLightfactorAngleCos[2] = { 20.0f, 30.0f };

	float circleShadowDir[3] = { 0, -1, 0 };
	float circleShadowAtten[3] = { 0.5f, 0.6f, 0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.5f };

	//objモデルデータ
	std::unique_ptr<ObjModel> modelSkydome;
	std::unique_ptr<ObjModel> modelSphere;
	std::unique_ptr<ObjModel> modelFighter;
	std::unique_ptr<ObjModel> modelBossMainBody;
	std::unique_ptr<ObjModel> modelBossMainBodySleep;
	std::unique_ptr<ObjModel> modelBossAvatar;
	std::unique_ptr<ObjModel> modelBossAvatarSleep;

	//自機
	std::unique_ptr<Player> player;
	//自機弾
	std::list<std::unique_ptr<PlayerBullet>> playerBullets;
	//敵
	std::list<std::unique_ptr<Enemy>> enemys;
	//敵弾
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets;
	//敵発生コマンド
	std::stringstream enemySetCommands;
	//待機中か
	bool isWait = false;
	//待機タイマー
	int32_t waitTimer = 0;
	//ボス
	std::unique_ptr<Boss> boss;
	//ボス戦中か
	bool isBossBattle = false;
	//天球
	std::unique_ptr<Skydome> objSkydome;
	//回復アイテム
	std::list<std::unique_ptr<HealingItem>> healingItems;
	//ステージクリアか
	bool isStageClear = false;


	//その他変数
	float soundVol = 0.1f;
	char str[100];
};