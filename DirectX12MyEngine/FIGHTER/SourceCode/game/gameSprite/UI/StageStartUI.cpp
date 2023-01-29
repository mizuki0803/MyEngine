#include "StageStartUI.h"
#include "SpriteTextureLoader.h"

StageStartUI* StageStartUI::Create(const int stageNum)
{
	//ステージ開始UIのインスタンスを生成
	StageStartUI* stageStartUI = new StageStartUI();
	if (stageStartUI == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!stageStartUI->Initialize(stageNum)) {
		delete stageStartUI;
		assert(0);
		return nullptr;
	}

	return stageStartUI;
}

bool StageStartUI::Initialize(const int stageNum)
{
	//枠スプライト生成
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
	//寿命タイマー更新
	deadTimer++;
	//タイマーが達したら死亡
	const int32_t lifeTime = 200;
	if (deadTimer >= lifeTime) {
		isDead = true;
	}

	//スプライト更新
	stageInfoSprite->Update();
}

void StageStartUI::Draw()
{
	//スプライト描画
	stageInfoSprite->Draw();
}
