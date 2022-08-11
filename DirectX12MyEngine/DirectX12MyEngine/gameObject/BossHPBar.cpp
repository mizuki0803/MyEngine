#include "BossHPBar.h"

BossHPBar* BossHPBar::Create(UINT texNumber, const Vector2 position, const int maxHP)
{
	//HPバーのインスタンスを生成
	BossHPBar* bossHPBar = new BossHPBar();
	if (bossHPBar == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!bossHPBar->Initialize(texNumber, { 0.0f, 0.0f }, false, false)) {
		delete bossHPBar;
		assert(0);
		return nullptr;
	}

	//座標をセット
	bossHPBar->position = position;

	//最大HPをセット
	bossHPBar->maxHP = maxHP;

	return bossHPBar;
}

void BossHPBar::ChangeLength(const int HP)
{
	//スプライトのサイズを変更
	size.y = lengthMax * ((float)HP / maxHP);
	//テクスチャサイズも更新したサイズに合わせる
	texSize = size;
}
