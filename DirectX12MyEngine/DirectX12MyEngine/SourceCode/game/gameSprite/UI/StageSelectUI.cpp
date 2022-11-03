#include "StageSelectUI.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"
#include "EnemyDefeatCounter.h"

void (StageSelectUI::* StageSelectUI::actionFuncTable[])() = {
	&StageSelectUI::TextStay,
	&StageSelectUI::TextInScreen,
	&StageSelectUI::TextStay,
	&StageSelectUI::TextOutScreen,
};

const float StageSelectUI::inScreenPosY = 100;
const float StageSelectUI::outScreenPosY = -100;
const float StageSelectUI::frameNumberDistanceY = 25;

StageSelectUI* StageSelectUI::Create()
{
	//ステージ選択画面UIのインスタンスを生成
	StageSelectUI* stageSelectUI = new StageSelectUI();
	if (stageSelectUI == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!stageSelectUI->Initialize()) {
		delete stageSelectUI;
		assert(0);
		return nullptr;
	}

	return stageSelectUI;
}

bool StageSelectUI::Initialize()
{
	//惑星名スプライト生成
	planetNameSprite.reset(Sprite::Create(SpriteTextureLoader::PlanetName));
	planetNameSprite->SetPosition({ 640, outScreenPosY });
	const Vector2 planetNameTexSize = { 500, 140 };
	planetNameSprite->SetTexSize(planetNameTexSize);
	planetNameSprite->SetSize(planetNameTexSize);

	//ハイスコア枠スプライト生成
	highScoreFrameSprite.reset(Sprite::Create(SpriteTextureLoader::HighScoreFrame));
	highScoreFrameSprite->SetPosition({ 1000, outScreenPosY });
	const Vector2 frameTexSize = { 172, 126 };
	highScoreFrameSprite->SetTexSize(frameTexSize);
	highScoreFrameSprite->SetSize(frameTexSize);

	//数字スプライト生成
	for (int i = 0; i < highScoreDigit; i++) {
		const Vector2 texSize = { 48, 48 };
		const Vector2 size = texSize * 0.8f;
		const Vector2 centerPos = highScoreFrameSprite->GetPosition();
		const Vector2 firstPos = { centerPos.x + size.x, centerPos.y + frameNumberDistanceY };
		const Vector2 pos = { firstPos.x - i * size.x, firstPos.y };
		numberSprites[i].reset(NumberSprite::Create(SpriteTextureLoader::HitPlusNumber, pos, size, texSize));
	}

	//操作説明スプライト生成
	howToPlaySprite.reset(Sprite::Create(SpriteTextureLoader::StageSelectHowToPlay));
	howToPlaySprite->SetPosition({ 640, 550 });
	const Vector2 howToPLayTexSize = { 200, 64 };
	howToPlaySprite->SetTexSize(howToPLayTexSize);
	howToPlaySprite->SetSize(howToPLayTexSize);

	return true;
}

void StageSelectUI::Update()
{
	//行動
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//スプライト更新
	planetNameSprite->Update();
	highScoreFrameSprite->Update();
	for (int i = 0; i < highScoreDigit; i++) {
		numberSprites[i]->Update();
	}
	howToPlaySprite->Update();
}

void StageSelectUI::Draw()
{
	//スプライト描画
	planetNameSprite->Draw();
	highScoreFrameSprite->Draw();
	for (int i = 0; i < highScoreDigit; i++) {
		numberSprites[i]->Draw();
	}

	//表示状態でなければ抜ける
	if (!(phase == ActionPhase::Show)) { return; }

	howToPlaySprite->Draw();
}

void StageSelectUI::TextInScreenStart(const int stageNum)
{
	//タイマーを初期化させる
	moveTimer = 0;

	//ステージ番号に合わせて表示するものを決める
	planetNameSprite->SetTexLeftTop({ planetNameSprite->GetTexSize().x * stageNum, 0 });

	//ハイスコアを表示するか決定
	if (stageNum == 1) { isHighScoreShow = true; }
	else { isHighScoreShow = false; }

	//ハイスコアを表示する場合
	if (isHighScoreShow) {
		//数字スプライトをハイスコアに更新
		const int highScore = EnemyDefeatCounter::GetHighScore();

		//ハイスコアが0(未設定)の場合は棒を表示
		if (highScore == 0) {
			//桁数の分、数字更新
			for (int i = 0; i < highScoreDigit; i++) {
				numberSprites[i]->SetNumber(10);
			}
		}
		//ハイスコアに0以外が設定されている場合
		else {
			//桁数の分、数字更新
			for (int i = 0; i < highScoreDigit; i++) {
				//割る数
				int divisionNum = 1;
				//桁数分10を乗算する(1,10,100,...)
				for (int j = 0; j < i; j++) {
					divisionNum *= 10;
				}
				//指定した桁の数字を出す
				int numberDigit = (highScore / divisionNum) % 10;
				//算出した桁の数字を数字スプライトに割り当てる
				numberSprites[i]->SetNumber(numberDigit);
			}
		}
	}

	//操作説明スプライトをステージに合わせて変更
	howToPlaySprite->SetTexLeftTop({ howToPlaySprite->GetTexSize().x * stageNum, 0 });

	//画面内に移動する状態に変更
	phase = ActionPhase::InScreen;

	//表示している状態にする
	isShow = true;
}

void StageSelectUI::TextOutScreenStart()
{
	//タイマーを初期化させる
	moveTimer = 0;

	//画面外に移動する状態に変更
	phase = ActionPhase::OutScreen;

	//画面外に移動開始する座標をセット
	moveOutScreenStartPos = planetNameSprite->GetPosition();

	//表示していない状態にする
	isShow = false;
}

void StageSelectUI::TextStay()
{
}

void StageSelectUI::TextInScreen()
{
	//タイマー更新
	const float moveTime = 45;
	moveTimer++;
	const float time = moveTimer / moveTime;

	//画面内に動かす
	Vector2 pos = planetNameSprite->GetPosition();
	pos.y = Easing::OutCubic(outScreenPosY, inScreenPosY, time);
	planetNameSprite->SetPosition(pos);
	//ハイスコアを表示する場合
	if (isHighScoreShow) {
		//枠スプライト
		Vector2 framePos = highScoreFrameSprite->GetPosition();
		framePos.y = Easing::OutCubic(outScreenPosY, inScreenPosY, time);
		highScoreFrameSprite->SetPosition(framePos);

		//数字スプライト
		for (int i = 0; i < highScoreDigit; i++) {
			Vector2 numberPos = numberSprites[i]->GetPosition();
			numberPos.y = framePos.y + frameNumberDistanceY;
			numberSprites[i]->SetPosition(numberPos);
		}
	}

	//タイマーが指定した時間になったら
	if (moveTimer >= moveTime) {
		//状態を変更
		phase = ActionPhase::Show;
	}
}

void StageSelectUI::TextOutScreen()
{
	//タイマー更新
	const float moveTime = 45;
	moveTimer++;
	const float time = moveTimer / moveTime;

	//画面外に動かす
	Vector2 pos = planetNameSprite->GetPosition();
	pos.y = Easing::OutCubic(moveOutScreenStartPos.y, outScreenPosY, time);
	planetNameSprite->SetPosition(pos);
	//ハイスコアを表示する場合
	if (isHighScoreShow) {
		//枠スプライト
		Vector2 framePos = highScoreFrameSprite->GetPosition();
		framePos.y = Easing::OutCubic(moveOutScreenStartPos.y, outScreenPosY, time);
		highScoreFrameSprite->SetPosition(framePos);

		//数字スプライト
		for (int i = 0; i < highScoreDigit; i++) {
			Vector2 numberPos = numberSprites[i]->GetPosition();
			numberPos.y = framePos.y + frameNumberDistanceY;
			numberSprites[i]->SetPosition(numberPos);
		}
	}

	//タイマーが指定した時間になったら
	if (moveTimer >= moveTime) {
		//状態を変更
		phase = ActionPhase::Stay;
	}
}
