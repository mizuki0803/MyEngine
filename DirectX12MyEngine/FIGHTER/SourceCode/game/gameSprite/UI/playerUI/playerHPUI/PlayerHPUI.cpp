#include "PlayerHPUI.h"
#include "SpriteTextureLoader.h"

PlayerHPUI* PlayerHPUI::Create(const Vector2& position, const int startHP, const int maxHP)
{
	//HPUIのインスタンスを生成
	PlayerHPUI* playerHPUI = new PlayerHPUI();
	if (playerHPUI == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!playerHPUI->Initialize(position, startHP, maxHP)) {
		delete playerHPUI;
		assert(0);
		return nullptr;
	}

	return playerHPUI;
}

bool PlayerHPUI::Initialize(const Vector2& position, const int startHP, const int maxHP)
{
	//HPバー生成
	const Vector2 hpBarPosition = position;
	hpBar.reset(PlayerHPBar::Create(SpriteTextureLoader::PlayerHPGaugeIn, hpBarPosition, startHP, maxHP));
	//HPバーフレーム生成
	const float posDiff = 3.0f;	//HPバーの座標との差分
	const Vector2 hpFramePosition = { hpBarPosition.x - posDiff, hpBarPosition.y - posDiff };
	hpFrame.reset(PlayerHPFrame::Create(SpriteTextureLoader::PlayerHPGaugeOut, hpFramePosition));

    return true;
}

void PlayerHPUI::Update()
{
	//HPバー更新
	hpBar->Update();
	//HPフレーム更新
	hpFrame->Update();
}

void PlayerHPUI::Draw()
{
	//HPフレーム描画
	hpFrame->Draw();
	//HPバー描画
	hpBar->Draw();
}

void PlayerHPUI::Damage(const int HP)
{
	//HPバーの長さを変更する
	hpBar->SetChangeLength(HP);
}

void PlayerHPUI::ItemGet(const int HP)
{
	//HPバーの長さを変更する
	hpBar->SetChangeLength(HP);
	//HPバーフレームを色をチカチカさせる
	hpFrame->ItemGetModeStart();
}
