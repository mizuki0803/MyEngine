#include "Stage02SortiePlayer.h"
#include "ParticleEmitter.h"
#include "GamePostEffect.h"

void (Stage02SortiePlayer::* Stage02SortiePlayer::sortieActionFuncTable[])() = {
	&Stage02SortiePlayer::SortieAdvance,
	&Stage02SortiePlayer::SortieBoost,
};

const float Stage02SortiePlayer::advanceSpeed = 1.3f;

Stage02SortiePlayer* Stage02SortiePlayer::Create(ObjModel* model, const Vector3& startPosition)
{
	//ステージ01出撃シーン用自機のインスタンスを生成
	Stage02SortiePlayer* sortiePlayer = new Stage02SortiePlayer();
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

void Stage02SortiePlayer::Update()
{
	//行動
	(this->*sortieActionFuncTable[static_cast<size_t>(sortieActionPhase)])();

	//オブジェクト更新
	ObjObject3d::Update();

	//自機のジェット噴射演出用パーティクル生成
	ParticleEmitter::GetInstance()->PlayerJet(matWorld, particleJetSizePhaseNum);
}

void Stage02SortiePlayer::BoostStart()
{
	//ブースト状態にする
	sortieActionPhase = Stage02SortieActionPhase::Boost;

	//パーティクルジェットを大きくする(加速用にする)
	const int32_t particleSizePhase = 1;
	particleJetSizePhaseNum = particleSizePhase;

	//ポストエフェクトのブラーをかける
	GamePostEffect::GetPostEffect()->SetRadialBlur(true);
	const float blurStrength = 0.8f;
	GamePostEffect::GetPostEffect()->SetRadialBlurStrength(blurStrength);
}

void Stage02SortiePlayer::SortieAdvance()
{
	//前進させる
	position.z += advanceSpeed;
}

void Stage02SortiePlayer::SortieBoost()
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
