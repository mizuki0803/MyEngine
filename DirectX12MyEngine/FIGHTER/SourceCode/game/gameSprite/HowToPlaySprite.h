#pragma once
#include "Sprite.h"

/// <summary>
/// 遊び方スプライト
/// </summary>
class HowToPlaySprite : public Sprite
{
public:
	//行動フェーズ
	enum class ActionPhase {
		Up,		//上に上がる
		Stay,	//停止
		Down,	//下に下がる
	};

public:
	/// <summary>
	/// 生成処理
	/// </summary>	
	/// <param name="texture">テクスチャ</param>
	/// <param name="size">大きさ</param>
	/// <param name="texSize">テクスチャサイズ</param>
	/// <param name="leftTop">左上座標</param>
	/// <returns>遊び方スプライト</returns>
	static HowToPlaySprite* Create(const Texture& texture, const Vector2& size, const Vector2& texSize, const Vector2& leftTop);

public: //メンバ変数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

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
	static void (HowToPlaySprite::* actionFuncTable[])();
	//停止座標
	static const Vector2 stayPos;
	//画面外座標
	static const Vector2 outScreenPos;

private: //メンバ変数
	//行動
	ActionPhase phase = ActionPhase::Up;
	//行動用タイマー
	int32_t actionTimer = 0;
	//死亡フラグ
	bool isDead = false;
};