#include "Reticle2D.h"

Reticle2D* Reticle2D::Create(UINT texNumber, const Vector2& size)
{
	//2Dレティクルのインスタンスを生成
	Reticle2D* reticle2d = new Reticle2D();
	if (reticle2d == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!reticle2d->Initialize(texNumber, { 0.5f, 0.5f }, false, false)) {
		delete reticle2d;
		assert(0);
		return nullptr;
	}

	//大きさをセット
	reticle2d->size = size;

	return reticle2d;
}

bool Reticle2D::Initialize(UINT texNumber, Vector2 anchorpoint, bool isFlipX, bool isFlipY)
{
	if (!Sprite::Initialize(texNumber, anchorpoint, isFlipX, isFlipY)) {
		return false;
	}

	size = {100, 100};

	return true;
}

void Reticle2D::Update()
{
	Vector3 positionReticle = worldPos3d;

	//ビューポート行列
	XMMATRIX matViewport = {
		{1280 / 2, 0, 0, 0 },
		{0, -720 / 2, 0, 0},
		{0, 0, 1, 0},
		{1280 / 2, 720 / 2, 0, 1}
	};

	//ビュー行列とプロジェクション行列とビューポート行列を合成
	XMMATRIX matViewProjectionViewport =
		matView * matProjection * matViewport;

	//ワールド座標からスクリーン座標に変換
	positionReticle = MatrixTransformWDivision(positionReticle, matViewProjectionViewport);

	//2Dレティクルの座標を設定
	position = { positionReticle.x, positionReticle.y };

	Sprite::Update();
}
