#include "BossHPBar.h"
#include "Easing.h"

BossHPBar* BossHPBar::Create(const Texture& texture, const Vector2& position, const int maxHP)
{
	//HPバーのインスタンスを生成
	BossHPBar* bossHPBar = new BossHPBar();
	if (bossHPBar == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!bossHPBar->Initialize(texture, { 0.0f, 0.0f }, false, false)) {
		delete bossHPBar;
		assert(0);
		return nullptr;
	}

	//表示する座標をセット
	bossHPBar->stayPosition = position;
	//画面外にセットしておく
	bossHPBar->position = position;

	//最大HPをセット
	bossHPBar->maxHP = maxHP;

	//大きさを0にしておく
	bossHPBar->size.y = 0;
	bossHPBar->texSize.y = 0;

	return bossHPBar;
}

void BossHPBar::Update()
{
	//長さ変更状態のときのみ長さ変更
	if (isChangeLength) {
		ChangeLength();
	}

	//スプライトの更新
	Sprite::Update();
}

void BossHPBar::StartSetMove(int moveTime, int timer)
{
	//イージング計算用の時間
	float easeTimer = (float)timer / moveTime;

	//適当に画面外座標をセット
	const float screenOutPos = -50;
	//画面外から表示する座標に移動させる
	position.x = Easing::OutQuint(screenOutPos, stayPosition.x, easeTimer);
}

void BossHPBar::StartSetChangeSize(int changeTime, int timer)
{
	//タイマーの値が変更にかかる時間を越えていたら抜ける
	if (timer > changeTime) { return; }

	//イージング計算用の時間
	float easeTimer = (float)timer / changeTime;

	//ゲージを下に伸ばしていく
	size.y = Easing::OutQuart(0, lengthMax, easeTimer);
	texSize = size;
}

void BossHPBar::SetChangeLength(const int HP)
{
	//長さ変更タイマーを初期化
	changeLengthTimer = 0;
	//変更前の長さ(現在の状態)をセット
	changeLengthBefore = size.y;
	//変更後の長さをセット
	changeLengthAfter = lengthMax * ((float)HP / maxHP);
	//長さ変更状態にする
	isChangeLength = true;
}

void BossHPBar::ChangeLength()
{
	//長さ変更を行う時間
	const int changeTime = 20;

	//長さ変更タイマー更新
	changeLengthTimer++;

	//イージング計算用の時間
	float easeTimer = (float)changeLengthTimer / changeTime;

	//スプライトのサイズを変更
	size.y = Easing::OutQuint(changeLengthBefore, changeLengthAfter, easeTimer);
	//テクスチャサイズも更新したサイズに合わせる
	texSize = size;

	//タイマーが指定した時間になったら
	if (changeLengthTimer >= changeTime) {
		//長さ変更終了
		isChangeLength = false;
	}
}
