#include "SortiePlayer.h"
#include "ParticleEmitter.h"
#include "GamePostEffect.h"

void (SortiePlayer::* SortiePlayer::sortieActionFuncTable[])() = {
	&SortiePlayer::SortieAdvance,
	&SortiePlayer::SortieBoost,
};

const float SortiePlayer::advanceSpeed = 1.3f;

SortiePlayer* SortiePlayer::Create(ObjModel* model, const Vector3& startPosition)
{
	//出撃シーン用自機のインスタンスを生成
	SortiePlayer* sortiePlayer = new SortiePlayer();
	if (sortiePlayer == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	sortiePlayer->model = model;

	// 初期化
	if (!sortiePlayer->Initialize()) {
		delete sortiePlayer;
		assert(0);
		return nullptr;
	}

	//初期座標をセット
	sortiePlayer->position = startPosition;
	//小さくしておく
	const float size = 1.5f;
	sortiePlayer->scale = { size, size, size };

	return sortiePlayer;
}

void SortiePlayer::Update()
{
	//行動
	(this->*sortieActionFuncTable[static_cast<size_t>(sortieActionPhase)])();

	//オブジェクト更新
	ObjObject3d::Update();

	//自機のジェット噴射演出用パーティクル生成
	ParticleEmitter::GetInstance()->PlayerJet(matWorld, particleJetSizePhaseNum);
}

void SortiePlayer::BoostStart()
{
	//ブースト状態にする
	sortieActionPhase = SortieActionPhase::Boost;

	//パーティクルジェットを大きくする(加速用にする)
	const int32_t particleSizePhase = 1;
	particleJetSizePhaseNum = particleSizePhase;

	//ポストエフェクトのブラーをかける
	GamePostEffect::GetPostEffect()->SetRadialBlur(true);
	const float blurStrength = 0.2f;
	GamePostEffect::GetPostEffect()->SetRadialBlurStrength(blurStrength);
}

void SortiePlayer::SortieAdvance()
{
	//前進させる
	position.z += advanceSpeed;
}

void SortiePlayer::SortieBoost()
{
	//ブーストさせる(通常スピードの2倍)
	const float boostSpeed = advanceSpeed * 2;
	position.z += boostSpeed;

	//タイマー更新
	boostTimer++;

	//一定時間経ったら出撃行動を終了させる
	const int endTime = 15;
	if (boostTimer >= endTime) {
		isSortieEnd = true;
	}
}
