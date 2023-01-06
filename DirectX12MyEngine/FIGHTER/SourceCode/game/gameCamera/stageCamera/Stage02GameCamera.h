#pragma once
#include "BaseGameCamera.h"

//ステージ02自機クラスを前方宣言
class Stage02Player;
//ギャラクシー胴体を前方宣言
class GalaxyBody;

/// <summary>
/// ステージ02ゲームカメラ
/// </summary>
class Stage02GameCamera : public BaseGameCamera
{
public:
	//ステージクリア後行動フェーズ
	enum class StageClearModePhase {
		BossSide,		//ボスの横に移動
		BossBack,		//ボスの後ろに移動
		PlayerFront,	//自機を正面に移動
		PlayerAround,	//自機の周りを回る
		PlayerBoostPos,	//自機のブーストを見る位置に移動
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
	void StageClearModeStart(GalaxyBody* galaxyBody);

	/// <summary>
	/// ステージクリア後の自機の正面に移動する行動を開始する
	/// </summary>
	void StageClearPlayerFrontStart();

	/// <summary>
	/// ステージクリア後の自機ブーストを見る位置に移動する行動を開始する
	/// </summary>
	void StageClearPlayerBoostLookPosStart();

	/// <summary>
	/// ボスの情報を削除
	/// </summary>
	void BossDelete();

	//setter
	StageClearModePhase GetStageClearModePhase() { return stageClearModePhase; }
	const float GetPlayerAroundRotAngle() { return playerAroundRotAngle; };
	const bool GetIsStageClearModeCompletion()  { return isStageClearModeCompletion; }

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
	/// ステージクリア後のボスの横に移動する行動
	/// </summary>
	void StageClearBossSide();

	/// <summary>
	/// ステージクリア後のボスの後ろに移動する行動
	/// </summary>
	void StageClearBossBack();

	/// <summary>
	/// ステージクリア後の自機の正面に移動する行動
	/// </summary>
	void StageClearPlayerFront();

	/// <summary>
	/// ステージクリア後の自機の周りを回る行動
	/// </summary>
	void StageClearPlayerAround();

	/// <summary>
	/// ステージクリア後の自機ブーストを見る位置に移動する行動
	/// </summary>
	void StageClearPlayerBoostLookPos();


private: //静的メンバ変数
	//ステージクリア後行動遷移
	static void (Stage02GameCamera::* stageClearCameraActionFuncTable[])();

private: //メンバ変数
	//ステージ01自機
	Stage02Player* player = nullptr;
	//ボス胴体
	GalaxyBody* galaxyBody = nullptr;
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
	StageClearModePhase stageClearModePhase = StageClearModePhase::BossSide;
	//ステージクリア後のカメラ挙動に使用するタイマー
	int32_t stageClearModeTimer = 0;
	//ステージクリア後の移動前座標
	Vector3 stageClearMoveBeforePos;
	//ステージクリア後の移動前カメラ角度
	Vector3 stageClearMoveBeforeRota;
	//自機周りを回るときの回転角度
	float playerAroundRotAngle = 0;
	//自機周りを回るときの自機との距離
	float playerAroundDistance = 0;
	//移動前の自機周りを回るときの回転角度
	float beforePlayerAroundRotAngle = 0;
	//移動前の自機周りを回るときの自機との距離
	float beforePlayerAroundDistance = 0;
	//カメラのステージクリア後が完了したか
	bool isStageClearModeCompletion = false;
};