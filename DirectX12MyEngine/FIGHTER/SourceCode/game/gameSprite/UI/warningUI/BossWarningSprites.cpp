#include "BossWarningSprites.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"
#include "WindowApp.h"

BossWarningSprites* BossWarningSprites::Create(const Vector2& leftTopPos)
{
	//ボス登場警告演出のインスタンスを生成
	BossWarningSprites* bossWarningEffect = new BossWarningSprites();
	if (bossWarningEffect == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!bossWarningEffect->Initialize(leftTopPos)) {
		delete bossWarningEffect;
		assert(0);
		return nullptr;
	}

	return bossWarningEffect;
}

bool BossWarningSprites::Initialize(const Vector2& leftTopPos)
{
	//ラインの本数文回す
	for (int i = 0; i < 2; i++) {
		//警告ライン用背景スプライト生成
		//大きさ
		const Vector2 backSize = { WindowApp::window_width, 30 };
		//表示座標
		Vector2 showPos = { WindowApp::window_width / 2, leftTopPos.y + backSize.y / 2 };
		if (i == 1) {
			//上のラインと下のラインの距離
			const float distanceLineToLine = 124.0f;
			showPos.y += distanceLineToLine;
		}
		warningLineBack[i].reset(WarningLineBack::Create(SpriteTextureLoader::White, showPos, backSize));

		//ラインを右方向に動かすか
		bool isMoveRight = true;
		if (i == 1) { isMoveRight = false; }
		warningLine[i].reset(WarningLine::Create(SpriteTextureLoader::WarningLine, showPos, isMoveRight));
	}

	//文字背景用スプライト生成
	//大きさ
	const Vector2 warningTextBackSize = { WindowApp::window_width, 98 };
	//座標
	Vector2 textBackPos = { WindowApp::window_width / 2, warningLineBack[0]->GetShowPos().y + warningLineBack[0]->GetShowSize().y / 2 + warningTextBackSize.y / 2 };
	warningTextBack.reset(WarningTextBack::Create(SpriteTextureLoader::White, textBackPos, warningTextBackSize));

	//警告テキスト生成
	Vector2 textPos = { WindowApp::window_width / 2, warningTextBack->GetPosition().y + warningTextBack->GetSize().y / 2 };
	warningText.reset(WarningText::Create(SpriteTextureLoader::WarningText, textPos));

	for (int i = 0; i < 2; i++) {
		//生成座標をセット
		Vector2 createPos = { WindowApp::window_width / 2, warningTextBack->GetPosition().y + warningTextBack->GetSize().y / 2 };
		warningLineBack[i]->StartSet(createPos);
		warningLine[i]->StartSet(createPos);
	}

	return true;
}

void BossWarningSprites::Update()
{
	for (int i = 0; i < 2; i++) {
		//警告ライン用背景スプライト更新
		warningLineBack[i]->Update();
		//警告ライン更新
		warningLine[i]->Update();
	}
	//文字背景用スプライト更新
	warningTextBack->Update();
	//文字スプライト更新
	warningText->Update();
}

void BossWarningSprites::Draw()
{
	//文字背景用スプライト描画
	warningTextBack->Draw();
	//文字スプライト描画
	warningText->Draw();
	for (int i = 0; i < 2; i++) {
		//警告ライン用背景スプライト更新
		warningLineBack[i]->Draw();
		//警告ライン更新
		warningLine[i]->Draw();
	}
}

void BossWarningSprites::Warning()
{
	//警告ラインスプライトを動かす(LeftTopをずらす)
	for (int i = 0; i < 2; i++) {
		warningLine[i]->LineMove();
	}
	//テキストの色変更
	warningText->TextColorChange();
}

void BossWarningSprites::WarningStart(float easeTimer)
{
	//警告開始処理
	for (int i = 0; i < 2; i++) {
		warningLine[i]->WarningStart(easeTimer);
		warningLineBack[i]->WarningStart(easeTimer);
	}
	warningText->WarningStart(easeTimer);
	warningTextBack->WarningStart(easeTimer);
}

void BossWarningSprites::WarningEnd(float easeTimer)
{
	//警告終了処理
	for (int i = 0; i < 2; i++) {
		warningLine[i]->WarningEnd(easeTimer);
		warningLineBack[i]->WarningEnd(easeTimer);
	}
	warningText->WarningEnd(easeTimer);
	warningTextBack->WarningEnd(easeTimer);
}
