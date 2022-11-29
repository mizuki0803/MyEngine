#include "warningLine.h"
#include "Easing.h"

WarningLine* WarningLine::Create(UINT texNumber, const Vector2& showPos, const bool isMoveRight)
{
	//警告ラインのインスタンスを生成
	WarningLine* warningLine = new WarningLine();
	if (warningLine == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!warningLine->Initialize(texNumber, showPos, isMoveRight)) {
		delete warningLine;
		assert(0);
		return nullptr;
	}

	return warningLine;
}

bool WarningLine::Initialize(UINT texNumber, const Vector2& showPos, const bool isMoveRight)
{
	//スプライト初期化
	if (!Sprite::Initialize(texNumber, { 0.5f, 0.5f }, false, false)) {
		return false;
	}

	//表示座標をセット
	this->showPos = showPos;
	//表示用の大きさをセット
	this->showSize = size;
	//色をセット
	const DirectX::XMFLOAT4 lineColor = { 1, 0.3f, 0.3f, 0.8f };
	color = lineColor;

	//だんだん出したいので大きさYを0にしておく
	size.y = 0;

	//右に移動するか
	this->isMoveRight = isMoveRight;

	return true;
}

void WarningLine::StartSet(const Vector2& createPos)
{
	//生成座標をセット
	this->createPos = createPos;
	//座標を生成座標にセット
	position = createPos;
}

void WarningLine::WarningStart(float easeTimer)
{
	//生成座標から移動して大きくしていく
	position.y = Easing::LerpFloat(createPos.y, showPos.y, easeTimer);
	size.y = Easing::LerpFloat(0, showSize.y, easeTimer);

	//ラインを移動させる
	LineMove();
}

void WarningLine::WarningEnd(float easeTimer)
{
	//小さくしながら生成座標に戻していく
	position.y = Easing::LerpFloat(showPos.y, createPos.y, easeTimer);
	size.y = Easing::LerpFloat(showSize.y, 0, easeTimer);

	//ラインを移動させる
	LineMove();
}

void WarningLine::LineMove()
{
	//左上座標をずらす
	const float moveSpeed = 3.0f;
	if (isMoveRight) { texLeftTop.x += moveSpeed; }
	else { texLeftTop.x -= moveSpeed; }
}
