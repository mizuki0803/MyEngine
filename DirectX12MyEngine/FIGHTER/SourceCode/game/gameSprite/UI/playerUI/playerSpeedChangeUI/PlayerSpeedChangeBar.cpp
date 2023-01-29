#include "PlayerSpeedChangeBar.h"

PlayerSpeedChangeBar* PlayerSpeedChangeBar::Create(const Texture& texture, const Vector2& position, const float maxGaugeNum)
{
	//プレイヤー速度変更UIバーのインスタンスを生成
	PlayerSpeedChangeBar* playerSpeedChangeBar = new PlayerSpeedChangeBar();
	if (playerSpeedChangeBar == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!playerSpeedChangeBar->Initialize(texture, { 0.0f, 0.0f }, false, false)) {
		delete playerSpeedChangeBar;
		assert(0);
		return nullptr;
	}

	//座標をセット
	playerSpeedChangeBar->position = position;

	//最大ゲージ数をセット
	playerSpeedChangeBar->maxGaugeNum = maxGaugeNum;

	return playerSpeedChangeBar;
}

void PlayerSpeedChangeBar::ChangeLength(const float gaugeNum)
{
	//スプライトのサイズを変更
	size.x = lengthMax * (gaugeNum / maxGaugeNum);
	//テクスチャサイズも更新したサイズに合わせる
	texSize = size;
}
