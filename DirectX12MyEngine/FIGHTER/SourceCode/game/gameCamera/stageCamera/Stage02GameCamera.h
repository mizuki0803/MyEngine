#pragma once
#include "BaseGameCamera.h"

//ステージ02自機クラスを前方宣言
class Stage02Player;
//ボス2胴体を前方宣言
class Boss2Body;

/// <summary>
/// ステージ02ゲームカメラ
/// </summary>
class Stage02GameCamera : public BaseGameCamera
{
public:
	//ステージクリア後行動フェーズ
	enum class StageClearModePhase {
		BossLook,		//ボスの方向を向く
		FrontLook,		//正面の方向を向く
		PlayerLook,		//自機の方向を向く
		PlayerZoom,		//自機をズーム
		PlayerFollow,	//自機を追従(実際はなにもしない)
		PlayerSideMove,	//自機横に移動
		PlayerKeepLook,	//自機の方向をずっと向く
	};

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="player">自機</param>
	void Initialize(Stage02Player* player);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 墜落開始
	/// </summary>
	void CrashStart();

	/// <summary>
	/// ステージクリア後の動きを開始する
	/// </summary>
	/// <param name="bossMainBody">ボス胴体</param>
	void StageClearModeStart(Boss2Body* boss2Body);

	/// <summary>
	/// ボスが削除されたら
	/// </summary>
	void BossDelete();

	//setter
	StageClearModePhase GetStageClearModePhase() { return stageClearModePhase; }

private: //メンバ関数
	/// <summary>
	/// 墜落
	/// </summary>
	void Crash() override;

	/// <summary>
	/// ステージクリア後行動
	/// </summary>
	void StageClear() override;

	/// <summary>
	/// ステージクリア後のボスの方向を向く行動
	/// </summary>
	void StageClearBossLook();

	/// <summary>
	/// ステージクリア後の正面の方向を向く行動
	/// </summary>
	void StageClearFrontLook();

	/// <summary>
	/// ステージクリア後の自機の方向を向く行動
	/// </summary>
	void StageClearPlayerLook();

	/// <summary>
	/// ステージクリア後の自機をズームする行動
	/// </summary>
	void StageClearPlayerZoom();

	/// <summary>
	/// ステージクリア後の自機追従する行動
	/// </summary>
	void StageClearPlayerFollow();

	/// <summary>
	/// ステージクリア後の自機横に移動する行動
	/// </summary>
	void StageClearPlayerSideMove();

	/// <summary>
	/// ステージクリア後の自機の方向をずっと向く行動
	/// </summary>
	void StageClearPlayerKeepLook();


private: //静的メンバ変数
	//ステージクリア後行動遷移
	static void (Stage02GameCamera::* stageClearCameraActionFuncTable[])();

private: //メンバ変数
	//ステージ01自機
	Stage02Player* player = nullptr;
	//ボス胴体
	Boss2Body* boss2Body = nullptr;
	//墜落状態のカメラ位置に移動するか
	bool isMoveCrashPos = false;
	//墜落用の自機の中心座標からの距離
	Vector3 crashPlayerDistance = {};
	//墜落状態のカメラ位置に移動タイマー
	int32_t moveCrashPosTimer = 0;
	//墜落状態のカメラ位置に移動前の座標
	Vector3 moveCrashBeforePos;
	//墜落状態のカメラ位置に移動前の角度
	Vector3 moveCrashBeforeRota;
	//ステージクリア後カメラ挙動
	StageClearModePhase stageClearModePhase = StageClearModePhase::BossLook;
	//ステージクリア後のカメラ挙動に使用するタイマー
	int32_t stageClearModeTimer = 0;
	//ボスの方向を向くときのY座標
	float bossLookPosY = 0;
	//ステージクリア後の移動前座標
	Vector3 stageClearMoveBeforePos;
	//ステージクリア後の移動前カメラ角度
	Vector3 stageClearMoveBeforeRota;
};