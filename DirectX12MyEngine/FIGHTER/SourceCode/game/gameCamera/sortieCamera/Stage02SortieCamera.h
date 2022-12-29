#pragma once
#include <DirectXMath.h>
#include "Camera.h"

//ステージ02出撃シーン用自機クラスを前方宣言
class Stage02SortiePlayer;

/// <summary>
/// ステージ02出撃シーン用カメラ
/// </summary>
class Stage02SortieCamera : public Camera
{
public:
	//ステージ02出撃行動フェーズ
	enum class Stage02SortieActionPhase {
		LookPlayer,		//自機の方向を向く
		ZoomPlayer,		//自機をズーム
		RunningSide,	//自機を並走
		Stay,			//カメラ停止
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
	void SetSortiePlayer(Stage02SortiePlayer* player) { this->player = player; }

private: //メンバ関数
	/// <summary>
	/// 自機の方向を向く
	/// </summary>
	void LookPlayer();

	/// <summary>
	/// 自機の方向を向く
	/// </summary>
	void ZoomPlayer();

	/// <summary>
	/// 自機を並走
	/// </summary>
	void RunningSidePlayer();

	/// <summary>
	/// 停止
	/// </summary>
	void Stay();

private: //静的メンバ変数
	//出撃行動遷移
	static void (Stage02SortieCamera::* sortieActionFuncTable[])();
	//並走時の視点と自機の距離
	static const Vector3 playerDistance;

private: //メンバ変数
	//プレイヤー自機
	Stage02SortiePlayer* player = nullptr;
	//出撃カメラ挙動
	Stage02SortieActionPhase sortieMovePhase = Stage02SortieActionPhase::LookPlayer;
	//カメラ行動用タイマー
	int32_t cameraActionTimer = 0;
	//イージング用カメラ移動前視点
	Vector3 moveBeforeEye = {};
	//自機ズームを終了するか
	bool isZoomEnd = false;
};