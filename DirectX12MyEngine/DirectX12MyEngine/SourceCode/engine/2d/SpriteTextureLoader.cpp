#include "SpriteTextureLoader.h"
#include "SpriteCommon.h"

void SpriteTextureLoader::TextureLoad()
{
	//�X�v���C�g���ʕ����̃C���X�^���X���擾
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();

	//�X�v���C�g�p�e�N�X�`���ǂݍ���
	spriteCommon->LoadTexture(TextureName::DebugFont, "debugFont.png");
	spriteCommon->LoadTexture(TextureName::White, "white1x1.png");
	spriteCommon->LoadTexture(TextureName::Number, "number.png");
	spriteCommon->LoadTexture(TextureName::TitleLogo, "titleLogo.png");
	spriteCommon->LoadTexture(TextureName::BButton, "bButton.png");
	spriteCommon->LoadTexture(TextureName::HowToPlayCharge, "chargeHowToPlay.png");
	spriteCommon->LoadTexture(TextureName::Reticle, "reticle.png");
	spriteCommon->LoadTexture(TextureName::PlayerHPGaugeIn, "HPGaugeIn.png");
	spriteCommon->LoadTexture(TextureName::PlayerHPGaugeOut, "HPGaugeOut.png");
	spriteCommon->LoadTexture(TextureName::BossHPGaugeIn, "bossHPGaugeIn.png");
	spriteCommon->LoadTexture(TextureName::BossHPGaugeOut, "bossHPGaugeOut.png");
	spriteCommon->LoadTexture(TextureName::HitPlusText, "hitPlus.png");
	spriteCommon->LoadTexture(TextureName::HitPlusNumber, "hitPlusNumber.png");
	spriteCommon->LoadTexture(TextureName::StageClearText, "stageClearText.png");
	spriteCommon->LoadTexture(TextureName::DefeatNumText, "defeatNumText.png");
}
