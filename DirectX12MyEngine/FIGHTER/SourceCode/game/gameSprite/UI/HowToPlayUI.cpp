#include "HowToPlayUI.h"
#include "SpriteTextureLoader.h"

HowToPlayUI* HowToPlayUI::Create()
{
	//遊び方UIのインスタンスを生成
	HowToPlayUI* howToPlayUI = new HowToPlayUI();
	if (howToPlayUI == nullptr) {
		return nullptr;
	}

	return howToPlayUI;
}

void HowToPlayUI::Update()
{
	//死亡したスプライトの削除
	howToPlaySprites.remove_if([](std::unique_ptr<HowToPlaySprite>& sprite) {
		return sprite->GetIsDead();
		});

	//スプライト更新
	for (const std::unique_ptr<HowToPlaySprite>& sprite : howToPlaySprites) {
		sprite->Update();
	}
}

void HowToPlayUI::Draw()
{
	//スプライト描画
	for (const std::unique_ptr<HowToPlaySprite>& sprite : howToPlaySprites) {
		sprite->Draw();
	}
}

void HowToPlayUI::CreateUI(DrawUI drawUI, DrawUI nextDrawUI)
{
	//「ショット」UI生成
	std::unique_ptr<HowToPlaySprite> newHowToPlaySprite;
	const Vector2 texSize = { 740, 100 };
	const Vector2 size = texSize * 0.8f;
	const Vector2 leftTop = { 0, texSize.y * (float)drawUI };
	newHowToPlaySprite.reset(HowToPlaySprite::Create(SpriteTextureLoader::HowToPlay, size, texSize, leftTop));
	howToPlaySprites.push_back(std::move(newHowToPlaySprite));

	//次に描画するUIを設定
	nextDrawPhase = nextDrawUI;
}
