#include "Stage01SortiePlayer.h"
#include "ParticleEmitter.h"
#include "GamePostEffect.h"

void (Stage01SortiePlayer::* Stage01SortiePlayer::sortieActionFuncTable[])() = {
	&Stage01SortiePlayer::SortieAdvance,
	&Stage01SortiePlayer::SortieBoost,
};

const float Stage01SortiePlayer::advanceSpeed = 1.3f;

Stage01SortiePlayer* Stage01SortiePlayer::Create(ObjModel* model, const Vector3& startPosition)
{
	//ステージ01出撃シーン用自機のインスタンスを生成
	Stage01SortiePlayer* sortiePlayer = new Stage01SortiePlayer();
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

void Stage01SortiePlayer::Update()
{
	//行動
	(this->*sortieActionFuncTable[static_cast<size_t>(sortieActionPhase)])();

	//オブジェクト更新
	ObjObject3d::Update();

	//ジェット発射座標を更新
	UpdateJetPos();
	//自機のジェット噴射演出用パーティクル生成
	std::function<Vector3()> getTargetPos = std::bind(&Stage01SortiePlayer::GetJetPos, this);
	ParticleEmitter::GetInstance()->PlayerJet(getTargetPos, matWorld, particleJetSizePhaseNum);
}

void Stage01SortiePlayer::BoostStart()
{
	//ブースト状態にする
	sortieActionPhase = Stage01SortieActionPhase::Boost;

	//パーティクルジェットを大きくする(加速用にする)
	const int32_t particleSizePhase = 1;
	particleJetSizePhaseNum = particleSizePhase;

	//ポストエフェクトのブラーをかける
	GamePostEffect::GetPostEffect()->SetRadialBlur(true);
	const float blurStrength = 0.8f;
	GamePostEffect::GetPostEffect()->SetRadialBlurStrength(blurStrength);
}

void Stage01SortiePlayer::UpdateJetPos()
{
	//自機の中心座標からの距離
	const Vector3 distancePos = { 0, -0.25f, -1.2f };

	//ジェット発射座標を取得
	jetPos = Vector3::LocalTranslation(distancePos, matWorld);
}

void Stage01SortiePlayer::SortieAdvance()
{
	//前進させる
	position.z += advanceSpeed;
}

void Stage01SortiePlayer::SortieBoost()
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
