#pragma once
#include "ObjObject3d.h"
#include "PlayerVaporEffect.h"

/// <summary>
/// タイトルシーン用自機
/// </summary>
class TitlePlayer : public ObjObject3d
{
public:
	//タイトル出撃行動フェーズ
	enum class SortieModePhase {
		Stay,		//停止
		Boost,		//ブースト
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>タイトルシーン用自機</returns>
	static TitlePlayer* Create(ObjModel* model, const Vector3& startPosition);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(ObjModel* model, const Vector3& startPosition);

	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	//getter
	bool GetIsSortie() { return isSortie; }
	const Vector3& GetJetPos() { return jetPos; }

	//setter
	void SetIsSortie(bool isSortie) { this->isSortie = isSortie; }

private: //メンバ関数
	/// <summary>
	/// 出撃
	/// </summary>
	void Sortie();

	/// <summary>
	/// 出撃状態の停止行動
	/// </summary>
	void SortieStay();

	/// <summary>
	/// 出撃状態のブースト行動
	/// </summary>
	void SortieBoost();

	/// <summary>
	/// ジェット発射座標を更新
	/// </summary>
	void UpdateJetPos();

	/// <summary>
	/// 両翼の座標を更新
	/// </summary>
	void UpdateWingPos();

private: //静的メンバ変数
	//出撃状態行動遷移
	static void (TitlePlayer::* sortieActionFuncTable[])();

private: //メンバ変数
	//出撃するか
	bool isSortie = false;
	//出撃用時間タイマー
	int32_t sortieTimer = 0;
	//出撃状態の行動
	SortieModePhase sortieModePhase = SortieModePhase::Stay;
	//ジェット発射座標
	Vector3 jetPos = {};
	//飛行機雲演出
	std::unique_ptr<PlayerVaporEffect> vaporEffect;
};
