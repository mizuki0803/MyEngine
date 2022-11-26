#pragma once
#include "BossHPLogo.h"
#include "BossHPBar.h"
#include "BossHPFrame.h"
#include <memory>

/// <summary>
/// ボス用HPUI
/// </summary>
class BossHPUI
{
	/// <summary>
	/// 開始時セットフェーズ
	/// </summary>
	enum class StartSetPhase
	{
		MoveStayPos,	//画面外から移動
		ChangeSize,		//大きさ変更
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="position">座標</param>
	/// <param name="maxHP">最大HP</param>
	/// <returns>ボス用HPUI</returns>
	static BossHPUI* Create(const Vector2& position, const int maxHP);

public: //メンバ関数
	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize(const Vector2& position, const int maxHP);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	/// <summary>
	/// 被弾したのでバーの長さ変更情報をセット
	/// </summary>
	/// <param name="HP">ボスのHP</param>
	void Damage(const int HP);

private: //メンバ関数
	/// <summary>
	/// 開始時のセット
	/// </summary>
	void StartSet();

	/// <summary>
	/// 開始時の移動
	/// </summary>
	void StartSetMove();

	/// <summary>
	/// 開始時の大きさ変更
	/// </summary>
	void StartSetChangeSize();


private: //静的メンバ変数
	//開始時セット行動遷移
	static void (BossHPUI::* startSetActionFuncTable[])();

private: //メンバ変数
	//HPUIロゴ
	std::unique_ptr<BossHPLogo> hpLogo;
	//HPバー
	std::unique_ptr<BossHPBar> hpBar;
	//HPフレーム
	std::unique_ptr<BossHPFrame> hpFrame;
	//開始時のセットをするか
	bool isStartSet = true;
	//開始時セットフェーズ
	StartSetPhase startSetPhase = StartSetPhase::MoveStayPos;
	//開始時セット用タイマー
	int32_t startSetTimer = 0;
};
