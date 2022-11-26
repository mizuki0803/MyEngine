#include "BossNameUI.h"
#include "SpriteTextureLoader.h"

BossNameUI* BossNameUI::Create()
{
	//ボス名表示UIのインスタンスを生成
	BossNameUI* bossNameUI = new BossNameUI();
	if (bossNameUI == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!bossNameUI->Initialize()) {
		delete bossNameUI;
		assert(0);
		return nullptr;
	}

	return bossNameUI;
}

bool BossNameUI::Initialize()
{
	//二つ名スプライト生成
	sprites[0].reset(Sprite::Create(SpriteTextureLoader::BButton, { 0, 0 }));
	const Vector2 pos = { 200, 500 };
	sprites[0]->SetPosition(pos);

	return true;
}

void BossNameUI::Update()
{
	//表示をしない場合は抜ける
	if (!isDraw) { return; }

	//タイマー更新
	CountDrawTimer();

	//スプライト更新
	for (int i = 0; i < spriteNum; i++) {
		//インスタンスがなければ飛ばす
		if (!sprites[i]) { continue; }
		sprites[i]->Update();
	}
}

void BossNameUI::Draw()
{
	//表示をしない場合は抜ける
	if (!isDraw) { return; }

	//スプライト描画
	for (int i = 0; i < spriteNum; i++) {
		//インスタンスがなければ飛ばす
		if (!sprites[i]) { continue; }
		sprites[i]->Draw();
	}
}

void BossNameUI::CountDrawTimer()
{
	//タイマー更新
	drawTimer++;

	//指定した時間になったらボス名を表示
	const int nameSpriteCreateTime = 90;
	if (drawTimer >= nameSpriteCreateTime && !sprites[1]) {
		//ボス名スプライト生成
		sprites[1].reset(Sprite::Create(SpriteTextureLoader::BButton, { 0, 0 }));
		const Vector2 pos = { 200, 600 };
		sprites[1]->SetPosition(pos);
		sprites[1]->SetSize({ 128, 128 });
	}

	//さらに指定した時間になったら
	const int lifeTime = 300;
	if (drawTimer >= lifeTime) {
		//表示を終える
		isDraw = false;
	}
}
