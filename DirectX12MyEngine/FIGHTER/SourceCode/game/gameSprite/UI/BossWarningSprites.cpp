#include "BossWarningSprites.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"
#include "WindowApp.h"

const float BossWarning::warningColorAlpha = 0.4f;
const DirectX::XMFLOAT4 BossWarning::warningTextColorMax = { 0.9f, 0.01f, 0.07f, 1.0f };
const DirectX::XMFLOAT4 BossWarning::warningTextColorMin = { 0.7f, 0.15f, 0.1f, 1.0f };

BossWarning* BossWarning::Create(int32_t warningTime)
{
	//ボス登場警告演出のインスタンスを生成
	BossWarning* bossWarningEffect = new BossWarning();
	if (bossWarningEffect == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!bossWarningEffect->Initialize()) {
		delete bossWarningEffect;
		assert(0);
		return nullptr;
	}

	//警告時間をセット
	bossWarningEffect->warningTime = warningTime;

	return bossWarningEffect;
}

bool BossWarning::Initialize()
{
	//画面赤スプライト生成
	screenRedSprite.reset(Sprite::Create(SpriteTextureLoader::White, { 0, 0 }));
	//大きさをセット
	screenRedSprite->SetSize({ WindowApp::window_width, WindowApp::window_height });
	//色をセット
	screenRedSprite->SetColor({ 1, 0, 0, 0 });
	
	//上に表示する警告スプライト群
	{
		//ラインの本数文回す
		for (int i = 0; i < 2; i++) {
			//警告ライン用背景スプライト生成
			lineBackSprite[i].reset(Sprite::Create(SpriteTextureLoader::White));
			//大きさをセット
			const Vector2 backSize = { WindowApp::window_width, 29 };
			lineBackSprite[i]->SetSize(backSize);
			//座標をセット
			Vector2 pos = { WindowApp::window_width / 2, lineBackSprite[i]->GetSize().y / 2 };
			if (i == 1) {
				//上のラインと下のラインの距離
				const float distanceLineToLine = 124.0f;
				pos.y += distanceLineToLine;
			}
			lineBackSprite[i]->SetPosition(pos);
			//色をセット
			const DirectX::XMFLOAT4 backColor = { 0.9f, 0.5f, 0.5f, 0.8f };
			lineBackSprite[i]->SetColor(backColor);


			//警告ラインスプライト生成
			lineSprite[i].reset(Sprite::Create(SpriteTextureLoader::WarningLine));
			//座標をセット
			lineSprite[i]->SetPosition(pos);
			//色をセット
			const DirectX::XMFLOAT4 lineColor = { 1, 0.3f, 0.3f, 0.8f };
			lineSprite[i]->SetColor(lineColor);
		}

		//文字背景用スプライト生成
		textBackSprite.reset(Sprite::Create(SpriteTextureLoader::White, { 0.5f, 0 }));
		//座標をセット
		Vector2 textBackPos = { WindowApp::window_width / 2, lineBackSprite[0]->GetPosition().y + lineBackSprite[0]->GetSize().y / 2 };
		textBackSprite->SetPosition(textBackPos);
		//大きさをセット
		textBackSprite->SetSize({ WindowApp::window_width, 98 });
		//警告なので赤くしておく(じわじわ出したいので透明にしておく)
		DirectX::XMFLOAT4 textBackColor = { 0.6f, 0.2f, 0.2f, 0.9f };
		textBackSprite->SetColor(textBackColor);

		//文字スプライト生成
		textSprite.reset(Sprite::Create(SpriteTextureLoader::WarningText));
		//座標をセット(文字用背景と同じ座標をセット)
		Vector2 textPos = { WindowApp::window_width / 2, textBackSprite->GetPosition().y + textBackSprite->GetSize().y / 2 };
		textSprite->SetPosition(textPos);
		//警告なので赤くしておく
		textSprite->SetColor(warningTextColorMax);
	}
	//下に表示する警告スプライト群
	{
		//ラインの本数文回す
		for (int i = 0; i < 2; i++) {
			//警告ライン用背景スプライト生成
			lineBackSpriteDown[i].reset(Sprite::Create(SpriteTextureLoader::White));
			//大きさをセット
			const Vector2 backSize = { WindowApp::window_width, 29 };
			lineBackSpriteDown[i]->SetSize(backSize);
			//座標をセット
			Vector2 pos = { WindowApp::window_width / 2, 567 + lineBackSpriteDown[i]->GetSize().y / 2 };
			if (i == 1) {
				//上のラインと下のラインの距離
				const float distanceLineToLine = 124.0f;
				pos.y += distanceLineToLine;
			}
			lineBackSpriteDown[i]->SetPosition(pos);
			//色をセット
			const DirectX::XMFLOAT4 backColor = { 0.9f, 0.5f, 0.5f, 0.8f };
			lineBackSpriteDown[i]->SetColor(backColor);


			//警告ラインスプライト生成
			lineSpriteDown[i].reset(Sprite::Create(SpriteTextureLoader::WarningLine));
			//座標をセット
			lineSpriteDown[i]->SetPosition(pos);
			//色をセット
			const DirectX::XMFLOAT4 lineColor = { 1, 0.3f, 0.3f, 0.8f };
			lineSpriteDown[i]->SetColor(lineColor);
		}

		//文字背景用スプライト生成
		textBackSpriteDown.reset(Sprite::Create(SpriteTextureLoader::White, { 0.5f, 0 }));
		//座標をセット
		Vector2 textBackPos = { WindowApp::window_width / 2, lineBackSpriteDown[0]->GetPosition().y + lineBackSpriteDown[0]->GetSize().y / 2 };
		textBackSpriteDown->SetPosition(textBackPos);
		//大きさをセット
		textBackSpriteDown->SetSize({ WindowApp::window_width, 98 });
		//警告なので赤くしておく(じわじわ出したいので透明にしておく)
		DirectX::XMFLOAT4 textBackColor = { 0.6f, 0.2f, 0.2f, 0.9f };
		textBackSpriteDown->SetColor(textBackColor);

		//文字スプライト生成
		textSpriteDown.reset(Sprite::Create(SpriteTextureLoader::WarningText));
		//座標をセット(文字用背景と同じ座標をセット)
		Vector2 textPos = { WindowApp::window_width / 2, textBackSpriteDown->GetPosition().y + textBackSpriteDown->GetSize().y / 2 };
		textSpriteDown->SetPosition(textPos);
		//警告なので赤くしておく
		textSpriteDown->SetColor(warningTextColorMax);
	}

	return true;
}

void BossWarning::Update()
{
	//警告
	Warning();

	//画面赤スプライト更新
	screenRedSprite->Update();
	//文字背景用スプライト更新
	textBackSprite->Update();
	//文字スプライト更新
	textSprite->Update();
	for (int i = 0; i < 2; i++) {
		//警告ライン用背景スプライト更新
		lineBackSprite[i]->Update();
		//警告ラインスプライト更新
		lineSprite[i]->Update();
	}

	//文字背景用スプライト更新
	textBackSpriteDown->Update();
	//文字スプライト更新
	textSpriteDown->Update();
	for (int i = 0; i < 2; i++) {
		//警告ライン用背景スプライト更新
		lineBackSpriteDown[i]->Update();
		//警告ラインスプライト更新
		lineSpriteDown[i]->Update();
	}
}

void BossWarning::Draw()
{
	//画面赤スプライト描画
	screenRedSprite->Draw();
	//文字背景用スプライト描画
	textBackSprite->Draw();
	//文字スプライト描画
	textSprite->Draw();
	for (int i = 0; i < 2; i++) {
		//警告ライン用背景スプライト更新
		lineBackSprite[i]->Draw();
		//警告ラインスプライト更新
		lineSprite[i]->Draw();
	}


	//文字背景用スプライト描画
	textBackSpriteDown->Draw();
	//文字スプライト描画
	textSpriteDown->Draw();
	for (int i = 0; i < 2; i++) {
		//警告ライン用背景スプライト更新
		lineBackSpriteDown[i]->Draw();
		//警告ラインスプライト更新
		lineSpriteDown[i]->Draw();
	}
}

void BossWarning::Warning()
{
	//警告タイマー更新
	warningTimer++;

	//警告画面赤開始
	WarningStart(warningTimer);

	//テキストの色変更
	TextColorChange();

	//警告画面赤終了
	WarningEnd(warningTimer);

	//警告ラインスプライトを動かす(LeftTopをずらす)
	for (int i = 0; i < 2; i++) {
		Vector2 texLeftTop = lineSprite[i]->GetTexLeftTop();
		Vector2 downTexLeftTop = lineSpriteDown[i]->GetTexLeftTop();
		const float moveSpeed = 3.0f;
		if (i == 0) { texLeftTop.x += moveSpeed; downTexLeftTop.x += moveSpeed; }
		else if (i == 1) { texLeftTop.x -= moveSpeed;  downTexLeftTop.x -= moveSpeed; }
		lineSprite[i]->SetTexLeftTop(texLeftTop);
		lineSpriteDown[i]->SetTexLeftTop(downTexLeftTop);
	}

	//タイマーが指定した時間になったら
	if (warningTimer >= warningTime) {
		//削除状態にする
		isDelete = true;
	}
}

void BossWarning::WarningStart(int32_t warningTimer)
{
	//警告開始にかかる時間
	const int warningStartTime = 60;
	//タイマーが時間を越えていたら抜ける
	if (warningTimer >= warningStartTime) { return; }

	//イージング用時間計測
	const float time = (float)warningTimer / warningStartTime;

	//イージングで画面を赤くしていく(実際には色を濃くする)
	DirectX::XMFLOAT4 color = screenRedSprite->GetColor();
	color.w = Easing::LerpFloat(0, warningColorAlpha, time);
	screenRedSprite->SetColor(color);
}

void BossWarning::WarningEnd(int32_t warningTimer)
{
	//元に戻すのにかかる時間
	const int warningEndTime = 60;
	//警告終了に合わせるので終了を始める時間を計算
	const int screenColorReturnStartTime = warningTime - warningEndTime;

	//タイマーが警告終了に合わせるので終了を始める時間以下なら抜ける
	if (warningTimer < screenColorReturnStartTime) { return; }

	//イージング用時間計測
	const float time = (float)(warningTimer - screenColorReturnStartTime) / warningEndTime;

	//イージングで画面赤を戻していく(実際には色を薄くする)
	DirectX::XMFLOAT4 color = screenRedSprite->GetColor();
	color.w = Easing::LerpFloat(warningColorAlpha, 0, time);
	screenRedSprite->SetColor(color);
}

void BossWarning::TextColorChange()
{
	//色を変更する1ループの時間
	const float colorChangeTime = 30;
	//タイマー更新
	textColorChangeTimer++;
	const float time = textColorChangeTimer / colorChangeTime;

	DirectX::XMFLOAT4 color = { 0, 0, 0, 1 };

	//濃くする
	if (isTextColorDeep) {
		color.x = Easing::LerpFloat(warningTextColorMin.x, warningTextColorMax.x, time);
		color.y = Easing::LerpFloat(warningTextColorMin.y, warningTextColorMax.y, time);
		color.z = Easing::LerpFloat(warningTextColorMin.z, warningTextColorMax.z, time);
	}
	//薄くする
	else {
		color.x = Easing::LerpFloat(warningTextColorMax.x, warningTextColorMin.x, time);
		color.y = Easing::LerpFloat(warningTextColorMax.y, warningTextColorMin.y, time);
		color.z = Easing::LerpFloat(warningTextColorMax.z, warningTextColorMin.z, time);
	}
	textSprite->SetColor(color);
	textSpriteDown->SetColor(color);

	//タイマーが指定した時間になったら
	if (textColorChangeTimer >= colorChangeTime) {
		//タイマー初期化
		textColorChangeTimer = 0;

		//色変更状態を逆にする
		if (isTextColorDeep) { isTextColorDeep = false; }
		else { isTextColorDeep = true; }
	}
}
