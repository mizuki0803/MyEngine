#pragma once
#include "Sprite.h"

/// <summary>
/// ボス用HPバー
/// </summary>
class BossHPBar : public Sprite
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>ボス用HPバー</returns>
	static BossHPBar* Create(UINT texNumber, const Vector2& position, const int maxHP);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// 開始時の移動
	/// </summary>
	/// <param name="moveTime">移動にかかる時間</param>
	/// <param name="timer">タイマー</param>
	void StartSetMove(int moveTime, int timer);

	/// <summary>
	/// 開始時の大きさ変更
	/// </summary>
	/// <param name="changeTime">大きさ変更にかかる時間</param>
	/// <param name="timer">タイマー</param>
	void StartSetChangeSize(int changeTime,int timer);

	/// <summary>
	/// バーの長さ変更情報をセット
	/// </summary>
	/// <param name="HP">ボスのHP</param>
	void SetChangeLength(const int HP);

private: //メンバ関数
	/// <summary>
	/// バーの長さ変更
	/// </summary>
	void ChangeLength();


private: //メンバ変数
	//最大HP
	int maxHP = 0;
	//バーの長さ変更中か
	bool isChangeLength = false;
	//バーの長さ最大値
	const float lengthMax = 400;
	//バーの長さ変更タイマー
	int changeLengthTimer = 0;
	//バーの長さ変更前の長さ
	float changeLengthBefore = 0;
	//バーの長さ変更後の長さ
	float changeLengthAfter = 0;
	//表示する座標
	Vector2 stayPosition;
};
