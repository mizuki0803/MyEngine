#pragma once
#include <DirectXMath.h>
#include "Camera.h"
#include "Vector2.h"

//自機の前方宣言
class BasePlayer;

/// <summary>
/// ゲームカメラ基盤
/// </summary>
class BaseGameCamera : public Camera
{
public: //静的メンバ関数
	//getter
	static float GetHighSpeedMagnification() { return BaseGameCamera::highSpeedMagnification; }
	static float GetSlowSpeedMagnification() { return BaseGameCamera::slowSpeedMagnification; }

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
	float GetAdvanceSpeed() { return advanceSpeed; }

	//setter
	void SetIsAdvance(const bool isAdvance) { this->isAdvance = isAdvance; }

protected: //メンバ関数
	/// <summary>
	/// ワールド行列を更新
	/// </summary>
	/// <param name="matTrans"></param>
	void UpdateMatWorld(const XMMATRIX& matTrans);

	/// <summary>
	/// ゆらゆらを加算したワールド行列を更新
	/// </summary>
	void UpdateSwayMatWorld(const XMMATRIX& matTrans);

	/// <summary>
	/// 視点と注視点を更新
	/// </summary>
	void UpdateEyeTarget();

	/// <summary>
	/// カメラ動き
	/// </summary>
	void CameraAction(BasePlayer* player);

	/// <summary>
	/// 墜落
	/// </summary>
	virtual void Crash() = 0;

	/// <summary>
	/// 回転
	/// </summary>
	void Rotate(const Vector3& playerRotation);

	/// <summary>
	/// カメラゆらゆら回転
	/// </summary>
	void Sway();

	/// <summary>
	/// 移動
	/// </summary>
	void Move(BasePlayer* player);

	/// <summary>
	/// プレイヤーに合わせてノックバックする
	/// </summary>
	void Knockback(BasePlayer* player);

	/// <summary>
	/// シェイク
	/// </summary>
	void Shake();

	/// <summary>
	/// ステージクリア後行動
	/// </summary>
	virtual void StageClear() = 0;


protected: //静的メンバ変数
	//ハイスピードで移動するときの速度倍率
	static const float highSpeedMagnification;
	//遅いスピードで移動するときの速度倍率
	static const float slowSpeedMagnification;

protected: //メンバ変数
	//回転
	Vector3 rotation = { 0, 0, 0 };
	//座標
	Vector3 position = { 0, 0, 0 };
	//実際にカメラの視点,注視点,上ベクトルに使用するワールド変換行列
	XMMATRIX cameraMatWorld = {};
	//カメラの移動限界
	Vector2 moveLimitMax = {};
	Vector2 moveLimitMin = {};
	//前進するか
	bool isAdvance = true;
	//前進のスピード
	float advanceSpeed = 0;
	//墜落状態か
	bool isCrash = false;
	//x軸ゆらゆら回転が上回転か
	bool isSwayXUp = true;
	//x軸ゆらゆら回転用
	float swayX = 0.0f;
	//x軸ゆらゆらの速度
	float swayXSpeed = 0;
	//x軸ゆらゆらの速度が最大か
	bool isSwaySpeedMax = false;
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