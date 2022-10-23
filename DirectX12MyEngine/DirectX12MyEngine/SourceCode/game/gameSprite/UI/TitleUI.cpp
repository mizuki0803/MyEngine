#include "TitleUI.h"
#include "SpriteTextureLoader.h"

TitleUI* TitleUI::Create()
{
	//タイトルUIのインスタンスを生成
	TitleUI* titleUI = new TitleUI();
	if (titleUI == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!titleUI->Initialize()) {
		delete titleUI;
		assert(0);
		return nullptr;
	}

	return titleUI;
}

bool TitleUI::Initialize()
{
	//タイトルロゴスプライト生成
	titleSprite.reset(Sprite::Create(SpriteTextureLoader::TitleLogo));
	const Vector2 titlePos = { 640, 100 };
	titleSprite->SetPosition(titlePos);
	const Vector2 titleSize = { 300, 100 };
	titleSprite->SetSize(titleSize);
	titleSprite->SetTexSize(titleSize);

	//Bボタンスプライト生成	
	bButtonSprite.reset(Sprite::Create(SpriteTextureLoader::BButton));
	const Vector2 bButtonPos = { 640, 630 };
	bButtonSprite->SetPosition(bButtonPos);
	const Vector2 bButtonSize = { 64, 64 };
	bButtonSprite->SetSize(bButtonSize);
	bButtonSprite->SetTexSize(bButtonSize);

	return true;
}

void TitleUI::Update()
{
	//ボタン表示切り替え
	DrawChangeButtonSprite();

	//タイトルロゴスプライト更新
	titleSprite->Update();
	//Bボタンスプライト更新
	if (isDrawButtonSprite) {
		bButtonSprite->Update();
	}
}

void TitleUI::Draw()
{
	//タイトルロゴスプライト描画
	titleSprite->Draw();
	//Bボタンスプライト描画
	if (isDrawButtonSprite) {
		bButtonSprite->Draw();
	}
}

void TitleUI::DrawChangeButtonSprite()
{
	//切り替える時間
	const int drawChangeTime = 50;
	//タイマー更新
	drawTimer++;

	//タイマーが指定した時間になったら
	if (drawTimer >= drawChangeTime) {
		//表示切り替え
		if (isDrawButtonSprite) { isDrawButtonSprite = false; }
		else { isDrawButtonSprite = true; }

		//タイマー初期化
		drawTimer = 0;
	}
}
