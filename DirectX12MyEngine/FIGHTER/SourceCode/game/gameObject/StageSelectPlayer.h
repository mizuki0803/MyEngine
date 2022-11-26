#pragma once
#include "ObjObject3d.h"

/// <summary>
/// ステージ選択シーン用自機
/// </summary>
class StageSelectPlayer : public ObjObject3d
{
public:
	//行動フェーズ
	enum class ActionPhase {
		GooutPlanet,	//惑星から出る
		StageSelect,	//ステージ選択
		EnterPlanet,	//惑星に入る
	};

	//惑星に入る行動フェーズ
	enum class EnterPlanetActionPhase {
		Rotate,	//回転
		Stay,	//ちょっと待機
		Boost,	//ブースト
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>出撃シーン用自機</returns>
	static StageSelectPlayer* Create(ObjModel* model, const Vector3& startPosition);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 惑星から出てくる行動を開始
	/// </summary>
	/// <param name="targetPos">目標座標</param>
	void GooutPlanetStart(const Vector3& targettPos);

	/// <summary>
	/// ステージ選択移動を開始
	/// </summary>
	/// <param name="targetPos">目標座標</param>
	void StageSelectMoveStart(const Vector3& targetPos, bool isStageSelectMoveRight);

	/// <summary>
	/// 惑星に入る行動を開始
	/// </summary>
	/// <param name="selectPlanetPos">選択した惑星の座標</param>
	void EnterPlanetStart(const Vector3& selectPlanetPos);

	//getter
	ActionPhase GetActionPhase() { return actionPhase; }
	bool GetIsGooutPlanetEnd() { return isGooutPlanetEnd; };
	bool GetIsStageSelectMove() { return isStageSelectMove; }
	bool GetIsStageSelectModeEnd() { return isStageSelectModeEnd; }

private: //メンバ関数
	/// <summary>
	/// 惑星から出る行動
	/// </summary>
	void GooutPlanet();

	/// <summary>
	/// ステージ選択移動
	/// </summary>
	void StageSelectMove();

	/// <summary>
	/// 惑星に入る行動
	/// </summary>
	void EnterPlanet();

	/// <summary>
	/// 惑星に入る行動(回転)
	/// </summary>
	void EnterPlanetRotate();

	/// <summary>
	/// 惑星に入る行動(ちょっと待機)
	/// </summary>
	void EnterPlanetStay();

	/// <summary>
	/// 惑星に入る行動(ブースト)
	/// </summary>
	void EnterPlanetBoost();

private: //静的メンバ変数
	//行動関数
	static void (StageSelectPlayer::* actionFuncTable[])();
	//行動関数
	static void (StageSelectPlayer::* enterPlanetActionFuncTable[])();

private: //メンバ変数
	//行動
	ActionPhase actionPhase = ActionPhase::GooutPlanet;
	//惑星に入る行動
	EnterPlanetActionPhase enterPlanetActionPhase = EnterPlanetActionPhase::Rotate;
	//惑星から出てくる行動を終えたか
	bool isGooutPlanetEnd = false;
	//ステージ選択で移動中か
	bool isStageSelectMove = false;
	//ステージ選択移動が右か
	bool isStageSelectMoveRight = true;
	//ステージ選択で行う全ての行動を終えたか
	bool isStageSelectModeEnd = false;
	//移動前座標
	Vector3 moveBeforePos;
	//移動後座標
	Vector3 moveAfterPos;
	//変更前角度
	Vector3 changeBeforeRota;
	//変更後角度
	Vector3 changeAfterRota;
	//行動タイマー
	int32_t actionTimer = 0;
	//パーティクルジェットの大きさ変更用
	int32_t particleJetSizePhaseNum = 0;
};
