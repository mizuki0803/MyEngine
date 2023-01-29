#include "BossNameUI.h"
#include "SpriteTextureLoader.h"

BossNameUI* BossNameUI::Create(int bossType)
{
	//�{�X���\��UI�̃C���X�^���X�𐶐�
	BossNameUI* bossNameUI = new BossNameUI();
	if (bossNameUI == nullptr) {
		return nullptr;
	}

	// ������
	if (!bossNameUI->Initialize(bossType)) {
		delete bossNameUI;
		assert(0);
		return nullptr;
	}

	return bossNameUI;
}

bool BossNameUI::Initialize(int bossType)
{
	//�{�X�̎�ނ��Z�b�g
	this->bossType = bossType;

	//����X�v���C�g����
	sprites[0].reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::BossNickname), { 0, 0 }));
	const Vector2 pos = { 80, 520 };
	sprites[0]->SetPosition(pos);
	const Vector2 texSize = { 300, 40 };
	sprites[0]->SetTexSize(texSize);
	sprites[0]->SetSize(texSize);
	sprites[0]->SetTexLeftTop({ texSize.x * bossType, 0 });

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
	const int nameSpriteCreateTime = 65;
	if (drawTimer >= nameSpriteCreateTime && !sprites[1]) {
		//�{�X���X�v���C�g����
		sprites[1].reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::BossName), { 0, 0 }));
		const Vector2 pos = { 70, 580 };
		sprites[1]->SetPosition(pos);
		const Vector2 texSize = { 800, 95 };
		sprites[1]->SetTexSize(texSize);
		sprites[1]->SetSize(texSize);
		sprites[1]->SetTexLeftTop({ texSize.x * bossType, 0 });
	}

	//����Ɏw�肵�����ԂɂȂ�����
	const int lifeTime = 300;
	if (drawTimer >= lifeTime) {
		//�\�����I����
		isDraw = false;
	}
}
