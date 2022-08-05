#include "PlayerHPFrame.h"

PlayerHPFrame* PlayerHPFrame::Create(UINT texNumber, const Vector2 position)
{
	//HPバーフレームのインスタンスを生成
	PlayerHPFrame* playerHPFrame = new PlayerHPFrame();
	if (playerHPFrame == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!playerHPFrame->Initialize(texNumber, { 0.0f, 0.0f }, false, false)) {
		delete playerHPFrame;
		assert(0);
		return nullptr;
	}

	//座標をセット
	playerHPFrame->position = position;

	return playerHPFrame;
}
