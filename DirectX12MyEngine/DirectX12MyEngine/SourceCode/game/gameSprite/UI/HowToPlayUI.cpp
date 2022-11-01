#include "HowToPlayUI.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"

void (HowToPlayUI::* HowToPlayUI::actionFuncTable[])() = {
	&HowToPlayUI::TextUp,
	&HowToPlayUI::TextStay,
	&HowToPlayUI::TextDown,
};

const Vector2 HowToPlayUI::stayPos = { 640, 600 };
const Vector2 HowToPlayUI::outScreenPos = { 640, 900 };

HowToPlayUI* HowToPlayUI::Create()
{
	//複数撃破UIのインスタンスを生成
	HowToPlayUI* howToPlayUI = new HowToPlayUI();
	if (howToPlayUI == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!howToPlayUI->Initialize()) {
		delete howToPlayUI;
		assert(0);
		return nullptr;
	}

	return howToPlayUI;
}

bool HowToPlayUI::Initialize()
{
	//枠スプライト生成
	chargeHowToPlaySprite.reset(Sprite::Create(SpriteTextureLoader::HowToPlayCharge));
	chargeHowToPlaySprite->SetPosition(outScreenPos);
	const Vector2 texSize = { 740, 100 };
	const Vector2 size = texSize * 0.8f;
	chargeHowToPlaySprite->SetSize(size);
	chargeHowToPlaySprite->SetTexSize(texSize);

	return true;
}

void HowToPlayUI::Update()
{
	//スプライト更新
	chargeHowToPlaySprite->Update();
}

void HowToPlayUI::Draw()
{
	//行動
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//スプライト描画
	chargeHowToPlaySprite->Draw();
}

void HowToPlayUI::TextUp()
{
	//タイマー更新
	const float upTime = 20;
	actionTimer++;
	const float time = actionTimer / upTime;

	//イージングで上に動かす
	Vector2 pos = chargeHowToPlaySprite->GetPosition();
	pos.y = Easing::OutCubic(outScreenPos.y, stayPos.y, time);
	chargeHowToPlaySprite->SetPosition(pos);

	//タイマーが指定した時間になったら
	if (actionTimer >= upTime) {
		//次のフェーズへ
		phase = ActionPhase::Stay;
		
		//タイマー初期化
		actionTimer = 0;
	}
}

void HowToPlayUI::TextStay()
{
	//タイマー更新
	const float stayTime = 700;
	actionTimer++;

	//タイマーが指定した時間になったら
	if (actionTimer >= stayTime) {
		//次のフェーズへ
		phase = ActionPhase::Down;

		//タイマー初期化
		actionTimer = 0;
	}
}

void HowToPlayUI::TextDown()
{
	//タイマー更新
	const float downTime = 20;
	actionTimer++;
	const float time = actionTimer / downTime;

	//イージングで上に動かす
	Vector2 pos = chargeHowToPlaySprite->GetPosition();
	pos.y = Easing::InCubic(stayPos.y, outScreenPos.y, time);
	chargeHowToPlaySprite->SetPosition(pos);

	//タイマーが指定した時間になったら
	if (actionTimer >= downTime) {
		//死亡フラグを立てる
		isDead = true;
	}
}
