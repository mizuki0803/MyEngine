#include "StageClearText.h"
#include "SpriteTexture.h"

StageClearText* StageClearText::Create()
{
	//ステージクリア時に表示するテキストのインスタンスを生成
	StageClearText* stageClearText = new StageClearText();
	if (stageClearText == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!stageClearText->Initialize()) {
		delete stageClearText;
		assert(0);
		return nullptr;
	}

	return stageClearText;
}

bool StageClearText::Initialize()
{
	//ステージ名スプライト生成
	stageNameSprite.reset(Sprite::Create(SpriteTexture::PlayerHPGaugeOut));
	stageNameSprite->SetPosition({640, 200});

	//文字数の分、文字スプライト生成
	for (int i = 0; i < textNum; i++) {
		textSprites[i].reset(Sprite::Create(SpriteTexture::Number));
		const Vector2 size = { 100, 100 };
		const Vector2 texSize = { 32, 48 };
		const Vector2 leftTop = { i * texSize.x, 0 };
		const float distance = 120;
		const Vector2 centerPos = { 640, 350 };
		const Vector2 pos = { centerPos.x + ((i % 2) * distance) - (distance / 2), centerPos.y + ((i / 2) * distance) - (distance / 2) };
		textSprites[i]->SetPosition(pos);
		textSprites[i]->SetSize(size);
		textSprites[i]->SetTexSize(texSize);
		textSprites[i]->SetTexLeftTop(leftTop);
	}

	return true;
}

void StageClearText::Update()
{
	//表示する文字数を増やす
	UpdateDisplayNum();

	//ステージ名スプライト更新
	stageNameSprite->Update();
	//表示する文字のみ文字スプライト更新
	for (int i = 0; i < displayTextNum; i++) {
		textSprites[i]->Update();
	}
}

void StageClearText::Draw()
{
	//ステージ名スプライト描画
	stageNameSprite->Draw();
	//表示する文字のみ文字スプライト描画
	for (int i = 0; i < displayTextNum; i++) {
		textSprites[i]->Draw();
	}
}

void StageClearText::UpdateDisplayNum()
{
	//既に全文字を表示していたら抜ける
	if (displayTextNum >= textNum) { return; }

	//表示を開始する時間
	const int displayTime = 60;
	//タイマー更新
	displayTimer++;

	//タイマーが表示開始時間より小さいなら抜ける
	if (displayTimer < displayTime) { return; }


	//表示する文字数を増やす時間
	const int increaseTime = 20;

	//タイマーが文字数を増やす時間の倍数でないなら抜ける
	if (!((displayTimer % increaseTime) == 0)) { return; }

	//表示する文字数を増やす
	displayTextNum++;
}
