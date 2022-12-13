#pragma once
#include <DirectXMath.h>
#include "Camera.h"

//自機クラスを前方宣言
class Player;
//ボス本体を前方宣言
class BossMainBody;

/// <summary>
/// ゲームカメラ
/// </summary>
class GameCamera : public Camera
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

public: //静的メンバ関数

	//getter
	static float GetAdvanceSpeed() { return GameCamera::advanceSpeed; }
	static float GetHighSpeedMagnification() { return GameCamera::highSpeedMagnification; }
	static float GetSlowSpeedMagnification() { return GameCamera::slowSpeedMagnification; }

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
	/// 墜落開始
	/// </summary>
	void CrashStart();

	/// <summary>
	/// シェイク開始
	/// </summary>
	void ShakeStart(const float shakePower = 20, const float shakeTime = 30);

	/// <summary>
	/// ステージクリア後の動きを開始する
	/// </summary>
	/// <param name="bossMainBody">ボス本体</param>
	void StageClearModeStart(BossMainBody* bossMainBody);

	/// <summary>
	/// ボスが削除されたら
	/// </summary>
	void BossDelete();

	//getter
	const Vector3& GetPosition() { return position; }

	//setter
	void SetPlayer(Player* player) { this->player = player; }
	void SetIsAdvance(const bool isAdvance) { this->isAdvance = isAdvance; }
	StageClearModePhase GetStageClearModePhase() { return stageClearModePhase; }

private: //メンバ関数
	/// <summary>
	/// カメラ動き
	/// </summary>
	void CameraAction();

	/// <summary>
	/// 墜落
	/// </summary>
	void Crash();

	/// <summary>
	/// 回転
	/// </summary>
	void Rotate();

	/// <summary>
	/// 移動
	/// </summary>
	void Move();

	/// <summary>
	/// プレイヤーに合わせてノックバックする
	/// </summary>
	void Knockback();

	/// <summary>
	/// シェイク
	/// </summary>
	void Shake();

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
	static void (GameCamera::* stageClearCameraActionFuncTable[])();
	//ステージ内を前進する速さ
	static const float advanceSpeed;
	//ハイスピードで移動するときの速度倍率
	static const float highSpeedMagnification;
	//遅いスピードで移動するときの速度倍率
	static const float slowSpeedMagnification;

private: //メンバ変数
	//プレイヤー自機
	Player* player = nullptr;
	//ボス本体
	BossMainBody* bossMainBody = nullptr;
	//回転
	Vector3 rotation = { 0, 0, 0 };
	//座標
	Vector3 position = { 0, 0, 0 };
	//前進するか
	bool isAdvance = true;
	//墜落状態か
	bool isCrash = false;
	//墜落状態のカメラ位置に移動するか
	bool isMoveCrashPos = false;
	//墜落状態のカメラ位置に移動タイマー
	int32_t moveCrashPosTimer = 0;
	//墜落状態のカメラ位置に移動前の座標
	Vector3 moveCrashBeforePos;
	//シェイクするか
	bool isShake = false;
	//カメラシェイク用タイマー
	int32_t shakeTimer = 0;
	//シェイクする時間
	float shakeTime = 0;
	//シェイク最大の強さ
	float maxShakePower = 0;
	//ステージクリア後の動きをするか
	bool isStageClearMode = false;
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