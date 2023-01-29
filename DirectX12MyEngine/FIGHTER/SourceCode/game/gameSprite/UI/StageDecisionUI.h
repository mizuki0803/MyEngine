#pragma once
#include "Sprite.h"
#include <memory>

/// <summary>
/// ステージ決定確認UI
/// </summary>
class StageDecisionUI
{
public:
	//行動フェーズ
	enum class ActionPhase {
		Stay,	//停止
		Up,		//上に上がる
		Show,	//表示
		Down,	//下に下がる
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ステージ決定確認UI</returns>
	static StageDecisionUI* Create();

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
	/// テキストを上に動かす開始
	/// </summary>
	/// <param name="stageNum">ステージ番号</param>
	void TextUpStart(int stageNum);

	/// <summary>
	/// テキストを下に動かす開始
	/// </summary>
	void TextDownStart();

	/// <summary>
	/// ステージ決定するかを変更可能
	/// </summary>
	/// <param name="isStageDecision">ステージを決定するか</param>
	void ChangeStageDecision(bool isStageDecision);

	//getter
	ActionPhase GetActionPhase() { return phase; }
	bool GetIsStageDecision() { return isStageDecision; }

private: //メンバ関数
	/// <summary>
	/// テキストを待機させる
	/// </summary>
	void TextStay();

	/// <summary>
	/// テキストを上に動かす
	/// </summary>
	void TextUp();

	/// <summary>
	/// テキストを下に動かす
	/// </summary>
	void TextDown();

private: //静的メンバ変数
	//行動遷移
	static void (StageDecisionUI::* actionFuncTable[])();
	//停止座標
	static const Vector2 stayPos;
	//画面外座標
	static const Vector2 outScreenPos;

private: //メンバ変数
	//行動
	ActionPhase phase = ActionPhase::Stay;
	//ステージ決定確認スプライト
	std::unique_ptr<Sprite> stageDecisionSprite;
	//ステージを決定か
	bool isStageDecision = true;
	//移動時間タイマー
	int32_t moveTimer = 0;
	//状態を変更可能か
	bool isCanChangeDecision = false;
	//テクスチャの左上座標倍数
	bool isSelectTitleStage = false;
};