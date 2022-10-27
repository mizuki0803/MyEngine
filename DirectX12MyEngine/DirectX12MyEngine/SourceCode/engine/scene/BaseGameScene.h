#pragma once
#include "BaseScene.h"
#include <DirectXMath.h>

/// <summary>
/// ゲーム固有の基底シーン
/// </summary>
class BaseGameScene : public BaseScene
{
protected: //メンバ関数
	/// <summary>
	/// シーン変更情報をセットし、シーン変更を開始する
	/// </summary>
	/// <param name="color">画面を覆う色</param>
	/// <param name="deepenTime">色を濃くする時間</param>
	/// <param name="returnTime">色を戻す時間</param>
	/// <param name="nextSceneName">シーン変更後のシーン名</param>
	void SceneChangeStart(const DirectX::XMFLOAT4& color, int32_t deepenTime, int32_t returnTime, const std::string& nextSceneName);

	/// <summary>
	/// シーン変更状態
	/// </summary>
	void SceneChangeMode();

protected: //メンバ変数
	//シーン変更にかかる時間
	int32_t sceneChangeTime = 0;
	//シーン変更開始からのタイマー
	int32_t sceneChangeTimer = 0;
	//シーン変更状態か
	bool isSceneChange = false;
	//次のシーン
	std::string nextSceneName;
};
