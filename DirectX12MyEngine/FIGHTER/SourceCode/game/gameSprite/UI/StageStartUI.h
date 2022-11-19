#pragma once
#include "Sprite.h"
#include <memory>

/// <summary>
/// ステージ開始UI
/// </summary>
class StageStartUI
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ステージ開始UI</returns>
	static StageStartUI* Create(const int stageNum);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(const int stageNum);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//getter
	bool GetIsDead() { return isDead; }

private: //メンバ変数
	//ステージ情報スプライト
	std::unique_ptr<Sprite> stageInfoSprite;
	//寿命タイマー
	int32_t deadTimer = 0;
	//死亡フラグ
	bool isDead = false;
};