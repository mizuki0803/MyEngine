#include "WarningLineBack.h"
#include "Easing.h"

WarningLineBack* WarningLineBack::Create(UINT texNumber, const Vector2& showPos, const Vector2 showSize)
{
	//警告ライン背景のインスタンスを生成
	WarningLineBack* warningLineBack = new WarningLineBack();
	if (warningLineBack == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!warningLineBack->Initialize(texNumber, showPos, showSize)) {
		delete warningLineBack;
		assert(0);
		return nullptr;
	}

	return warningLineBack;
}

bool WarningLineBack::Initialize(UINT texNumber, const Vector2& showPos, const Vector2 showSize)
{
	//スプライト初期化
	if (!Sprite::Initialize(texNumber, { 0.5f, 0.5f }, false, false)) {
		return false;
	}

	//表示座標をセット
	this->showPos = showPos;
	//表示用の大きさをセット
	size = showSize;
	this->showSize = size;
	//色をセット
	const DirectX::XMFLOAT4 backColor = { 0.9f, 0.5f, 0.5f, 0.8f };
	color = backColor;

	//だんだん出したいので大きさYを0にしておく
	size.y = 0;

	return true;
}

void WarningLineBack::StartSet(const Vector2& createPos)
{
	//生成座標をセット
	this->createPos = createPos;
	//座標を生成座標にセット
	position = createPos;
}

void WarningLineBack::WarningStart(float easeTimer)
{
	//生成座標から移動して大きくしていく
	position.y = Easing::LerpFloat(createPos.y, showPos.y, easeTimer);
	size.y = Easing::LerpFloat(0, showSize.y, easeTimer);
}

void WarningLineBack::WarningEnd(float easeTimer)
{
	//小さくしながら生成座標に戻していく
	position.y = Easing::LerpFloat(showPos.y, createPos.y, easeTimer);
	size.y = Easing::LerpFloat(showSize.y, 0, easeTimer);
}
