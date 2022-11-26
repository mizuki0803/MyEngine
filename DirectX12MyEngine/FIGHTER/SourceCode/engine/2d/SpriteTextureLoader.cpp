#include "SpriteTextureLoader.h"
#include "SpriteCommon.h"

void SpriteTextureLoader::TextureLoad()
{
	//スプライト共通部分のインスタンスを取得
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	//スプライト用テクスチャ読み込み
	spriteCommon->LoadTexture(TextureName::DebugFont, "debugFont.png");
	spriteCommon->LoadTexture(TextureName::White, "white1x1.png");
	spriteCommon->LoadTexture(TextureName::Number, "number.png");
	spriteCommon->LoadTexture(TextureName::TitleLogo, "titleLogo.png");
	spriteCommon->LoadTexture(TextureName::BButton, "bButton.png");
	spriteCommon->LoadTexture(TextureName::PlanetName, "planetName.png");
	spriteCommon->LoadTexture(TextureName::HighScoreFrame, "highScoreFrame.png");
	spriteCommon->LoadTexture(TextureName::StageSelectHowToPlay, "stageSelectHowToPlay.png");
	spriteCommon->LoadTexture(TextureName::StageDecision, "stageDecision.png");
	spriteCommon->LoadTexture(TextureName::HowToPlay, "howToPlay.png");
	spriteCommon->LoadTexture(TextureName::Reticle, "reticle.png");
	spriteCommon->LoadTexture(TextureName::PlayerHPGaugeIn, "HPGaugeIn.png");
	spriteCommon->LoadTexture(TextureName::PlayerHPGaugeOut, "HPGaugeOut.png");
	spriteCommon->LoadTexture(TextureName::BossHPLogo, "bossHPGaugeLogo.png");
	spriteCommon->LoadTexture(TextureName::BossHPGaugeIn, "bossHPGaugeIn.png");
	spriteCommon->LoadTexture(TextureName::BossHPGaugeOut, "bossHPGaugeOut.png");
	spriteCommon->LoadTexture(TextureName::BossName, "bossName.png");
	spriteCommon->LoadTexture(TextureName::BossNickname, "bossNickname.png");
	spriteCommon->LoadTexture(TextureName::HitPlusText, "hitPlus.png");
	spriteCommon->LoadTexture(TextureName::HitPlusNumber, "hitPlusNumber.png");
	spriteCommon->LoadTexture(TextureName::StageClearText, "stageClearText.png");
	spriteCommon->LoadTexture(TextureName::DefeatNumText, "defeatNumText.png");
}
