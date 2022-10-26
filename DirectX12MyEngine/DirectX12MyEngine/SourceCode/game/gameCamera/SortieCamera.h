#pragma once
#include <DirectXMath.h>
#include "Camera.h"

//出撃シーン用自機クラスを前方宣言
class SortiePlayer;

/// <summary>
/// 出撃シーン用カメラ
/// </summary>
class SortieCamera : public Camera
{
public:
	//出撃行動フェーズ
	enum class SortieActionPhase {
		Stay,					//自機が遠くから来るのを待つ
		RunningSideZoom,		//自機を並走&ズーム
		RunningSideSpeedMove,	//自機を並走していた速度で動く
	};

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	//getter
	bool GetIsZoomEnd() const { return isZoomEnd; }

	//setter
	void SetSortiePlayer(SortiePlayer* player) { this->player = player; }

private: //メンバ関数
	/// <summary>
	/// 自機が遠くから来るのを待つ
	/// </summary>
	void StayPlayer();

	/// <summary>
	/// 自機を並走ズーム
	/// </summary>
	void RunningSideZoomPlayer();

	/// <summary>
	/// 自機並走していた速度で動かす
	/// </summary>
	void CameraRunningSideSpeedMove();

private: //静的メンバ変数
	//出撃行動遷移
	static void (SortieCamera::* sortieActionFuncTable[])();
	//並走を開始する自機とカメラの距離
	static float runningSideStartDistance;

private: //メンバ変数
	//プレイヤー自機
	SortiePlayer* player = nullptr;
	//カメラ視点と自機座標の距離
	Vector3 eyePlayerDistance;
	//出撃カメラ挙動
	SortieActionPhase sortieMovePhase = SortieActionPhase::Stay;
	//カメラ行動用タイマー
	int32_t cameraActionTimer = 0;
	//自機ズームを終了するか
	bool isZoomEnd = false;
};