#include "PlayerHPFrame.h"

const DirectX::XMFLOAT4 PlayerHPFrame::normalColor = { 0.57f, 0.56f, 0.5f, 1 };
const DirectX::XMFLOAT4 PlayerHPFrame::itemGetColor = { 0.95f, 0.95f, 0.1f, 1 };

PlayerHPFrame* PlayerHPFrame::Create(const Texture& texture, const Vector2& position)
{
	//HPバーフレームのインスタンスを生成
	PlayerHPFrame* playerHPFrame = new PlayerHPFrame();
	if (playerHPFrame == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!playerHPFrame->Initialize(texture, { 0.0f, 0.0f }, false, false)) {
		delete playerHPFrame;
		assert(0);
		return nullptr;
	}

	//座標をセット
	playerHPFrame->position = position;
	//色をセット
	playerHPFrame->color = normalColor;

	return playerHPFrame;
}

void PlayerHPFrame::Update()
{
	//アイテム獲得状態
	if (isItemGetMode) {
		ItemGetMode();
	}

	//スプライトの更新
	Sprite::Update();
}

void PlayerHPFrame::ItemGetModeStart()
{
	//タイマーを初期化
	itemGetModeTimer = 0;
	//アイテム獲得状態にする
	isItemGetMode = true;
}

void PlayerHPFrame::ItemGetMode()
{
	//アイテム獲得状態の時間
	const int itemGetModeTime = 80;
	//タイマー更新
	itemGetModeTimer++;

	//時間で色を黄色と通常色交互に変更
	if (itemGetModeTimer % 5 == 0) {
		//通常色ならアイテム獲得色へ変更
		if (isNormalColor) {
			color = itemGetColor;
			isNormalColor = false;
		}
		//アイテム獲得色なら通常色へ変更
		else {
			color = normalColor;
			isNormalColor = true;
		}
	}


	//タイマーが指定した時間になったら
	if (itemGetModeTimer >= itemGetModeTime) {
		//アイテム獲得状態終了
		isItemGetMode = false;

		//色を元に戻しておく
		isNormalColor = true;
		color = normalColor;
	}
}
