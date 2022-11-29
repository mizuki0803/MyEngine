#include "WarningTextBack.h"
#include "Easing.h"

WarningTextBack* WarningTextBack::Create(UINT texNumber, const Vector2& position, const Vector2 size)
{
	//警告テキスト背景のインスタンスを生成
	WarningTextBack* warningTextBack = new WarningTextBack();
	if (warningTextBack == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!warningTextBack->Initialize(texNumber, position, size)) {
		delete warningTextBack;
		assert(0);
		return nullptr;
	}

	return warningTextBack;
}

bool WarningTextBack::Initialize(UINT texNumber, const Vector2& position, const Vector2 size)
{
	//スプライト初期化
	if (!Sprite::Initialize(texNumber, { 0.5f, 0.5f }, false, false)) {
		return false;
	}

	//座標をセット
	this->position = position;
	//表示用の大きさをセット
	this->size = size;
	showSize = this->size;
	//色をセット(警告なので赤くしておく)
	DirectX::XMFLOAT4 textBackColor = { 0.6f, 0.2f, 0.2f, 0.9f };
	color = textBackColor;

	//だんだん出したいので大きさYを0にしておく
	this->size.y = 0;

	return true;
}

void WarningTextBack::WarningStart(float easeTimer)
{
	//大きくしていく
	size.y = Easing::LerpFloat(0, showSize.y, easeTimer);
}

void WarningTextBack::WarningEnd(float easeTimer)
{
	//小さくしていく
	size.y = Easing::LerpFloat(showSize.y, 0, easeTimer);
}
