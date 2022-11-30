#include "BossWarning.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"
#include "WindowApp.h"

const float BossWarning::warningColorAlpha = 0.2f;

BossWarning* BossWarning::Create(int32_t warningTime)
{
	//ボス登場警告のインスタンスを生成
	BossWarning* bossWarning = new BossWarning();
	if (bossWarning == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!bossWarning->Initialize()) {
		delete bossWarning;
		assert(0);
		return nullptr;
	}

	//警告時間をセット
	bossWarning->warningTime = warningTime;

	return bossWarning;
}

bool BossWarning::Initialize()
{
	//画面赤スプライト生成
	screenRedSprite.reset(Sprite::Create(SpriteTextureLoader::White, { 0, 0 }));
	//大きさをセット
	screenRedSprite->SetSize({ WindowApp::window_width, WindowApp::window_height });
	//色をセット
	screenRedSprite->SetColor({ 1, 0, 0, 0 });


	//左上座標をセット
	Vector2 leftTop[2] = { {}, {0, 567} };
	for (int i = 0; i < 2; i++) {
		bossWarningSprites[i].reset(BossWarningSprites::Create(leftTop[i]));
	}

	return true;
}

void BossWarning::Update()
{
	//警告
	Warning();

	//画面赤スプライト更新
	screenRedSprite->Update();
	//警告スプライト群更新
	for (int i = 0; i < 2; i++) {
		bossWarningSprites[i]->Update();
	}
}

void BossWarning::Draw()
{
	//画面赤スプライト描画
	screenRedSprite->Draw();
	//警告スプライト群更新
	for (int i = 0; i < 2; i++) {
		bossWarningSprites[i]->Draw();
	}
}

void BossWarning::Warning()
{
	//タイマー更新
	warningTimer++;

	//警告画面赤開始
	WarningStart(warningTimer);
	//スプライト群警告処理
	for (int i = 0; i < 2; i++) {
		bossWarningSprites[i]->Warning();
	}

	//警告画面赤終了
	WarningEnd(warningTimer);

	//タイマーが指定した時間になったら
	if (warningTimer >= warningTime) {
		//削除状態にする
		isDelete = true;
	}
}

void BossWarning::WarningStart(int32_t warningTimer)
{
	//警告開始にかかる時間
	const int warningStartTime = 20;
	//タイマーが時間を越えていたら抜ける
	if (warningTimer > warningStartTime) { return; }

	//イージング用時間計測
	const float time = (float)warningTimer / warningStartTime;

	//イージングで画面を赤くしていく(実際には色を濃くする)
	DirectX::XMFLOAT4 color = screenRedSprite->GetColor();
	color.w = Easing::OutQuad(0, warningColorAlpha, time);
	screenRedSprite->SetColor(color);

	//スプライト群の警告開始処理
	for (int i = 0; i < 2; i++) {
		bossWarningSprites[i]->WarningStart(time);
	}
}

void BossWarning::WarningEnd(int32_t warningTimer)
{
	//元に戻すのにかかる時間
	const int warningEndTime = 20;
	//警告終了に合わせるので終了を始める時間を計算
	const int screenColorReturnStartTime = warningTime - warningEndTime;

	//タイマーが警告終了に合わせるので終了を始める時間以下なら抜ける
	if (warningTimer < screenColorReturnStartTime) { return; }

	//イージング用時間計測
	const float time = (float)(warningTimer - screenColorReturnStartTime) / warningEndTime;

	//イージングで画面赤を戻していく(実際には色を薄くする)
	DirectX::XMFLOAT4 color = screenRedSprite->GetColor();
	color.w = Easing::InQuad(warningColorAlpha, 0, time);
	screenRedSprite->SetColor(color);

	//スプライト群の警告終了処理
	for (int i = 0; i < 2; i++) {
		bossWarningSprites[i]->WarningEnd(time);
	}
}
