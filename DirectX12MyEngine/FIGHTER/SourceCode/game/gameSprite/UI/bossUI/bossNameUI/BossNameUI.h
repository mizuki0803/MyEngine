#pragma once
#include "Sprite.h"
#include <memory>
#include <array>

/// <summary>
/// ボス名表示UI
/// </summary>
class BossNameUI
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="bossType">ボスの種類</param>
	/// <returns>ボス名表示UI</returns>
	static BossNameUI* Create(int bossType);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="bossType">ボスの種類</param>
	/// <returns>成否</returns>
	bool Initialize(int bossType);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();


private: //メンバ関数
	/// <summary>
	/// 表示時間カウント
	/// </summary>
	void CountDrawTimer();


private: //静的メンバ変数
	static const int spriteNum = 2;

private: //メンバ変数
	//スプライト(二つ名も表示する為arrayにしておく 二つ名 : 0、名前 : 1)
	std::array<std::unique_ptr<Sprite>, spriteNum> sprites;
	//ボスの種類
	int bossType = 0;
	//表示時間タイマー
	int32_t drawTimer = 0;
	//表示フラグ
	bool isDraw = true;
};
