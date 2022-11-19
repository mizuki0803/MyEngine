#pragma once
#include <DirectXMath.h>
#include "Camera.h"

//ステージ選択シーン用自機クラスを前方宣言
class StageSelectPlayer;

/// <summary>
/// 出撃シーン用カメラ
/// </summary>
class StageSelectCamera : public Camera
{
public:
	//行動フェーズ
	enum class ActionPhase {
		GooutPlayerLook,	//惑星から出てくる自機を見る
		StageSelectStart,	//ステージ選択開始
		StageSelect,		//ステージ選択
		StageDecisionCheck,	//ステージ決定確認
		ReturnSelect,		//ステージ選択に戻る
	};

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 惑星から出てくる自機を見るの動き開始
	/// </summary>
	void GooutPlayerLookActionStart(const Vector3& gooutPlanetPos, const Vector3& selectFieldPos);

	/// <summary>
	/// ステージ選択開始の動き開始
	/// </summary>
	void StageSelectStartActionStart();

	/// <summary>
	/// ステージ決定確認の動き開始
	/// </summary>
	void StageDecisionCheckActionStart();

	/// <summary>
	/// ステージ選択に戻る動き開始
	/// </summary>
	void ReturnSelectActionStart();

	//getter
	ActionPhase GetActionPhase() { return phase; }

	//setter
	void SetStageSelectPlayer(StageSelectPlayer* player) { this->player = player; }

private: //メンバ関数
	/// <summary>
	/// 惑星から出てくる自機を見るの動き
	/// </summary>
	void GooutPlayerLookAction();

	/// <summary>
	/// ステージ選択開始の動き
	/// </summary>
	void StageSelectStartAction();

	/// <summary>
	/// ステージ選択状態の動き
	/// </summary>
	void StageSelectAction();

	/// <summary>
	/// ステージ決定確認の動き
	/// </summary>
	void StageDecisionCheckAction();

	/// <summary>
	/// ステージ選択に戻る状態の動き
	/// </summary>
	void ReturnSelectAction();

private: //静的メンバ変数
	//行動遷移
	static void (StageSelectCamera::* actionFuncTable[])();
	//惑星から出てくる自機を見る状態のステージ選択フィールドとカメラの距離
	static const Vector3 gooutPlayerLookSelectFieldDistance;
	//ステージ選択状態で並走する自機とカメラの距離
	static const Vector3 stageSelectPlayerDistance;
	//ステージ決定確認状態で並走する自機とカメラの距離
	static const Vector3 stageDecisionCheckPlayerDistance;

private: //メンバ変数
	//プレイヤー自機
	StageSelectPlayer* player = nullptr;
	//カメラ挙動
	ActionPhase phase = ActionPhase::StageSelect;
	//カメラ行動用タイマー
	int32_t cameraActionTimer = 0;
	//自機が出てくる惑星の座標
	Vector3 gooutPlanetPos;
	//移動前視点
	Vector3 moveBeforeEye;
	//移動前注視点
	Vector3 moveBeforeTarget;
};