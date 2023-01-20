#include "Stage02SortiePlayer.h"
#include "ParticleEmitter.h"
#include "GamePostEffect.h"
#include "BaseStageScene.h"
#include "StraightBullet.h"

void (Stage02SortiePlayer::* Stage02SortiePlayer::sortieActionFuncTable[])() = {
	&Stage02SortiePlayer::SortieAdvance,
	&Stage02SortiePlayer::SortieBoost,
};

BaseStageScene* Stage02SortiePlayer::stageScene = nullptr;
ObjModel* Stage02SortiePlayer::bulletModel = nullptr;
const float Stage02SortiePlayer::advanceSpeed = 1.0f;

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
	//発射座標更新
	UpdateBulletShotPos();

	//弾発射
	Fire();

	//行動
	(this->*sortieActionFuncTable[static_cast<size_t>(sortieActionPhase)])();

	//オブジェクト更新
	ObjObject3d::Update();

	//ジェット発射座標を更新
	UpdateJetPos();
	//自機のジェット噴射演出用パーティクル生成
	std::function<Vector3()> getTargetPos = std::bind(&Stage02SortiePlayer::GetJetPos, this);
	ParticleEmitter::GetInstance()->PlayerJet(getTargetPos, matWorld, particleJetSizePhaseNum);
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

void Stage02SortiePlayer::UpdateJetPos()
{
	//自機の中心座標からの距離
	const Vector3 distancePos = { 0, -0.25f, -1.2f };

	//ジェット発射座標を取得
	jetPos = Vector3::LocalTranslation(distancePos, matWorld);
}

void Stage02SortiePlayer::UpdateBulletShotPos()
{
	//自機の中心座標からの距離
	const Vector3 distancePos = { 0, -0.3f, 5.0f };

	//弾発射座標を取得
	bulletShotPos = Vector3::LocalTranslation(distancePos, matWorld);
}

void Stage02SortiePlayer::ShotStraightBullet()
{
	//弾の速度を設定
	const float bulletSpeed = 4;
	//自機の向いている方向に飛ばす
	Vector3 velocity = { 0, 0, bulletSpeed };
	velocity = MatrixTransformDirection(velocity, matWorld);

	//直進弾を生成
	std::unique_ptr<PlayerBullet> newBullet;
	newBullet.reset(StraightBullet::Create(bulletModel, bulletShotPos, velocity));
	stageScene->AddPlayerBullet(std::move(newBullet));

	//ショット演出用パーティクル生成
	ParticleEmitter::GetInstance()->Shot(bulletShotPos);
}

void Stage02SortiePlayer::Fire()
{
	//指定した座標以下なら抜ける
	const float fireStartPos = 270;
	if (position.z < fireStartPos) { return; }
	//既に3回発射していたら抜ける
	if (fireCount > 3) { return; }

	//弾発射間隔
	const int fireInterval = 8;
	//タイマー更新
	fireTimer++;
	if (fireTimer <= fireInterval) { return; }

	//弾発射
	ShotStraightBullet();
	//発射回数更新
	fireCount++;
	//タイマー初期化
	fireTimer = 0;
}

void Stage02SortiePlayer::SortieAdvance()
{
	//前進させる
	position.z += advanceSpeed;
}

void Stage02SortiePlayer::SortieBoost()
{
	//ブーストさせる(通常スピードの3倍)
	const float boostSpeed = advanceSpeed * 3;
	position.z += boostSpeed;

	//タイマー更新
	boostTimer++;

	//一定時間経ったら出撃行動を終了させる
	const int endTime = 50;
	if (boostTimer >= endTime) {
		isSortieEnd = true;
	}
}
