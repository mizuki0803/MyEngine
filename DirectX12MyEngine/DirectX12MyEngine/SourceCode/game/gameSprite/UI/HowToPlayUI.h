#pragma once
#include "Sprite.h"
#include <memory>

/// <summary>
/// 遊び方UI
/// </summary>
class HowToPlayUI
{
public:
	//ス行動フェーズ
	enum class ActionPhase {
		Up,		//上に上がる
		Stay,	//停止
		Down,	//下に下がる
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>遊び方UI</returns>
	static HowToPlayUI* Create();

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

	//getter
	bool GetIsDead() { return isDead; }

private: //メンバ関数
	/// <summary>
	/// テキストを上に動かす
	/// </summary>
	void TextUp();

	/// <summary>
	/// テキストを停止させる
	/// </summary>
	void TextStay();

	/// <summary>
	/// テキストを下に動かす
	/// </summary>
	void TextDown();

private: //静的メンバ変数
	//行動遷移
	static void (HowToPlayUI::* actionFuncTable[])();
	//停止座標
	static const Vector2 stayPos;
	//画面外座標
	static const Vector2 outScreenPos;

private: //メンバ変数
	//チャージ説明スプライト
	std::unique_ptr<Sprite> chargeHowToPlaySprite;
	//行動
	ActionPhase phase = ActionPhase::Up;
	//行動用タイマー
	int32_t actionTimer = 0;
	//死亡フラグ
	bool isDead = false;
};