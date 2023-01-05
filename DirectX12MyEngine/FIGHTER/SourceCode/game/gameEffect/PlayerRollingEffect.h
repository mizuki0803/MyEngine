#pragma once
#include "Vector3.h"
#include <functional>
#include <vector>

/// <summary>
/// 自機の緊急回避エフェクト管理
/// </summary>
class PlayerRollingEffect
{
public:
	//生成用フェーズ
	enum class CreatePhase {
		Phase01, //フェーズ01
		Phase02, //フェーズ02
	};

	//エフェクト1つに必要な情報
	struct EffectParam {
		//生成時間
		int life = 0;
		//追従対象からの距離
		Vector3 localPos = {};
		//回転角
		float rotation = 0;
		//大きさ
		float size = 0;
		//色
		DirectX::XMFLOAT4 color = {};
	};

public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="getTargetPos">追従対象の座標取得関数</param>
	/// <param name="isRollRight">右回転か</param>
	/// <returns>自機の緊急回避エフェクト管理</returns>
	static PlayerRollingEffect* Create(std::function<Vector3()> getTargetPos, const bool isRollRight);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	//getter
	const bool GetIsDelete() { return isDelete; }

private: //メンバ関数
	/// <summary>
	/// 生成フェーズ01の行動
	/// </summary>
	void Phase01Create();

	/// <summary>
	/// 生成フェーズ02の行動
	/// </summary>
	void Phase02Create();

	/// <summary>
	/// エフェクトのパラメータ構造体を元にした生成処理
	/// </summary>
	void CreateEffects(const std::vector<EffectParam>& effectParams);

private: //静的メンバ変数
	//生成フェーズ関数
	static void (PlayerRollingEffect::* createPhaseFuncTable[])();

private: //メンバ変数
	//追従対象の座標取得関数
	std::function<Vector3()> getTargetPos;
	//生成フェーズ
	CreatePhase createPhase = CreatePhase::Phase01;
	//生成用タイマー
	int32_t createTimer = 0;
	//右回転か
	bool isRollRight = true;
	//削除フラグ
	bool isDelete = false;
};