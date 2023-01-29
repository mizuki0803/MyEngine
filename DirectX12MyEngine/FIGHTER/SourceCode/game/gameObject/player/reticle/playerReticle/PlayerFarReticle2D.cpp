#include "PlayerFarReticle2D.h"
#include "Easing.h"

const XMFLOAT4 PlayerFarReticle2D::chargeLockonColor = { 0.9f, 0.1f, 0.1f, 1 };

PlayerFarReticle2D* PlayerFarReticle2D::Create(const Texture& texture, const Vector2& normalSize, const Vector2& chargeModeSize)
{
	//自機付属の2Dレティクル(遠)のインスタンスを生成
	PlayerFarReticle2D* playerFarReticle2D = new PlayerFarReticle2D();
	if (playerFarReticle2D == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!playerFarReticle2D->Initialize(texture, { 0.5f, 0.5f }, false, false)) {
		delete playerFarReticle2D;
		assert(0);
		return nullptr;
	}

	//大きさをセット
	playerFarReticle2D->size = normalSize;

	//通常時の大きさを記録しておく
	playerFarReticle2D->normalSize = normalSize;

	//チャージ時の大きさを記録しておく
	playerFarReticle2D->chargeModeSize = chargeModeSize;

	return playerFarReticle2D;
}

void PlayerFarReticle2D::Update()
{
	//チャージ状態の挙動
	ChargeMode();

	//敵をロックオンしているときの移動
	if (isLockon) {
		MoveLockon();
	}
	//敵をロックオンしていないときの移動
	else {
		Move();
	}

	//スプライトの更新
	Sprite::Update();
}

void PlayerFarReticle2D::ChargeModeStart()
{
	//変更前の大きさを記録しておく
	beforeChangeSize = size;
	//色をチャージ状態(ロックオン状態)に変更
	color = chargeLockonColor;
	//チャージ状態にする
	isChargeMode = true;
	//チャージ状態開始状態にする
	isChargeModeStart = true;
	//チャージ状態終了状態を終了させておく
	isChargeModeEnd = false;
	//チャージ状態用タイマーを初期化
	chargeTimer = 0;
}

void PlayerFarReticle2D::ChargeModeEnd()
{
	//変更前の大きさを記録しておく
	beforeChangeSize = size;
	//色を通常時の色に戻す
	color = normalColor;
	//チャージ状態を終了
	isChargeMode = false;
	//チャージ状態開始状態を終了させておく
	isChargeModeStart = false;
	//チャージ状態終了状態にする
	isChargeModeEnd = true;
	//チャージ状態用タイマーを初期化
	chargeTimer = 0;	
}

void PlayerFarReticle2D::LockonEnemy(Enemy* enemy)
{
	//大きさを元に戻す
	size = normalSize;
	//ロックオン対象の敵をセットしておく
	lockonEnemy = enemy;
	//ロックオンしたので色を変更する
	color = chargeLockonColor;
	//ロックオン状態に設定
	isLockon = true;
}

void PlayerFarReticle2D::UnlockonEnemy()
{
	//ロックオン対象の敵を解除しておく
	lockonEnemy = nullptr;
	//ロックオンしていない状態に設定
	isLockon = false;
	//レティクルを元の位置(自機追従)に戻す状態に設定
	isBackPos = true;
	//元の位置(自機追従)に戻すタイマーを初期化
	backTimer = 0;
	//ロックオン解除したので色を通常色に戻す
	color = normalColor;
}

void PlayerFarReticle2D::ChargeMode()
{
	if (isChargeModeStart) {
		ChargeModeStartSizeChange();
	}
	else if (isChargeModeEnd) {
		ChargeModeEndSizeChange();
	}
	else  if (isChargeMode) {
		ChargeModeSizeChange();
	}
}

void PlayerFarReticle2D::ChargeModeStartSizeChange()
{
	//タイマーを更新
	const float chargeModeEndTime = 10;
	chargeTimer++;
	const float time = chargeTimer / chargeModeEndTime;

	//大きさを大きくする
	size.x = Easing::OutQuad(beforeChangeSize.x, chargeModeSize.x, time);
	size.y = Easing::OutQuad(beforeChangeSize.y, chargeModeSize.y, time);

	//タイマーが指定した時間になったら緊急回避終了
	if (chargeTimer >= chargeModeEndTime) {
		//チャージ状態開始状態を終了
		isChargeModeStart = false;
	}
}

void PlayerFarReticle2D::ChargeModeEndSizeChange()
{
	//タイマーを更新
	const float chargeModeEndTime = 10;
	chargeTimer++;
	const float time = chargeTimer / chargeModeEndTime;

	//大きさを元に戻す
	size.x = Easing::OutQuad(beforeChangeSize.x, normalSize.x, time);
	size.y = Easing::OutQuad(beforeChangeSize.y, normalSize.y, time);

	//タイマーが指定した時間になったら緊急回避終了
	if (chargeTimer >= chargeModeEndTime) {
		//チャージ状態終了状態を終了
		isChargeModeEnd = false;
	}
}

void PlayerFarReticle2D::ChargeModeSizeChange()
{
	//チャージ状態時の大きさを小さく変更していく速さ
	Vector2 sizeChangeSpeed = { 1, 1 };
	size -= sizeChangeSpeed;

	//一定ラインまで小さくなったらチャージ時の大きさに戻す
	const float chargeModeSizeMin = 80.0f;
	if (size.x <= chargeModeSizeMin) {
		size = chargeModeSize;
	}
}

void PlayerFarReticle2D::MoveLockon()
{
	//ロックオン対象の敵のスクリーン座標を追従する
	position = lockonEnemy->GetScreenPos();

	//レティクルが画面外に出たらロックオン状態を解除する
	const bool isOutsideScreen = (position.x > WindowApp::window_width || position.x < 0 || position.y > WindowApp::window_height || position.y < 0);
	if (isOutsideScreen) {
		UnlockonEnemy();
	}
}

void PlayerFarReticle2D::Move()
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
		position = Easing::LerpVec2(position, screenPosition, time);

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
