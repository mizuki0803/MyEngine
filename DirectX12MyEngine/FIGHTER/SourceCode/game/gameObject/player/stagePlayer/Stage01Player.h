#pragma once
#include "BasePlayer.h"

/// <summary>
/// ステージ01自機
/// </summary>
class Stage01Player : public BasePlayer
{
public:
	//ステージクリア後行動フェーズ
	enum class StageClearModePhase {
		SideMove,	//横旋回移動
		Return,		//旋回帰還
		Up,			//上昇
		Stay,		//停止
		Boost,		//ブースト
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="startHP">開始時HP</param>
	/// <param name="maxHP">最大HP</param>
	/// <param name="isVaporCreate">飛行機雲演出を生成するか</param>
	/// <returns>ステージ01自機</returns>
	static Stage01Player* Create(ObjModel* model, const int startHP, const int maxHP, const bool isVaporCreate);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <param name="startHP">開始時HP</param>
	/// <param name="maxHP">最大HP</param>
	/// <param name="isVaporCreate">飛行機雲演出を生成するか</param>
	/// <returns>成否</returns>
	bool Initialize(ObjModel* model, const int startHP, const int maxHP, const bool isVaporCreate) override;

	/// <summary>
	/// ステージクリア後の動きを開始する
	/// </summary>
	void StageClearModeStart();

	/// <summary>
	/// ステージクリア後の帰還を開始する
	/// </summary>
	/// <param name="cameraPos">カメラ座標</param>
	void StageClearReturnStart(const Vector3& cameraPos);

	/// <summary>
	/// ステージクリア後のブーストを開始する
	/// </summary>
	void StageClearBoostStart();

	//getter
	const int GetCrashBoundCount() { return crashBoundCount; }
	StageClearModePhase GetStageClearModePhase() { return stageClearModePhase; }

private: //メンバ関数
	/// <summary>
	/// 墜落
	/// </summary>
	void Crash() override;

	/// <summary>
	/// 回転
	/// </summary>
	void Rotate() override;

	/// <summary>
	/// ステージクリア行動
	/// </summary>
	void StageClear() override;

	/// <summary>
	/// ステージクリア後の横移動
	/// </summary>
	void StageClearSideMove();

	/// <summary>
	/// ステージクリア後の帰還
	/// </summary>
	void StageClearReturn();

	/// <summary>
	/// ステージクリア後の上昇
	/// </summary>
	void StageClearUp();

	/// <summary>
	/// ステージクリア後の停止
	/// </summary>
	void StageClearStay();

	/// <summary>
	/// ステージクリア後のブースト
	/// </summary>
	void StageClearBoost();

private: //静的メンバ変数
	//ステージクリア後行動遷移
	static void (Stage01Player::* stageClearActionFuncTable[])();

private: //メンバ変数
	//墜落バウンド回数
	int crashBoundCount = 0;
	//墜落速度
	Vector3 crashVel = { 0, 0, 0.25f };
	//ステージクリア後行動
	StageClearModePhase stageClearModePhase = StageClearModePhase::SideMove;
	//ステージクリア後に使用するタイマー
	int32_t stageClearModeTimer = 0;
	//ステージクリア移動方向が右か
	bool isStageClearMoveRight = true;
	//ステージクリア移動速度
	Vector3 stageClearMoveVelocity;
	//ステージクリア時角度
	Vector3 stageClearRota;
	//ステージクリア後に使用するカメラホーミング用座標
	Vector3 stageClearCameraPos;
	//カメラホーミング用座標
	Vector3 cameraHomingPos;
};
