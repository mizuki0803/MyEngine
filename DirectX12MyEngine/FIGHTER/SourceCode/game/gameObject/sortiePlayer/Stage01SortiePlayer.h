#pragma once
#include "ObjObject3d.h"

/// <summary>
/// ステージ01出撃シーン用自機
/// </summary>
class Stage01SortiePlayer : public ObjObject3d
{
public:
	//ステージ01出撃行動フェーズ
	enum class Stage01SortieActionPhase {
		Advance,	//前進
		Boost,		//ブースト
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>ステージ01出撃シーン用自機</returns>
	static Stage01SortiePlayer* Create(ObjModel* model, const Vector3& startPosition);

	//getter
	static const float GetAdvanceSpeed() { return Stage01SortiePlayer::advanceSpeed; }

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// ブースト状態にする
	/// </summary>
	void BoostStart();

	//getter
	const Vector3& GetJetPos() { return jetPos; }
	Stage01SortieActionPhase GetSortieActionPhase() { return sortieActionPhase; }
	bool GetIsSortieEnd() { return isSortieEnd; }

private: //メンバ関数
	/// <summary>
	/// ジェット発射座標を更新
	/// </summary>
	void UpdateJetPos();

	/// <summary>
	/// 出撃前進
	/// </summary>
	void SortieAdvance();

	/// <summary>
	/// 出撃ブースト
	/// </summary>
	void SortieBoost();

private: //静的メンバ変数
	//出撃行動遷移
	static void (Stage01SortiePlayer::* sortieActionFuncTable[])();
	//通常前進スピード
	static const float advanceSpeed;

private: //メンバ変数
	//ジェット発射座標
	Vector3 jetPos = {};
	//出撃行動
	Stage01SortieActionPhase sortieActionPhase = Stage01SortieActionPhase::Advance;
	//出撃行動が終了したか
	bool isSortieEnd = false;
	//ブースト経過時間タイマー
	int32_t boostTimer = 0;
	//パーティクルジェットの大きさ変更用
	int32_t particleJetSizePhaseNum = 0;
};
