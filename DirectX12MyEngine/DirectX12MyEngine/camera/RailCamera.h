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
public: //静的メンバ関数
	/// <summary>
	/// 自機をセット
	/// </summary>
	/// <param name="player">自機</param>
	static void SetPlayer(Player* player) { RailCamera::player = player; }


public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

private: //メンバ関数
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
	/// シェイク開始
	/// </summary>
	void ShakeStart();

	/// <summary>
	/// シェイク
	/// </summary>
	void Shake();

private: //静的メンバ変数
	//プレイヤー自機
	static Player* player;

private: //メンバ変数
	//回転
	Vector3 rotation = { 0, 0, 0 };
	//座標
	Vector3 position = { 0, 0, 0 };
	//シェイクするか
	bool isShake = false;
	//カメラシェイク用タイマー
	int32_t shakeTimer = 0;
};