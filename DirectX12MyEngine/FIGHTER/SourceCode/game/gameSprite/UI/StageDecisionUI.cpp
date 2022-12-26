#include "StageDecisionUI.h"
#include "SpriteTextureLoader.h"
#include "Easing.h"

void (StageDecisionUI::* StageDecisionUI::actionFuncTable[])() = {
	&StageDecisionUI::TextStay,
	&StageDecisionUI::TextUp,
	&StageDecisionUI::TextStay,
	&StageDecisionUI::TextDown,
};

const Vector2 StageDecisionUI::stayPos = { 640, 360 };
const Vector2 StageDecisionUI::outScreenPos = { 640, 1000 };

StageDecisionUI* StageDecisionUI::Create()
{
	//ステージ決定確認UIのインスタンスを生成
	StageDecisionUI* stageDecisionUI = new StageDecisionUI();
	if (stageDecisionUI == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!stageDecisionUI->Initialize()) {
		delete stageDecisionUI;
		assert(0);
		return nullptr;
	}

	return stageDecisionUI;
}

bool StageDecisionUI::Initialize()
{
	//スプライト生成
	stageDecisionSprite.reset(Sprite::Create(SpriteTextureLoader::StageDecision));
	stageDecisionSprite->SetPosition(outScreenPos);
	const Vector2 texSize = { 400, 300 };
	stageDecisionSprite->SetTexSize(texSize);
	const Vector2 size = texSize * 1.5f;
	stageDecisionSprite->SetSize(size);

	return true;
}

void StageDecisionUI::Update()
{
	//行動
	(this->*actionFuncTable[static_cast<size_t>(phase)])();

	//スプライト更新
	stageDecisionSprite->Update();
}

void StageDecisionUI::Draw()
{
	//スプライト描画
	stageDecisionSprite->Draw();
}

void StageDecisionUI::TextUpStart(int stageNum)
{
	//ステージを決定する状態にセット
	isStageDecision = true;

	//ステージ番号が0ならタイトルシーンを選択している状態にする
	if (stageNum == 0) { isSelectTitleStage = true; }
	else { isSelectTitleStage = false; }

	//スプライトの状態も変更する
	float leftTop = 0;
	if (isStageDecision) { leftTop = 0.0f; }
	else { leftTop = 1.0f; }

	//タイトルシーンを選択しているときのみテクスチャを2つずらす
	if (isSelectTitleStage) { leftTop += 2; }

	stageDecisionSprite->SetTexLeftTop({ stageDecisionSprite->GetTexSize().x * leftTop, 0 });

	//移動タイマー初期化
	moveTimer = 0;

	//状態を変更
	phase = ActionPhase::Up;
}

void StageDecisionUI::TextDownStart()
{
	//移動タイマー初期化
	moveTimer = 0;

	//状態を変更
	phase = ActionPhase::Down;
}

void StageDecisionUI::ChangeStageDecision(bool isStageDecision)
{
	//既に同じ状態なら抜ける
	if (this->isStageDecision == isStageDecision) { return; }

	//スプライトの状態も変更する
	float leftTop = 0;
	if (isStageDecision) { leftTop = 0.0f; }
	else { leftTop = 1.0f; }

	//タイトルシーンを選択しているときのみテクスチャを2つずらす
	if (isSelectTitleStage) { leftTop += 2; }

	stageDecisionSprite->SetTexLeftTop({ stageDecisionSprite->GetTexSize().x * leftTop, 0 });

	//状態を引数の状態にセット
	this->isStageDecision = isStageDecision;
}

void StageDecisionUI::TextStay()
{
}

void StageDecisionUI::TextUp()
{
	//タイマー更新
	const float upTime = 45;
	moveTimer++;
	const float time = moveTimer / upTime;

	//上に動かす
	Vector2 pos = stageDecisionSprite->GetPosition();
	pos.y = Easing::OutCubic(outScreenPos.y, stayPos.y, time);
	stageDecisionSprite->SetPosition(pos);

	//タイマーが指定した時間になったら
	if (moveTimer >= upTime) {
		//状態を変更
		phase = ActionPhase::Show;
	}
}

void StageDecisionUI::TextDown()
{
	//タイマー更新
	const float downTime = 45;
	moveTimer++;
	const float time = moveTimer / downTime;

	//上に動かす
	Vector2 pos = stageDecisionSprite->GetPosition();
	pos.y = Easing::OutCubic(stayPos.y, outScreenPos.y, time);
	stageDecisionSprite->SetPosition(pos);

	//タイマーが指定した時間になったら
	if (moveTimer >= downTime) {
		//状態を変更
		phase = ActionPhase::Stay;
	}
}
