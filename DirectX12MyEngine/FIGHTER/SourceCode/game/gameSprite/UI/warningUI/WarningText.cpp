#include "WarningText.h"
#include "Easing.h"

const DirectX::XMFLOAT4 WarningText::warningTextColorMax = { 0.9f, 0.01f, 0.07f, 1.0f };
const DirectX::XMFLOAT4 WarningText::warningTextColorMin = { 0.7f, 0.15f, 0.1f, 1.0f };

WarningText* WarningText::Create(UINT texNumber, const Vector2& position)
{
	//警告テキストのインスタンスを生成
	WarningText* warningText = new WarningText();
	if (warningText == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!warningText->Initialize(texNumber, position)) {
		delete warningText;
		assert(0);
		return nullptr;
	}

	return warningText;
}

bool WarningText::Initialize(UINT texNumber, const Vector2& position)
{
	//スプライト初期化
	if (!Sprite::Initialize(texNumber, { 0.5f, 0.5f }, false, false)) {
		return false;
	}

	//座標をセット
	this->position = position;
	//表示用の大きさをセット
	this->showSize = size;
	//色をセット(警告なので赤くしておく)
	color = warningTextColorMax;

	//だんだん出したいので大きさYを0にしておく
	size.y = 0;

	return true;
}

void WarningText::WarningStart(float easeTimer)
{
	//大きくしていく
	size.y = Easing::LerpFloat(0, showSize.y, easeTimer);
}

void WarningText::WarningEnd(float easeTimer)
{
	//小さくしていく
	size.y = Easing::LerpFloat(showSize.y, 0, easeTimer);
}

void WarningText::TextColorChange()
{
	//色を変更する1ループの時間
	const float colorChangeTime = 30;
	//タイマー更新
	textColorChangeTimer++;
	const float time = textColorChangeTimer / colorChangeTime;

	//濃くする
	if (isTextColorDeep) {
		color.x = Easing::LerpFloat(warningTextColorMin.x, warningTextColorMax.x, time);
		color.y = Easing::LerpFloat(warningTextColorMin.y, warningTextColorMax.y, time);
		color.z = Easing::LerpFloat(warningTextColorMin.z, warningTextColorMax.z, time);
	}
	//薄くする
	else {
		color.x = Easing::LerpFloat(warningTextColorMax.x, warningTextColorMin.x, time);
		color.y = Easing::LerpFloat(warningTextColorMax.y, warningTextColorMin.y, time);
		color.z = Easing::LerpFloat(warningTextColorMax.z, warningTextColorMin.z, time);
	}

	//タイマーが指定した時間になったら
	if (textColorChangeTimer >= colorChangeTime) {
		//タイマー初期化
		textColorChangeTimer = 0;

		//色変更状態を逆にする
		if (isTextColorDeep) { isTextColorDeep = false; }
		else { isTextColorDeep = true; }
	}
}
