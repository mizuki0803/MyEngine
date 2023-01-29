#include "HowToPlaySprite.h"
#include "Easing.h"

void (HowToPlaySprite::* HowToPlaySprite::actionFuncTable[])() = {
	&HowToPlaySprite::TextUp,
	&HowToPlaySprite::TextStay,
	&HowToPlaySprite::TextDown,
};

const Vector2 HowToPlaySprite::stayPos = { 640, 600 };
const Vector2 HowToPlaySprite::outScreenPos = { 640, 900 };

HowToPlaySprite* HowToPlaySprite::Create(const Texture& texture, const Vector2& size, const Vector2& texSize, const Vector2& leftTop)
{
	//複数撃破UIのインスタンスを生成
	HowToPlaySprite* howToPlaySprite = new HowToPlaySprite();
	if (howToPlaySprite == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!howToPlaySprite->Initialize(texture, { 0.5f, 0.5f }, false, false)) {
		delete howToPlaySprite;
		assert(0);
		return nullptr;
	}

	//座標をセット
	howToPlaySprite->position = outScreenPos;

	//大きさをセット
	howToPlaySprite->size = size;
	howToPlaySprite->texSize = texSize;

	//左上座標をセット
	howToPlaySprite->texLeftTop = leftTop;

	return howToPlaySprite;
}

void HowToPlaySprite::Update()
{
	//行動
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//スプライト更新
	Sprite::Update();
}

void HowToPlaySprite::TextUp()
{
	//タイマー更新
	const float upTime = 20;
	actionTimer++;
	const float time = actionTimer / upTime;

	//イージングで上に動かす
	position.y = Easing::OutCubic(outScreenPos.y, stayPos.y, time);

	//タイマーが指定した時間になったら
	if (actionTimer >= upTime) {
		//次のフェーズへ
		phase = ActionPhase::Stay;

		//タイマー初期化
		actionTimer = 0;
	}
}

void HowToPlaySprite::TextStay()
{
	//タイマー更新
	const float stayTime = 500;
	actionTimer++;

	//タイマーが指定した時間になったら
	if (actionTimer >= stayTime) {
		//次のフェーズへ
		phase = ActionPhase::Down;

		//タイマー初期化
		actionTimer = 0;
	}
}

void HowToPlaySprite::TextDown()
{
	//タイマー更新
	const float downTime = 20;
	actionTimer++;
	const float time = actionTimer / downTime;

	//イージングで上に動かす
	position.y = Easing::InCubic(stayPos.y, outScreenPos.y, time);

	//タイマーが指定した時間になったら
	if (actionTimer >= downTime) {
		//死亡フラグを立てる
		isDead = true;
	}
}
