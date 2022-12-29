#pragma once
#include "ObjObject3d.h"
#include "PlayerBullet.h"

//ステージシーンの前方宣言
class BaseStageScene;

/// <summary>
/// ステージ02出撃シーン用自機
/// </summary>
class Stage02SortiePlayer : public ObjObject3d
{
public:
	//ステージ02出撃行動フェーズ
	enum class Stage02SortieActionPhase {
		Advance,	//前進
		Boost,		//ブースト
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>ステージ02出撃シーン用自機</returns>
	static Stage02SortiePlayer* Create(ObjModel* model, const Vector3& startPosition);

	//getter
	static const float GetAdvanceSpeed() { return Stage02SortiePlayer::advanceSpeed; }

	//setter
	static void SetStageScene(BaseStageScene* stageScene) { Stage02SortiePlayer::stageScene = stageScene; }
	static void SetBulletModel(ObjModel* model) { Stage02SortiePlayer::bulletModel = model; }

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// ブースト状態にする
	/// </summary>
	void BoostStart();

	//getter
	Vector3 GetWorldPos();
	Stage02SortieActionPhase GetSortieActionPhase() { return sortieActionPhase; }
	bool GetIsSortieEnd() { return isSortieEnd; }

private: //メンバ関数
	/// <summary>
	/// 弾発射座標を更新
	/// </summary>
	void UpdateBulletShotPos();

	/// <summary>
	/// 直進弾発射
	/// </summary>
	void ShotStraightBullet();

	/// <summary>
	/// 弾発射
	/// </summary>
	void Fire();

	/// <summary>
	/// 出撃前進
	/// </summary>
	void SortieAdvance();

	/// <summary>
	/// 出撃ブースト
	/// </summary>
	void SortieBoost();

private: //静的メンバ変数
	//出撃行動遷移
	static void (Stage02SortiePlayer::* sortieActionFuncTable[])();
	//ステージシーン
	static BaseStageScene* stageScene;
	//自機弾のモデル
	static ObjModel* bulletModel;
	//通常前進スピード
	static const float advanceSpeed;

private: //メンバ変数
	//出撃行動
	Stage02SortieActionPhase sortieActionPhase = Stage02SortieActionPhase::Advance;
	//弾発射座標
	Vector3 bulletShotPos;
	//弾発射タイマー
	int32_t fireTimer = 0;
	//弾発射回数
	int fireCount = 0;
	//出撃行動が終了したか
	bool isSortieEnd = false;
	//ブースト経過時間タイマー
	int32_t boostTimer = 0;
	//パーティクルジェットの大きさ変更用
	int32_t particleJetSizePhaseNum = 0;
};
