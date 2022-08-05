#pragma once
#include "Sprite.h"

/// <summary>
/// プレイヤー用HPバー
/// </summary>
class PlayerHPBar : public Sprite
{
public:
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>プレイヤー用HPバー</returns>
	static PlayerHPBar* Create(UINT texNumber, const Vector2 position, const int maxHP);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	/// <summary>
	/// バーの長さ変更情報をセット
	/// </summary>
	/// <param name="HP">プレイヤーのHP</param>
	void SetChangeLength(const int HP);


private: //メンバ関数
	/// <summary>
	/// バーの長さ変更
	/// </summary>
	/// <param name="HPDifference">HPの差分</param>
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
};
