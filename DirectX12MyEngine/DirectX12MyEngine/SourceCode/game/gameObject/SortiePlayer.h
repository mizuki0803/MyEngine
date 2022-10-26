#pragma once
#include "ObjObject3d.h"

/// <summary>
/// 出撃シーン用自機
/// </summary>
class SortiePlayer : public ObjObject3d
{
public:
	//出撃行動フェーズ
	enum class SortieActionPhase {
		Advance,	//前進
		Boost,		//ブースト
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>出撃シーン用自機</returns>
	static SortiePlayer* Create(ObjModel* model, const Vector3& startPosition);

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
	SortieActionPhase GetSortieActionPhase() { return sortieActionPhase; }
	bool GetIsSortieEnd() { return isSortieEnd; }

private: //メンバ関数
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
	static void (SortiePlayer::* sortieActionFuncTable[])();

private: //メンバ変数
	//出撃行動
	SortieActionPhase sortieActionPhase = SortieActionPhase::Advance;
	//出撃行動が終了したか
	bool isSortieEnd = false;
	//ブースト経過時間タイマー
	int32_t boostTimer = 0;
};
