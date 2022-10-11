#pragma once
#include "ObjObject3d.h"

/// <summary>
/// タイトルシーン用自機
/// </summary>
class TitlePlayer : public ObjObject3d
{
public: //静的メンバ関数
	/// <summary>
	/// 生成処理
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>タイトルシーン用自機</returns>
	static TitlePlayer* Create(ObjModel* model, const Vector3& startPosition);

public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update() override;

	//getter
	bool GetIsSortie() { return isSortie; }

	//setter
	void SetIsSortie(bool isSortie) { this->isSortie = isSortie; }

private: //メンバ関数
	/// <summary>
	/// 出撃
	/// </summary>
	void Sortie();

private: //メンバ変数
	//出撃するか
	bool isSortie = false;
	//出撃開始までエンジンをふかす時間タイマー
	int32_t sortieStartTimer = 0;
};
