#include "HowToPlayUI.h"
#include "SpriteTextureLoader.h"

HowToPlayUI* HowToPlayUI::Create()
{
	//�V�ѕ�UI�̃C���X�^���X�𐶐�
	HowToPlayUI* howToPlayUI = new HowToPlayUI();
	if (howToPlayUI == nullptr) {
		return nullptr;
	}

	return howToPlayUI;
}

void HowToPlayUI::Update()
{
	//���S�����X�v���C�g�̍폜
	howToPlaySprites.remove_if([](std::unique_ptr<HowToPlaySprite>& sprite) {
		return sprite->GetIsDead();
		});

	//�X�v���C�g�X�V
	for (const std::unique_ptr<HowToPlaySprite>& sprite : howToPlaySprites) {
		sprite->Update();
	}
}

void HowToPlayUI::Draw()
{
	//�X�v���C�g�`��
	for (const std::unique_ptr<HowToPlaySprite>& sprite : howToPlaySprites) {
		sprite->Draw();
	}
}

void HowToPlayUI::CreateUI(DrawUI drawUI, DrawUI nextDrawUI)
{
	//�u�V���b�g�vUI����
	std::unique_ptr<HowToPlaySprite> newHowToPlaySprite;
	const Vector2 texSize = { 740, 100 };
	const Vector2 size = texSize * 0.8f;
	const Vector2 leftTop = { 0, texSize.y * (float)drawUI };
	newHowToPlaySprite.reset(HowToPlaySprite::Create(SpriteTextureLoader::HowToPlay, size, texSize, leftTop));
	howToPlaySprites.push_back(std::move(newHowToPlaySprite));

	//���ɕ`�悷��UI��ݒ�
	nextDrawPhase = nextDrawUI;
}
