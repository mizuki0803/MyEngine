#pragma once
#include <DirectXMath.h>
#include "Camera.h"
#include "Vector2.h"

//自機の前方宣言
class Player;

/// <summary>
/// ゲームカメラ
/// </summary>
class GameCamera : public Camera
{
public: //静的メンバ関数
	//getter
	static float GetAdvanceSpeed() { return GameCamera::advanceSpeed; }
	static float GetHighSpeedMagnification() { return GameCamera::highSpeedMagnification; }
	static float GetSlowSpeedMagnification() { return GameCamera::slowSpeedMagnification; }

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// シェイク開始
	/// </summary>
	void ShakeStart(const float shakePower = 20, const float shakeTime = 30);

	//getter
	const Vector3& GetPosition() { return position; }

	//setter
	void SetIsAdvance(const bool isAdvance) { this->isAdvance = isAdvance; }

protected: //メンバ関数
	/// <summary>
	/// カメラ動き
	/// </summary>
	void CameraAction(Player* player);

	/// <summary>
	/// 墜落
	/// </summary>
	virtual void Crash() = 0;

	/// <summary>
	/// 回転
	/// </summary>
	void Rotate(const Vector3& playerRotation);

	/// <summary>
	/// 移動
	/// </summary>
	void Move(Player* player);

	/// <summary>
	/// プレイヤーに合わせてノックバックする
	/// </summary>
	void Knockback(Player* player);

	/// <summary>
	/// シェイク
	/// </summary>
	void Shake();

	/// <summary>
	/// ステージクリア後行動
	/// </summary>
	virtual void StageClear() = 0;


protected: //静的メンバ変数
	//ステージ内を前進する速さ
	static const float advanceSpeed;
	//ハイスピードで移動するときの速度倍率
	static const float highSpeedMagnification;
	//遅いスピードで移動するときの速度倍率
	static const float slowSpeedMagnification;

protected: //メンバ変数
	//回転
	Vector3 rotation = { 0, 0, 0 };
	//座標
	Vector3 position = { 0, 0, 0 };
	//カメラの移動限界
	Vector2 moveLimitMax = {};
	Vector2 moveLimitMin = {};
	//前進するか
	bool isAdvance = true;
	//墜落状態か
	bool isCrash = false;
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
};