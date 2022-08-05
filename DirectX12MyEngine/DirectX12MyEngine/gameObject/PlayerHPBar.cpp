#include "PlayerHPBar.h"
#include "Easing.h"

PlayerHPBar* PlayerHPBar::Create(UINT texNumber, const Vector2 position, const int maxHP)
{
	//HPバーのインスタンスを生成
	PlayerHPBar* playerHPBar = new PlayerHPBar();
	if (playerHPBar == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!playerHPBar->Initialize(texNumber, { 0.0f, 0.0f }, false, false)) {
		delete playerHPBar;
		assert(0);
		return nullptr;
	}

	//座標をセット
	playerHPBar->position = position;

	//最大HPをセット
	playerHPBar->maxHP = maxHP;

	return playerHPBar;
}

void PlayerHPBar::Update()
{
	//長さ変更状態のときのみ長さ変更
	if (isChangeLength)
	{
		ChangeLength();
	}

	//スプライトの更新
	Sprite::Update();
}

void PlayerHPBar::SetChangeLength(const int HP)
{
	//長さ変更タイマーを初期化
	changeLengthTimer = 0;
	//変更前の長さ(現在の状態)をセット
	changeLengthBefore = size.x;
	//変更後の長さをセット
	changeLengthAfter = lengthMax * ((float)HP / maxHP);
	//長さ変更状態にする
	isChangeLength = true;
}

void PlayerHPBar::ChangeLength()
{
	//長さ変更を行う時間
	const int changeTime = 60;

	//長さ変更タイマー更新
	changeLengthTimer++;

	//イージング計算用の時間
	float easeTimer = (float)changeLengthTimer / changeTime;

	//スプライトのサイズを変更
	size.x = Easing::OutQuint(changeLengthBefore, changeLengthAfter, easeTimer);
	//テクスチャサイズも更新したサイズに合わせる
	texSize = size;

	//タイマーが指定した時間になったら
	if (changeLengthTimer >= changeTime)
	{
		//長さ変更終了
		isChangeLength = false;
	}
}
