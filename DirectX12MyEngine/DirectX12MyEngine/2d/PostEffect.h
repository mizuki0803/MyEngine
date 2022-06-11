#pragma once
#include "Sprite.h"

class PostEffect :public Sprite
{
public:
	/// <summary>
	/// スプライト生成
	/// </summary>
	/// <param name="texNumber">テクスチャ番号</param>
	/// <param name="anchorpoint">アンカーポイント</param>
	/// <param name="isFlipX">左右反転するか</param>
	/// <param name="isFlipY">上下反転するか</param>
	/// <returns>Sprite</returns>
	static PostEffect* Create(UINT texNumber, XMFLOAT2 anchorpoint = { 0.5f, 0.5f }, bool isFlipX = false, bool isFlipY = false);


	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw();
};

