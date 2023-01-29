#include "Reticle2D.h"
#include "Easing.h"

const XMFLOAT4 Reticle2D::normalColor = { 0.5f, 0.9f, 0.2f, 1 };

Reticle2D* Reticle2D::Create(const Texture& texture, const Vector2& size)
{
	//2Dレティクルのインスタンスを生成
	Reticle2D* reticle2d = new Reticle2D();
	if (reticle2d == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!reticle2d->Initialize(texture, { 0.5f, 0.5f }, false, false)) {
		delete reticle2d;
		assert(0);
		return nullptr;
	}

	//大きさをセット
	reticle2d->size = size;

	return reticle2d;
}

bool Reticle2D::Initialize(const Texture& texture, const Vector2& anchorpoint, bool isFlipX, bool isFlipY)
{
	if (!Sprite::Initialize(texture, anchorpoint, isFlipX, isFlipY)) {
		return false;
	}

	//通常色をセット
	color = normalColor;

	return true;
}

void Reticle2D::Update()
{
	//移動
	Move();

	//スプライトの更新
	Sprite::Update();
}

void Reticle2D::Move()
{
	//3Dレティクルの座標をスクリーン座標に変換させる
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
	Vector2 screenPosition = { positionReticle.x, positionReticle.y };

	//2Dレティクルの座標を設定
	position = screenPosition;
}
