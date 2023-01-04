#pragma once
#include "BasePlayer.h"

/// <summary>
/// ステージ02自機
/// </summary>
class Stage02Player : public BasePlayer
{
public:
	//ステージクリア後行動フェーズ
	enum class StageClearModePhase {
		Stay,		//停止
		Advance,	//前進
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
	/// <returns>ステージ02自機</returns>
	static Stage02Player* Create(ObjModel* model, const int startHP, const int maxHP, const bool isVaporCreate);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(ObjModel* model, const int startHP, const int maxHP, const bool isVaporCreate) override;

	/// <summary>
	/// ステージクリア後の動きを開始する
	/// </summary>
	void StageClearModeStart();

	//getter
	StageClearModePhase GetStageClearModePhase() { return stageClearModePhase; }

	//setter
	void SetStageClearModePhase(StageClearModePhase stageClearModePhase) { this->stageClearModePhase = stageClearModePhase; }

private: //メンバ関数
	/// <summary>
	/// 墜落開始
	/// </summary>
	void CrashStart() override;

	/// <summary>
	/// 墜落
	/// </summary>
	void Crash() override;

	/// <summary>
	/// ステージクリア行動
	/// </summary>
	void StageClear() override;

	/// <summary>
	/// ステージクリア後の停止
	/// </summary>
	void StageClearStay();

	/// <summary>
	/// 回転角を0に修正
	/// </summary>
	void StageClearRotateFix();

	/// <summary>
	/// ステージクリア後の前進
	/// </summary>
	void StageClearAdvance();

	/// <summary>
	/// ステージクリア後のブースト
	/// </summary>
	void StageClearBoost();

private: //静的メンバ変数
	//ステージクリア後行動遷移
	static void (Stage02Player::* stageClearActionFuncTable[])();

private: //メンバ変数
	//墜落してからの時間タイマー
	int32_t crashTimer = 0;
	//ステージクリア後行動
	StageClearModePhase stageClearModePhase = StageClearModePhase::Stay;
	//ステージクリア後に使用するタイマー
	int32_t stageClearModeTimer = 0;
	//ステージクリア移動速度
	Vector3 stageClearMoveVelocity;
	//ステージクリア時角度
	Vector3 stageClearRota;
};
