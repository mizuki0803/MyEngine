#include "StageResultUI.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"

void (StageResultUI::* StageResultUI::resultActionPhaseFuncTable[])() = {
	&StageResultUI::UpdateDisplayNum,
	&StageResultUI::RankMedalSetAction,
};

StageResultUI* StageResultUI::Create(const int enemyDefeatNum, const int enemyDefeatRank)
{
	//ステージリザルトUIのインスタンスを生成
	StageResultUI* stageResultUI = new StageResultUI();
	if (stageResultUI == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!stageResultUI->Initialize(enemyDefeatNum, enemyDefeatRank)) {
		delete stageResultUI;
		assert(0);
		return nullptr;
	}

	return stageResultUI;
}

bool StageResultUI::Initialize(const int enemyDefeatNum, const int enemyDefeatRank)
{
	//撃破数取得
	this->enemyDefeatNum = enemyDefeatNum;

	//枠スプライト生成
	frameSprite.reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::DefeatNumText)));
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
		const Vector2 pos = { framePos.x + 140 - (i * size.x), framePos.y };
		newNumberSprite.reset(NumberSprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::Number), pos, size, size));
		numberSprites.push_back(std::move(newNumberSprite));
	}

	//ランクスプライト生成
	rankMedalSprite.reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::RankMedal)));
	const Vector2 rankMedalPos = { framePos.x + 202, framePos.y };
	rankMedalSprite->SetPosition(rankMedalPos);
	const Vector2 rankMedalTexSize = { 500, 500 };
	rankMedalSprite->SetTexSize(rankMedalTexSize);
	const Vector2 rankMedalTexLeftTop = { rankMedalTexSize.x * enemyDefeatRank, 0 };
	rankMedalSprite->SetTexLeftTop(rankMedalTexLeftTop);
	rankMedalSprite->SetColor({ 1, 1, 1, 0 });

	//Bボタンスプライト生成
	bButtonSprite.reset(Sprite::Create(SpriteTextureLoader::GetTexture(SpriteTextureLoader::BButton)));
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
	//リザルト行動
	if (!isResultEnd) {
		(this->*resultActionPhaseFuncTable[static_cast<size_t>(resultActionPhase)])();
	}

	//枠スプライト更新
	frameSprite->Update();
	//表示用撃破数の分数字スプライト更新
	for (int i = 0; i < enemyDefeatDisplayNumDigit; i++) {
		numberSprites[i]->Update();
	}
	//ランクメダルスプライト更新
	if (resultActionPhase == ResultActionPhase::RankMedalSet) {
		rankMedalSprite->Update();
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
	//ランクメダルスプライト描画
	if (resultActionPhase == ResultActionPhase::RankMedalSet) {
		rankMedalSprite->Draw();
	}
	//Bボタンスプライト描画
	if (isDrawButtonSprite) {
		bButtonSprite->Draw();
	}
}

void StageResultUI::UpdateDisplayNum()
{
	//更新開始する時間
	const int updateStartTime = 30;
	//タイマー更新
	actionTimer++;

	//タイマーが更新開始時間より小さいなら抜ける
	if (actionTimer < updateStartTime) { return; }

	//更新する時間
	const int updateMinTime = 120;
	const float updateTime = (float)(updateMinTime + (enemyDefeatNum / 10));
	const float time = (actionTimer - updateStartTime) / updateTime;

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

	//タイマーが指定した時間になったら
	if ((actionTimer - updateStartTime) >= updateTime) {
		//次のフェーズへ
		resultActionPhase = ResultActionPhase::RankMedalSet;

		//タイマーを初期化しておく
		actionTimer = 0;
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
void StageResultUI::RankMedalSetAction()
{
	//メダルセット開始する時間
	const int medalSetStartTime = 80;
	//タイマー更新
	actionTimer++;

	//タイマーがメダルセット開始時間より小さいなら抜ける
	if (actionTimer < medalSetStartTime) { return; }

	//セットする時間
	const float setTime = 15;
	const float time = (actionTimer - medalSetStartTime) / setTime;

	//メダルのサイズを変更してセットしているように見せる
	float medalSize = Easing::OutQuad(256, 64, time);
	rankMedalSprite->SetSize({ medalSize, medalSize });
	//色を濃くしていく
	float colorAlpha = Easing::OutQuad(0, 1, time);
	rankMedalSprite->SetColor({ 1, 1, 1, colorAlpha });

	//タイマーが指定した時間になったら
	if ((actionTimer - medalSetStartTime) >= setTime) {
		//リザルト表示が完了したことにする
		isResultEnd = true;
	}
}
