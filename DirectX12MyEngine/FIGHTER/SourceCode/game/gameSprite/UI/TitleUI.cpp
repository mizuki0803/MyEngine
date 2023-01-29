#include "TitleUI.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"

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
	titleSprite.reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::TitleLogo)));
	const Vector2 titlePos = { 640, 100 };
	titleSprite->SetPosition(titlePos);
	const Vector2 titleTexSize = { 500, 100 };
	titleSprite->SetTexSize(titleTexSize);
	const Vector2 titleSize = titleTexSize * 1.2f;
	titleSprite->SetSize(titleSize);

	//Bボタンスプライト生成	
	bButtonSprite.reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::BButton)));
	const Vector2 bButtonPos = { 640, 630 };
	bButtonSprite->SetPosition(bButtonPos);
	const Vector2 bButtonSize = { 64, 64 };
	bButtonSprite->SetSize(bButtonSize);
	bButtonSprite->SetTexSize(bButtonSize);

	return true;
}

void TitleUI::Update()
{
	//表示を終えていない場合
	if (!isShowEnd) {
		//ボタン表示切り替え
		DrawChangeButtonSprite();
	}
	//表示を終えている場合
	else {
		ShowEnd();
	}

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

	//表示を終えたら抜ける
	if (isShowEnd) { return; }

	//Bボタンスプライト描画
	if (isDrawButtonSprite) {
		bButtonSprite->Draw();
	}
}

void TitleUI::SetShowEnd()
{
	//表示を終える
	isShowEnd = true;

	//タイマーを初期化
	drawTimer = 0;
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

void TitleUI::ShowEnd()
{
	//表示を終える時間
	const float endTime = 20;
	//タイマーが指定した時間以上なら抜ける
	if (drawTimer >= endTime) { return; }

	//タイマー更新
	drawTimer++;
	const float time = drawTimer / endTime;

	//イージングで上に動かす
	Vector2 pos = titleSprite->GetPosition();
	pos.y = Easing::OutQuad(100, -100, time);
	titleSprite->SetPosition(pos);
}
