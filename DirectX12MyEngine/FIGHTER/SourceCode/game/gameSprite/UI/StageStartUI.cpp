#include "StageStartUI.h"
#include "SpriteTextureLoader.h"

StageStartUI* StageStartUI::Create(const int stageNum)
{
	//�X�e�[�W�J�nUI�̃C���X�^���X�𐶐�
	StageStartUI* stageStartUI = new StageStartUI();
	if (stageStartUI == nullptr) {
		return nullptr;
	}

	// ������
	if (!stageStartUI->Initialize(stageNum)) {
		delete stageStartUI;
		assert(0);
		return nullptr;
	}

	return stageStartUI;
}

bool StageStartUI::Initialize(const int stageNum)
{
	//�g�X�v���C�g����
	stageInfoSprite.reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::White)));
	stageInfoSprite->SetPosition({ 640, 360 });
	const Vector2 size = { 500, 200 };
	const Vector2 texSize = { 1, 1 };
	stageInfoSprite->SetSize(size);
	stageInfoSprite->SetTexSize(texSize);
	stageInfoSprite->SetColor({0.2f, 0.9f, 0.2f, 0.4f});

	return true;
}

void StageStartUI::Update()
{
	//�����^�C�}�[�X�V
	deadTimer++;
	//�^�C�}�[���B�����玀�S
	const int32_t lifeTime = 200;
	if (deadTimer >= lifeTime) {
		isDead = true;
	}

	//�X�v���C�g�X�V
	stageInfoSprite->Update();
}

void StageStartUI::Draw()
{
	//�X�v���C�g�`��
	stageInfoSprite->Draw();
}
