#include "SpriteTextureLoader.h"
#include "TextureManager.h"

Texture SpriteTextureLoader::textures[SpriteTextureLoader::SpriteTextureNum];

void SpriteTextureLoader::TextureLoad()
{
	//スプライトテクスチャ用ディレクトリパス
	const std::string directoryPath = "Resources/spriteTexture/";

	//スプライト用テクスチャ読み込み
	TextureManager::LoadTexture(textures[DebugFont], directoryPath, "debugFont.png");
	TextureManager::LoadTexture(textures[White], directoryPath, "white1x1.png");
	TextureManager::LoadTexture(textures[Number], directoryPath, "number.png");
	TextureManager::LoadTexture(textures[TitleLogo], directoryPath, "titleLogo.png");
	TextureManager::LoadTexture(textures[BButton], directoryPath, "bButton.png");
	TextureManager::LoadTexture(textures[PlanetName], directoryPath, "planetName.png");
	TextureManager::LoadTexture(textures[HighScoreFrame], directoryPath, "highScoreFrame.png");
	TextureManager::LoadTexture(textures[RankMedal], directoryPath, "rankMedal.png");
	TextureManager::LoadTexture(textures[StageSelectHowToPlay], directoryPath, "stageSelectHowToPlay.png");
	TextureManager::LoadTexture(textures[StageDecision], directoryPath, "stageDecision.png");
	TextureManager::LoadTexture(textures[HowToPlay], directoryPath, "howToPlay.png");
	TextureManager::LoadTexture(textures[Reticle], directoryPath, "reticle.png");
	TextureManager::LoadTexture(textures[PlayerHPGaugeIn], directoryPath, "HPGaugeIn.png");
	TextureManager::LoadTexture(textures[PlayerHPGaugeOut], directoryPath, "HPGaugeOut.png");
	TextureManager::LoadTexture(textures[SpeedChangeGaugeIn], directoryPath, "speedChangeGaugeIn.png");
	TextureManager::LoadTexture(textures[SpeedChangeGaugeOut], directoryPath, "speedChangeGaugeOut.png");
	TextureManager::LoadTexture(textures[WarningText], directoryPath, "warningText.png");
	TextureManager::LoadTexture(textures[WarningLine], directoryPath, "warningLine.png");
	TextureManager::LoadTexture(textures[BossHPLogo], directoryPath, "bossHPGaugeLogo.png");
	TextureManager::LoadTexture(textures[BossHPGaugeIn], directoryPath, "bossHPGaugeIn.png");
	TextureManager::LoadTexture(textures[BossHPGaugeOut], directoryPath, "bossHPGaugeOut.png");
	TextureManager::LoadTexture(textures[BossName], directoryPath, "bossName.png");
	TextureManager::LoadTexture(textures[BossNickname], directoryPath, "bossNickname.png");
	TextureManager::LoadTexture(textures[HitPlusText], directoryPath, "hitPlus.png");
	TextureManager::LoadTexture(textures[HitPlusNumber], directoryPath, "hitPlusNumber.png");
	TextureManager::LoadTexture(textures[StageClearText], directoryPath, "stageClearText.png");
	TextureManager::LoadTexture(textures[DefeatNumText], directoryPath, "defeatNumText.png");
}
