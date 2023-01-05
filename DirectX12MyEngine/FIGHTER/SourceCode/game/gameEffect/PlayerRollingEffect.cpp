#include "PlayerRollingEffect.h"
#include "ParticleEmitter.h"

void (PlayerRollingEffect::* PlayerRollingEffect::createPhaseFuncTable[])() = {
	&PlayerRollingEffect::Phase01Create,
	&PlayerRollingEffect::Phase02Create,
};

PlayerRollingEffect* PlayerRollingEffect::Create(std::function<Vector3()> getTargetPos, const bool isRollRight)
{
	//自機の緊急回避エフェクト管理のインスタンスを生成
	PlayerRollingEffect* playerRollingEffect = new PlayerRollingEffect();
	if (playerRollingEffect == nullptr) {
		return nullptr;
	}

	//追従対象の座標取得関数をセット
	playerRollingEffect->getTargetPos = getTargetPos;
	//回転方向をセット
	playerRollingEffect->isRollRight = isRollRight;

	return playerRollingEffect;
}

void PlayerRollingEffect::Update()
{
	//生成フェーズ
	(this->*createPhaseFuncTable[static_cast<size_t>(createPhase)])();
}

void PlayerRollingEffect::Phase01Create()
{
	//このフェーズで生成するエフェクトの情報をセット
	std::vector<EffectParam> effectParams = {
		{20, {2, 2, -1}, 125, 5, { 0.4f, 0.5f, 0.5f, 1.0f }},
		{20, {2, 2, -1}, 135, 5, { 0.4f, 0.5f, 0.5f, 1.0f }},
		{20, {-2, -2, -1}, 305, 5, { 0.4f, 0.5f, 0.5f, 1.0f }},
		{20, {-2, -2, -1}, 315, 5, { 0.4f, 0.5f, 0.5f, 1.0f }},

		{25, {2, 2, -1}, 125, 7.5f, { 0.1f, 0.5f, 0.5f, 1.0f }},
		{25, {2, 2, -1}, 135, 7.5f, { 0.1f, 0.5f, 0.5f, 1.0f }},
		{25, {-2, -2, -1}, 305, 7.5f, { 0.1f, 0.5f, 0.5f, 1.0f }},
		{25, {-2, -2, -1}, 315, 7.5f, { 0.1f, 0.5f, 0.5f, 1.0f }},

		{30, {2, 2, -1}, 125, 10, { 0.05f, 0.25f, 0.25f, 1.0f }},
		{30, {2, 2, -1}, 135, 10, { 0.05f, 0.25f, 0.25f, 1.0f }},
		{30, {-2, -2, -1}, 305, 10, { 0.05f, 0.25f, 0.25f, 1.0f }},
		{30, {-2, -2, -1}, 315, 10, { 0.05f, 0.25f, 0.25f, 1.0f }},
	};
	//パラメータを元にエフェクトを生成
	CreateEffects(effectParams);

	//次のフェーズへ
	createPhase = CreatePhase::Phase02;
}

void PlayerRollingEffect::Phase02Create()
{
	//このフェーズで生成までかかる時間
	const int phaseTime = 5;
	//タイマー更新
	createTimer++;

	//タイマーが指定した時間に以下なら抜ける
	if (createTimer < phaseTime) { return; }

	//このフェーズで生成するエフェクトの情報をセット
	std::vector<EffectParam> effectParams = {
		{20, {2, -2, -1}, 35, 5, { 0.4f, 0.5f, 0.5f, 1.0f }},
		{20, {2, -2, -1}, 45, 5, { 0.4f, 0.5f, 0.5f, 1.0f }},
		{20, {-2, 2, -1}, 215, 5, { 0.4f, 0.5f, 0.5f, 1.0f }},
		{20, {-2, 2, -1}, 225, 5, { 0.4f, 0.5f, 0.5f, 1.0f }},

		{25, {2, -2, -1}, 35, 7.5f, { 0.1f, 0.5f, 0.5f, 1.0f }},
		{25, {2, -2, -1}, 45, 7.5f, { 0.1f, 0.5f, 0.5f, 1.0f }},
		{25, {-2, 2, -1}, 215, 7.5f, { 0.1f, 0.5f, 0.5f, 1.0f }},
		{25, {-2, 2, -1}, 225, 7.5f, { 0.1f, 0.5f, 0.5f, 1.0f }},

		{30, {2, -2, -1}, 35, 10, { 0.05f, 0.25f, 0.25f, 1.0f }},
		{30, {2, -2, -1}, 45, 10, { 0.05f, 0.25f, 0.25f, 1.0f }},
		{30, {-2, 2, -1}, 215, 10, { 0.05f, 0.25f, 0.25f, 1.0f }},
		{30, {-2, 2, -1}, 225, 10, { 0.05f, 0.25f, 0.25f, 1.0f }},
	};
	
	//パラメータを元にエフェクトを生成
	CreateEffects(effectParams);

	//削除状態にする
	isDelete = true;
}

void PlayerRollingEffect::CreateEffects(const std::vector<EffectParam>& effectParams)
{
	//引数の要素も数、エフェクトを生成する
	for (int i = 0; i < (int)effectParams.size(); i++) {
		ParticleEmitter::GetInstance()->PlayerRolling(effectParams[i].life, getTargetPos,
			effectParams[i].localPos, effectParams[i].rotation, effectParams[i].size, effectParams[i].color);
	}
}
