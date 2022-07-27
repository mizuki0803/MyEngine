#include "Reticle2D.h"
#include "Easing.h"

const XMFLOAT4 Reticle2D::normalColor = { 0.5f, 0.9f, 0.2f, 1 };
const XMFLOAT4 Reticle2D::lockonColor = { 0.9f, 0.1f, 0.1f, 1 };

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

	size = { 100, 100 };
	color = normalColor;

	return true;
}

void Reticle2D::Update()
{
	//敵をロックオンしているとき
	if (isLockon) {
		MoveLockon();
	}
	//敵をロックオンしていないとき
	else {
		MoveNotLockon();
	}

	//スプライトの更新
	Sprite::Update();
}

void Reticle2D::LockonEnemy(Enemy* enemy)
{
	//ロックオン対象の敵をセットしておく
	this->lockonEnemy = enemy;
	//ロックオンしたので色を変更する
	color = lockonColor;
	//ロックオン状態に設定
	isLockon = true;
}

void Reticle2D::UnlockonEnemy()
{
	//ロックオン対象の敵を解除しておく
	this->lockonEnemy = nullptr;
	//ロックオン解除したので色を戻す
	color = normalColor;
	//ロックオンしていない状態に設定
	isLockon = false;
	//レティクルを元の位置(自機追従)に戻す状態に設定
	isBackPos = true;
	//元の位置(自機追従)に戻すタイマーを初期化
	backTimer = 0;
}

void Reticle2D::MoveLockon()
{
	//ロックオン対象の敵のスクリーン座標を追従する
	position = lockonEnemy->GetScreenPos();

	//レティクルが画面外に出たらロックオン状態を解除する
	const bool isOutsideScreen = (position.x > WindowApp::window_width || position.x < 0 || position.y > WindowApp::window_height || position.y < 0);
	if (isOutsideScreen) {
		UnlockonEnemy();
	}
}

void Reticle2D::MoveNotLockon()
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

	//元の位置(自機追従)に戻す状態のとき
	if (isBackPos) {
		//Lerpを使って自機追従に戻す
		const float maxTime = 60;
		backTimer++;
		float time = backTimer / maxTime;
		position = Easing::Lerp(position, screenPosition, time);

		//Lerpが終わると自機追従に戻っている
		if (backTimer >= maxTime) {
			isBackPos = false;
		}
	}
	//自機追従しているとき
	else {
		//2Dレティクルの座標を設定
		position = screenPosition;
	}
}
