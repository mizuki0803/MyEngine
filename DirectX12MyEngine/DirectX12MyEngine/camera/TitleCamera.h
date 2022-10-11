#pragma once
#include <DirectXMath.h>
#include "Camera.h"

//タイトルシーン用自機クラスを前方宣言
class TitlePlayer;

/// <summary>
/// タイトルシーン用カメラ
/// </summary>
class TitleCamera : public Camera
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
	/// 出撃開始
	/// </summary>
	void SortieStart();

	//setter
	void SetTitlePlayer(TitlePlayer* player) { this->player = player; }

private: //メンバ関数
	/// <summary>
	/// 出撃
	/// </summary>
	void Sortie();

private: //メンバ変数
	//プレイヤー自機
	TitlePlayer* player = nullptr;
	//ターゲットと視点の距離
	Vector3 eyeDistance;
	//回転角度
	float rotAngle = 0;
	//出撃開始一周の開始角度
	float beforeRoundRotAngle = 0;
	//出撃開始一周用タイマー
	int32_t rotRoundTimer = 0;
};