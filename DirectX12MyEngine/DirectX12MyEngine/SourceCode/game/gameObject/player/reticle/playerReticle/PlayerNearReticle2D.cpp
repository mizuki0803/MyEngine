#include "PlayerNearReticle2D.h"

const XMFLOAT4 PlayerNearReticle2D::chargeColor = { 0.9f, 0.9f, 0.1f, 1 };

PlayerNearReticle2D* PlayerNearReticle2D::Create(UINT texNumber, const Vector2& size)
{
	//自機付属の2Dレティクル(近)のインスタンスを生成
	PlayerNearReticle2D* playerNearReticle2D = new PlayerNearReticle2D();
	if (playerNearReticle2D == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!playerNearReticle2D->Initialize(texNumber, { 0.5f, 0.5f }, false, false)) {
		delete playerNearReticle2D;
		assert(0);
		return nullptr;
	}

	//大きさをセット
	playerNearReticle2D->size = size;

	return playerNearReticle2D;
}

void PlayerNearReticle2D::ChargeModeStart()
{
	//チャージ状態の色に変更
	color = chargeColor;
}

void PlayerNearReticle2D::ChargeModeEnd()
{
	//色を通常色に戻す
	color = normalColor;
}
