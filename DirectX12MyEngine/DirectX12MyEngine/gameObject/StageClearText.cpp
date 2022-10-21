#include "StageClearText.h"
#include "SpriteTextureLoader.h"

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
	stageNameSprite.reset(Sprite::Create(SpriteTextureLoader::PlayerHPGaugeOut));
	stageNameSprite->SetPosition({ 640, 200 });

	//文字数の分、文字スプライト生成
	for (int i = 0; i < textNum; i++) {
		textSprites[i].reset(Sprite::Create(SpriteTextureLoader::StageClearText));
		const Vector2 size = { 200, 200 };
		const Vector2 texSize = { 155, 155 };
		const Vector2 leftTop = { i * texSize.x, 0 };
		const float distance = size.x;
		const Vector2 centerPos = { 640, 400 };
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
	//全文字を表示していなければ
	if (displayTextNum < textNum) {
		//表示する文字数を増やす
		UpdateDisplayNum();
	}
	//表示していたら
	else {
		//テキストを画面外に移動させる
		TextMove();
	}


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

void StageClearText::TextMove()
{
	//移動を開始する時間
	const int moveStartTime = 120;
	//タイマー更新
	moveTimer++;

	//タイマーが開始時間より小さいなら抜ける
	if (moveTimer < moveStartTime) { return; }

	//文字を画面外に移動させる
	const float moveSpeed = 80.0f;

	for (int i = 0; i < textNum; i++) {
		Vector2 velocity = { moveSpeed, 0 };
		//左側の文字は左、右側の文字は右に移動
		if ((i % 2) == 0) { velocity.x = -velocity.x; }
		Vector2 pos = textSprites[i]->GetPosition();
		pos += velocity;
		textSprites[i]->SetPosition(pos);
	}
}
