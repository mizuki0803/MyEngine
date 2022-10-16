#include "SpriteTexture.h"
#include "SpriteCommon.h"

void SpriteTexture::TextureLoad()
{
	//スプライト共通部分のインスタンスを取得
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	//スプライト用テクスチャ読み込み
	spriteCommon->LoadTexture(TextureName::DebugFont, "debugFont.png");
	spriteCommon->LoadTexture(TextureName::Number, "number.png");
	spriteCommon->LoadTexture(TextureName::Reticle, "reticle.png");
	spriteCommon->LoadTexture(TextureName::PlayerHPGaugeIn, "HPGaugeIn.png");
	spriteCommon->LoadTexture(TextureName::PlayerHPGaugeOut, "HPGaugeOut.png");
	spriteCommon->LoadTexture(TextureName::BossHPGaugeIn, "bossHPGaugeIn.png");
	spriteCommon->LoadTexture(TextureName::BossHPGaugeOut, "bossHPGaugeOut.png");
}
