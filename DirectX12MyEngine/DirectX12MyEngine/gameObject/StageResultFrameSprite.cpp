#include "StageResultFrameSprite.h"

StageResultFrameSprite* StageResultFrameSprite::Create(UINT texNumber, const Vector2& position, const Vector2& size, const Vector2& texSize)
{
	//ステージリザルトの枠スプライトのインスタンスを生成
	StageResultFrameSprite* stageResultFrameSprite = new StageResultFrameSprite();
	if (stageResultFrameSprite == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!stageResultFrameSprite->Initialize(texNumber, { 0.5f, 0.5f }, false, false)) {
		delete stageResultFrameSprite;
		assert(0);
		return nullptr;
	}

	//座標をセット
	stageResultFrameSprite->position = position;

	//大きさをセット
	stageResultFrameSprite->size = size;
	stageResultFrameSprite->texSize = texSize;

	return stageResultFrameSprite;
}
