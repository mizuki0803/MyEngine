#include "StageResultUI.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"

StageResultUI* StageResultUI::Create(const int enemyDefeatNum, bool isHighScore)
{
	//ステージリザルトUIのインスタンスを生成
	StageResultUI* stageResultUI = new StageResultUI();
	if (stageResultUI == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!stageResultUI->Initialize(enemyDefeatNum)) {
		delete stageResultUI;
		assert(0);
		return nullptr;
	}

	return stageResultUI;
}

bool StageResultUI::Initialize(const int enemyDefeatNum)
{
	//撃破数取得
	this->enemyDefeatNum = enemyDefeatNum;

	//枠スプライト生成
	frameSprite.reset(Sprite::Create(SpriteTextureLoader::DefeatNumText));
	const Vector2 framePos = { 640, 360 };
	frameSprite->SetPosition(framePos);
	const Vector2 frameSize = { 508, 108 };
	frameSprite->SetSize(frameSize);
	frameSprite->SetTexSize(frameSize);

	//桁数を取得
	int defeatNum = enemyDefeatNum;
	while (defeatNum != 0) {
		defeatNum = defeatNum / 10;
		enemyDefeatNumDigit++;
	}

	//桁数の分、数字スプライト生成
	for (int i = 0; i < enemyDefeatNumDigit; i++) {
		std::unique_ptr<NumberSprite> newNumberSprite;
		const Vector2 size = { 32, 48 };
		const Vector2 pos = { framePos.x + 150 - (i * size.x), framePos.y };
		newNumberSprite.reset(NumberSprite::Create(SpriteTextureLoader::Number, pos, size, size));
		numberSprites.push_back(std::move(newNumberSprite));
	}

	//Bボタンスプライト生成
	bButtonSprite.reset(Sprite::Create(SpriteTextureLoader::BButton));
	const Vector2 bButtonPos = { framePos.x, framePos.y + 90 };
	bButtonSprite->SetPosition(bButtonPos);
	const Vector2 bButtonSize = { 48, 48 };
	bButtonSprite->SetSize(bButtonSize);
	const Vector2 bButtonTexSize = { 64, 64 };
	bButtonSprite->SetTexSize(bButtonTexSize);

	return true;
}

void StageResultUI::Update()
{
	//表示用撃破数の更新
	UpdateDisplayNum();

	//枠スプライト更新
	frameSprite->Update();
	//表示用撃破数の分数字スプライト更新
	for (int i = 0; i < enemyDefeatDisplayNumDigit; i++) {
		numberSprites[i]->Update();
	}
	//Bボタンスプライト更新
	if (isDrawButtonSprite) {
		bButtonSprite->Update();
	}
}

void StageResultUI::Draw()
{
	//枠スプライト描画
	frameSprite->Draw();
	//表示用撃破数の分数字スプライト描画
	for (int i = 0; i < enemyDefeatDisplayNumDigit; i++) {
		numberSprites[i]->Draw();
	}
	//Bボタンスプライト描画
	if (isDrawButtonSprite) {
		bButtonSprite->Draw();
	}
}

void StageResultUI::UpdateDisplayNum()
{
	//リザルトを表示し終えていたら抜ける
	if (isResultEnd) { return; }

	//更新開始する時間
	const int updateStartTime = 30;
	//タイマー更新
	updateDisplayNumTimer++;

	//タイマーが更新開始時間より小さいなら抜ける
	if (updateDisplayNumTimer < updateStartTime) { return; }

	//更新する時間
	const int updateMinTime = 120;
	const float updateTime = (float)(updateMinTime + (enemyDefeatNum / 10));
	const float time = (updateDisplayNumTimer - updateStartTime) / updateTime;

	//表示用撃破数を増やしていく
	enemyDefeatDisplayNum = (int)Easing::LerpFloat(0, (float)enemyDefeatNum, time);

	//表示用撃破数の桁数を更新
	int defeatNum = enemyDefeatDisplayNum;
	int digit = 0;
	while (defeatNum != 0) {
		defeatNum = defeatNum / 10;
		digit++;
	}
	enemyDefeatDisplayNumDigit = digit;

	//数字スプライト更新
	UpdateNumberSprite();

	//タイマーが指定した時間になったら抜ける
	if ((updateDisplayNumTimer - updateStartTime) >= updateTime) {
		//リザルト表示が完了したことにする
		isResultEnd = true;
	}
}

void StageResultUI::UpdateNumberSprite()
{
	//数字をそれぞれ出力する
	for (int i = 0; i < enemyDefeatDisplayNumDigit; i++) {
		//割る数
		int divisionNum = 1;
		//桁数分10を乗算する(1,10,100,...)
		for (int j = 0; j < i; j++) {
			divisionNum *= 10;
		}

		//指定した桁の数字を出す
		int numberDigit = (enemyDefeatDisplayNum / divisionNum) % 10;

		//算出した桁の数字を数字スプライトに割り当てる
		numberSprites[i]->SetNumber(numberDigit);
	}
}
