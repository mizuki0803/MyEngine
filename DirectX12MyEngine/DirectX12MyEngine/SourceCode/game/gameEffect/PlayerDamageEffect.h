#pragma once
#include "Sprite.h"
#include <memory>

/// <summary>
/// 自機ダメージ演出
/// </summary>
class PlayerDamageEffect
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>自機ダメージ演出</returns>
	static PlayerDamageEffect* Create();

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// ダメージ演出を開始する
	/// </summary>
	/// <param name="maxHP">最大HP</param>
	/// <param name="HP">HP</param>
	void DamageEffectStart(const int maxHP, const int HP);

private: //メンバ関数
	/// <summary>
	/// ダメージ演出
	/// </summary>
	void DamageEffect();

private: //メンバ変数
	//演出用スプライト
	std::unique_ptr<Sprite> effectSprite;
	//画面を覆う色の濃さ
	float colorDeepNum = 0;
	//描画時間タイマー
	int32_t drawTimer = 0;
	//描画するか
	bool isDraw = false;
};
