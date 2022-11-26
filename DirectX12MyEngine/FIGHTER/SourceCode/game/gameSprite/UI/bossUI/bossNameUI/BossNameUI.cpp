#include "BossNameUI.h"
#include "SpriteTextureLoader.h"

BossNameUI* BossNameUI::Create()
{
	//�{�X���\��UI�̃C���X�^���X�𐶐�
	BossNameUI* bossNameUI = new BossNameUI();
	if (bossNameUI == nullptr) {
		return nullptr;
	}

	// ������
	if (!bossNameUI->Initialize()) {
		delete bossNameUI;
		assert(0);
		return nullptr;
	}

	return bossNameUI;
}

bool BossNameUI::Initialize()
{
	//����X�v���C�g����
	sprites[0].reset(Sprite::Create(SpriteTextureLoader::BButton, { 0, 0 }));
	const Vector2 pos = { 200, 500 };
	sprites[0]->SetPosition(pos);

	return true;
}

void BossNameUI::Update()
{
	//�\�������Ȃ��ꍇ�͔�����
	if (!isDraw) { return; }

	//�^�C�}�[�X�V
	CountDrawTimer();

	//�X�v���C�g�X�V
	for (int i = 0; i < spriteNum; i++) {
		//�C���X�^���X���Ȃ���Δ�΂�
		if (!sprites[i]) { continue; }
		sprites[i]->Update();
	}
}

void BossNameUI::Draw()
{
	//�\�������Ȃ��ꍇ�͔�����
	if (!isDraw) { return; }

	//�X�v���C�g�`��
	for (int i = 0; i < spriteNum; i++) {
		//�C���X�^���X���Ȃ���Δ�΂�
		if (!sprites[i]) { continue; }
		sprites[i]->Draw();
	}
}

void BossNameUI::CountDrawTimer()
{
	//�^�C�}�[�X�V
	drawTimer++;

	//�w�肵�����ԂɂȂ�����{�X����\��
	const int nameSpriteCreateTime = 90;
	if (drawTimer >= nameSpriteCreateTime && !sprites[1]) {
		//�{�X���X�v���C�g����
		sprites[1].reset(Sprite::Create(SpriteTextureLoader::BButton, { 0, 0 }));
		const Vector2 pos = { 200, 600 };
		sprites[1]->SetPosition(pos);
		sprites[1]->SetSize({ 128, 128 });
	}

	//����Ɏw�肵�����ԂɂȂ�����
	const int lifeTime = 300;
	if (drawTimer >= lifeTime) {
		//�\�����I����
		isDraw = false;
	}
}
