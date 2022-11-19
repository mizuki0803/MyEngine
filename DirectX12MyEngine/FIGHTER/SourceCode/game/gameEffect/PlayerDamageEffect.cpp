#include "PlayerDamageEffect.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"
#include "WindowApp.h"

PlayerDamageEffect* PlayerDamageEffect::Create()
{
	//自機ダメージ演出のインスタンスを生成
	PlayerDamageEffect* playerDamageEffect = new PlayerDamageEffect();
	if (playerDamageEffect == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!playerDamageEffect->Initialize()) {
		delete playerDamageEffect;
		assert(0);
		return nullptr;
	}

	return playerDamageEffect;
}

bool PlayerDamageEffect::Initialize()
{
	//演出用スプライト生成
	effectSprite.reset(Sprite::Create(SpriteTextureLoader::White, { 0, 0 }));
	effectSprite->SetSize({ WindowApp::window_width, WindowApp::window_height });
	effectSprite->SetTexSize({ 1, 1 });
	effectSprite->SetColor({ 1, 0, 0, 0 });

    return true;
}

void PlayerDamageEffect::Update()
{
	//ダメージ演出
	DamageEffect();

	//演出用スプライト更新
	effectSprite->Update();
}

void PlayerDamageEffect::Draw()
{
	//描画状態でなければ抜ける
	if (!isDraw) { return; }

	//演出用スプライト描画
	effectSprite->Draw();
}

void PlayerDamageEffect::DamageEffectStart(const int maxHP, const int HP)
{
	//現在のHPの割合を算出
	const float ratioHP = (float)HP / (float)maxHP;
	//割合によって変化する色の濃さの範囲
	const float ratioDeepRange = 0.4f;
	//ベースとなる色の濃さ(この数字に割合で算出した濃さを加算する)
	const float baseDeepNum = 0.15f;
	//色の濃さをセット(残りHPが少ないほど色が濃くなる)
	const float deepNum = ratioDeepRange * (1 - ratioHP) + baseDeepNum;
	colorDeepNum = deepNum;

	//色をセット
	DirectX::XMFLOAT4 color = effectSprite->GetColor();
	color.w = colorDeepNum;
	effectSprite->SetColor(color);

	//タイマーを初期化
	drawTimer = 0;

	//描画状態にする
	isDraw = true;
}

void PlayerDamageEffect::DamageEffect()
{
	//描画状態でなければ抜ける
	if (!isDraw) { return; }

	//色を薄くする時間
	const float returnTime = 50;
	//タイマー更新
	drawTimer++;
	const float time = drawTimer / returnTime;

	//色を薄くしていく
	DirectX::XMFLOAT4 color = effectSprite->GetColor();
	color.w = Easing::LerpFloat(colorDeepNum, 0, time);
	effectSprite->SetColor(color);

	//タイマーが指定した時間になったら
	if (drawTimer >= returnTime) {
		//描画終了
		isDraw = false;
	}
}
