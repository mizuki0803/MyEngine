#pragma once
#include "Vector3.h"
#include <list>

/// <summary>
/// 自機の飛行機雲エフェクト
/// </summary>
class PlayerVaporEffect
{
public: //メンバ関数
	/// <summary>
	/// 更新
	/// </summary>
	void Update(const Vector3& leftWingPos, const Vector3& rightWingPos);

	/// <summary>
	/// 飛行機雲演出を開始する
	/// </summary>
	void VaporStart();

	/// <summary>
	/// 飛行機雲演出を終了する
	/// </summary>
	void VaporEnd();

private: //メンバ関数
	/// <summary>
	/// 飛行機雲生成
	/// </summary>
	/// <param name="wingPosList">翼の直近の座標リスト</param>
	/// <param name="segmentCount">分割数</param>
	void VaporCreate(std::list<Vector3>& wingPosList, const int segmentCount);

private: //メンバ変数
	//直近3フレーム分の左翼の座標
	std::list<Vector3> leftWingPosList;
	//直近3フレーム分の右翼の座標
	std::list<Vector3> rightWingPosList;
};