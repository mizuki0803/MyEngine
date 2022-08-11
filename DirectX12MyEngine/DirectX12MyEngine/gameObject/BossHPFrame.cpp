#include "BossHPFrame.h"

BossHPFrame* BossHPFrame::Create(UINT texNumber, const Vector2 position)
{
	//HPバーフレームのインスタンスを生成
	BossHPFrame* bossHPFrame = new BossHPFrame();
	if (bossHPFrame == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!bossHPFrame->Initialize(texNumber, { 0.0f, 0.0f }, false, false)) {
		delete bossHPFrame;
		assert(0);
		return nullptr;
	}

	//座標をセット
	bossHPFrame->position = position;

	return bossHPFrame;
}
