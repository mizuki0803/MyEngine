#include "BossHPFrame.h"
#include "Easing.h"

BossHPFrame* BossHPFrame::Create(const Texture& texture, const Vector2& position)
{
	//HPバーフレームのインスタンスを生成
	BossHPFrame* bossHPFrame = new BossHPFrame();
	if (bossHPFrame == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!bossHPFrame->Initialize(texture, { 0.0f, 0.0f }, false, false)) {
		delete bossHPFrame;
		assert(0);
		return nullptr;
	}

	//枠の大きさをセット
	bossHPFrame->frameSize = bossHPFrame->texSize;

	//表示する座標をセット
	bossHPFrame->stayPosition = position;
	//画面外にセットしておく
	bossHPFrame->position = position;

	//大きさを0にしておく
	bossHPFrame->size.y = 0;
	bossHPFrame->texSize.y = 0;

	return bossHPFrame;
}

void BossHPFrame::StartSetMove(int moveTime, int timer)
{
	//イージング計算用の時間
	float easeTimer = (float)timer / moveTime;

	//適当に画面外座標をセット
	const float screenOutPos = -50;
	//画面外から表示する座標に移動させる
	position.x = Easing::OutQuint(screenOutPos, stayPosition.x, easeTimer);
}

void BossHPFrame::StartSetChangeSize(int changeTime, int timer)
{
	//タイマーの値が変更にかかる時間を越えていたら抜ける
	if (timer > changeTime) { return; }

	//イージング計算用の時間
	float easeTimer = (float)timer / changeTime;

	//ゲージを下に伸ばしていく
	size.y = Easing::OutQuart(0, frameSize.y, easeTimer);
	texSize = size;
}
