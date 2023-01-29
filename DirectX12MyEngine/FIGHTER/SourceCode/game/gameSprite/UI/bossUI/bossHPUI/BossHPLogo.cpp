#include "BossHPLogo.h"
#include "Easing.h"

BossHPLogo* BossHPLogo::Create(const Texture& texture, const Vector2& position)
{
	//HPUIロゴのインスタンスを生成
	BossHPLogo* bossHPLogo = new BossHPLogo();
	if (bossHPLogo == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!bossHPLogo->Initialize(texture, { 0.0f, 0.0f }, false, false)) {
		delete bossHPLogo;
		assert(0);
		return nullptr;
	}

	//表示する座標をセット
	bossHPLogo->stayPosition = position;
	//画面外にセットしておく
	bossHPLogo->position = position;

	return bossHPLogo;
}

void BossHPLogo::StartSetMove(int moveTime, int timer)
{
	//イージング計算用の時間
	float easeTimer = (float)timer / moveTime;

	//適当に画面外座標をセット
	const float screenOutPos = -50;
	//画面外から表示する座標に移動させる
	position.x = Easing::OutQuint(screenOutPos, stayPosition.x, easeTimer);
}
