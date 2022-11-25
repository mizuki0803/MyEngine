#include "PlayerSpeedChangeUI.h"
#include "SpriteTextureLoader.h"

PlayerSpeedChangeUI* PlayerSpeedChangeUI::Create(const Vector2& position, const float maxGaugeNum)
{
	//プレイヤー速度変更UIのインスタンスを生成
	PlayerSpeedChangeUI* playerSpeedChangeUI = new PlayerSpeedChangeUI();
	if (playerSpeedChangeUI == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!playerSpeedChangeUI->Initialize(position, maxGaugeNum)) {
		delete playerSpeedChangeUI;
		assert(0);
		return nullptr;
	}

	return playerSpeedChangeUI;
}

bool PlayerSpeedChangeUI::Initialize(const Vector2& position, const float maxGaugeNum)
{
	//速度変更UIバー生成
	const Vector2 barPosition = position;
	speedChangeBar.reset(PlayerSpeedChangeBar::Create(SpriteTextureLoader::PlayerHPGaugeIn, barPosition, maxGaugeNum));
	speedChangeBar->SetColor({ 1, 1, 0, 1 });
	//速度変更UIバーフレーム生成
	const float posDiff = 3.0f;	//HPバーの座標との差分
	const Vector2 hpFramePosition = { barPosition.x - posDiff, barPosition.y - posDiff };
	speedChangeFrame.reset(Sprite::Create(SpriteTextureLoader::PlayerHPGaugeOut, { 0, 0 }));
	speedChangeFrame->SetPosition(hpFramePosition);

	return true;
}

void PlayerSpeedChangeUI::Update()
{
	//バー更新
	speedChangeBar->Update();
	//フレーム更新
	speedChangeFrame->Update();
}

void PlayerSpeedChangeUI::Draw()
{
	//フレーム描画
	speedChangeFrame->Draw();
	//バー描画
	speedChangeBar->Draw();
}

void PlayerSpeedChangeUI::ChangeLength(const float gaugeNum)
{
	//バーの長さを変更する
	speedChangeBar->ChangeLength(gaugeNum);
}
