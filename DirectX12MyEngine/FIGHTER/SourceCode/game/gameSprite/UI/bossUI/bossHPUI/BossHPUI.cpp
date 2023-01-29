#include "BossHPUI.h"
#include "SpriteTextureLoader.h"

void (BossHPUI::* BossHPUI::startSetActionFuncTable[])() = {
	&BossHPUI::StartSetMove,
	&BossHPUI::StartSetChangeSize,
};

BossHPUI* BossHPUI::Create(const Vector2& position, const int maxHP)
{
	//ボス用HPUIのインスタンスを生成
	BossHPUI* bossHPUI = new BossHPUI();
	if (bossHPUI == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!bossHPUI->Initialize(position, maxHP)) {
		delete bossHPUI;
		assert(0);
		return nullptr;
	}

	return bossHPUI;
}

bool BossHPUI::Initialize(const Vector2& position, const int maxHP)
{
	//HPUIロゴ生成
	const Vector2 hpLogoPosition = position;
	hpLogo.reset(BossHPLogo::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::BossHPLogo), hpLogoPosition));
	//HPバーフレーム生成
	const Vector2 posDiffLogo = { 13.0f, 38.0f };
	const Vector2 hpFramePosition = { hpLogoPosition.x + posDiffLogo.x, hpLogoPosition.y + posDiffLogo.y };
	hpFrame.reset(BossHPFrame::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::BossHPGaugeOut), hpFramePosition));
	//HPUIバー生成
	const float posDiffFrame = 3.0f;	//HP枠の座標との差分
	const Vector2 hpBarPosition = { hpFramePosition.x + posDiffFrame, hpFramePosition.y + posDiffFrame };
	hpBar.reset(BossHPBar::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::BossHPGaugeIn), hpBarPosition, maxHP));

	return true;
}

void BossHPUI::Update()
{
	//開始時セット
	if (isStartSet) {
		StartSet();
	}

	//ロゴ更新
	hpLogo->Update();
	//バー更新
	hpBar->Update();
	//フレーム更新
	hpFrame->Update();
}

void BossHPUI::Draw()
{
	//フレーム描画
	hpFrame->Draw();
	//バー描画
	hpBar->Draw();
	//ロゴ更新
	hpLogo->Draw();
}

void BossHPUI::Damage(const int HP)
{
	//バーの長さを変更する
	hpBar->SetChangeLength(HP);
}

void BossHPUI::StartSet()
{
	//開始時セット行動
	(this->*startSetActionFuncTable[static_cast<size_t>(startSetPhase)])();
}

void BossHPUI::StartSetMove()
{
	//画面内移動にかかる時間
	const int moveTime = 40;
	//タイマーを更新
	startSetTimer++;

	//移動させる
	hpLogo->StartSetMove(moveTime, startSetTimer);
	hpBar->StartSetMove(moveTime, startSetTimer);
	hpFrame->StartSetMove(moveTime, startSetTimer);

	//タイマーが指定した時間になったら
	if (startSetTimer >= moveTime) {
		//次のフェーズへ
		startSetPhase = StartSetPhase::ChangeSize;

		//タイマーを初期化
		startSetTimer = 0;
	}
}

void BossHPUI::StartSetChangeSize()
{
	//大きさ変更にかかる時間
	const int changeTime = 60;
	//タイマーを更新
	startSetTimer++;

	//移動させる
	hpBar->StartSetChangeSize(changeTime, startSetTimer);

	//フレームの大きさ変更時間
	const int frameChangeTime = 20;
	hpFrame->StartSetChangeSize(frameChangeTime, startSetTimer);

	//タイマーが指定した時間になったら
	if (startSetTimer >= changeTime) {
		//開始時セット終了
		isStartSet = false;
	}
}
