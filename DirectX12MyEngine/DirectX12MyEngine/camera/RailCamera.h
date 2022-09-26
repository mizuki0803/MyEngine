#pragma once
#include <DirectXMath.h>
#include "Camera.h"

//自機クラスを前方宣言
class Player;

/// <summary>
/// レールカメラ
/// </summary>
class RailCamera : public Camera
{
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
	void ShakeStart();

	//setter
	void SetPlayer(Player* player) { this->player = player; }
	void SetIsAdvance(const bool isAdvance) { this->isAdvance = isAdvance; }

private: //メンバ関数
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

private: //メンバ変数
	//プレイヤー自機
	Player* player = nullptr;
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
};