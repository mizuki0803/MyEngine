#include "MultiHitUI.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"
#include <DirectXMath.h>

MultiHitUI* MultiHitUI::Create(const Vector3& position, GameCamera* camera, const int enemyHitNum)
{
	//複数撃破UIのインスタンスを生成
	MultiHitUI* multiHitUI = new MultiHitUI();
	if (multiHitUI == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!multiHitUI->Initialize(position, camera, enemyHitNum)) {
		delete multiHitUI;
		assert(0);
		return nullptr;
	}

	return multiHitUI;
}

bool MultiHitUI::Initialize(const Vector3& position, GameCamera* camera, const int enemyHitNum)
{
	//HITワールド座標取得
	this->hitWorldPos = position;
	//カメラ取得
	this->camera = camera;
	//HIT数取得
	this->hitNum = enemyHitNum;


	Vector2 hitScreenPos = GetScreenPos(hitWorldPos, camera);

	//枠スプライト生成
	hitTextSprite.reset(Sprite::Create(SpriteTextureLoader::HitPlusText));
	const Vector2 hitTextPos = { hitScreenPos.x + 25, hitScreenPos.y };
	hitTextSprite->SetPosition(hitTextPos);
	const Vector2 hitTextSize = { 150, 37.5 };
	const Vector2 hitTextTexSize = { 200, 50 };
	hitTextSprite->SetSize(hitTextSize);
	hitTextSprite->SetTexSize(hitTextTexSize);

	//桁数を取得
	int defeatNum = enemyHitNum;
	while (defeatNum != 0) {
		defeatNum = defeatNum / 10;
		hitNumDigit++;
	}

	//桁数の分、数字スプライト生成
	for (int i = 0; i < hitNumDigit; i++) {
		std::unique_ptr<NumberSprite> newNumberSprite;
		const Vector2 size = { 36, 36 };
		const Vector2 texSize = { 48, 48 };
		const Vector2 pos = { hitTextPos.x + texSize.x + ((hitNumDigit - i) * size.x), hitTextPos.y };
		newNumberSprite.reset(NumberSprite::Create(SpriteTextureLoader::HitPlusNumber, pos, size, texSize));
		//割る数
		int divisionNum = 1;
		//桁数分10を乗算する(1,10,100,...)
		for (int j = 0; j < i; j++) {
			divisionNum *= 10;
		}
		//指定した桁の数字を出す
		int numberDigit = (hitNum / divisionNum) % 10;
		//算出した桁の数字を数字スプライトに割り当てる
		newNumberSprite->SetNumber(numberDigit);
		numberSprites.push_back(std::move(newNumberSprite));
	}

	return true;
}

void MultiHitUI::Update()
{
	//寿命タイマー更新
	deadTimer++;
	//タイマーが達したら死亡
	if (deadTimer >= lifeTime) {
		isDead = true;
	}

	//テキストをワールド座標上で上に動かす
	TextMove();

	//カメラより手前にあるか確認
	FrontOfScreenDelete();

	//「HIT」テキスト更新
	hitTextSprite->Update();

	//桁数の分、数字スプライト更新
	for (int i = 0; i < hitNumDigit; i++) {
		numberSprites[i]->Update();
	}
}

void MultiHitUI::Draw()
{
	//「HIT」テキスト描画
	hitTextSprite->Draw();

	//桁数の分、数字スプライト描画
	for (int i = 0; i < hitNumDigit; i++) {
		numberSprites[i]->Draw();
	}
}

Vector2 MultiHitUI::GetScreenPos(const Vector3& position, GameCamera* camera)
{
	Vector3 worldPos = position;

	//ビューポート行列
	DirectX::XMMATRIX matViewport = {
		{1280 / 2, 0, 0, 0 },
		{0, -720 / 2, 0, 0},
		{0, 0, 1, 0},
		{1280 / 2, 720 / 2, 0, 1}
	};

	//ビュー行列とプロジェクション行列とビューポート行列を合成
	DirectX::XMMATRIX matViewProjectionViewport =
		camera->GetMatView() * camera->GetMatProjection() * matViewport;

	//ワールド座標からスクリーン座標に変換
	worldPos = MatrixTransformWDivision(worldPos, matViewProjectionViewport);
	Vector2 screenPos = { worldPos.x, worldPos.y };

	return screenPos;
}

void MultiHitUI::TextMove()
{
	//イージング用タイム取得
	const float time = (float)deadTimer / (float)lifeTime;

	//ワールド座標上で上に動かす
	Vector3 worldPos = hitWorldPos;
	const float upNum = 5.0f;
	worldPos.y = Easing::OutQuint(hitWorldPos.y, hitWorldPos.y + upNum, time);

	//スクリーン座標に変換
	Vector2 screenPos = GetScreenPos(worldPos, camera);

	//HITスプライトの座標をセット
	const Vector2 hitTextPos = { screenPos.x - 10, screenPos.y };
	hitTextSprite->SetPosition(hitTextPos);

	//桁数の分、数字スプライトの座標をセット
	for (int i = 0; i < hitNumDigit; i++) {
		const Vector2 size = numberSprites[i]->GetSize();
		const Vector2 pos = { hitTextPos.x + 48 + ((hitNumDigit - i) * size.x), hitTextPos.y };
		numberSprites[i]->SetPosition(pos);
	}
}

void MultiHitUI::FrontOfScreenDelete()
{
	//座標がカメラより手前(画面外手前)まで行ったら削除
	if (hitWorldPos.z > camera->GetPosition().z) { return; }

	isDead = true;
}
