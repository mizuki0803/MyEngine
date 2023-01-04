#include "TitlePlayer.h"
#include "ParticleEmitter.h"
#include "Easing.h"
#include "GamePostEffect.h"

void (TitlePlayer::* TitlePlayer::sortieActionFuncTable[])() = {
	&TitlePlayer::SortieStay,
	&TitlePlayer::SortieBoost,
};

TitlePlayer* TitlePlayer::Create(ObjModel* model, const Vector3& startPosition)
{
	//タイトルシーン用自機のインスタンスを生成
	TitlePlayer* titlePlayer = new TitlePlayer();
	if (titlePlayer == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!titlePlayer->Initialize(model, startPosition)) {
		delete titlePlayer;
		assert(0);
		return nullptr;
	}

	return titlePlayer;
}

bool TitlePlayer::Initialize(ObjModel* model, const Vector3& startPosition)
{
	//オブジェクトの初期化
	if (!ObjObject3d::Initialize()) {
		return false;
	}

	//モデルをセット
	assert(model);
	this->model = model;

	//座標をセット
	position = startPosition;

	//飛行機雲演出生成
	vaporEffect.reset(new PlayerVaporEffect());

	return true;
}

void TitlePlayer::Update()
{
	//出撃
	if (isSortie) {
		Sortie();
	}

	//オブジェクト更新
	ObjObject3d::Update();

	//両翼の座標更新
	UpdateWingPos();
}

void TitlePlayer::Sortie()
{
	//出撃行動
	(this->*sortieActionFuncTable[static_cast<size_t>(sortieModePhase)])();

	//自機のジェット噴射演出用パーティクル生成
	ParticleEmitter::GetInstance()->PlayerJet(matWorld);
}

void TitlePlayer::SortieStay()
{
	//発射するまでの時間
	const float sortieStartTime = 180;
	sortieTimer++;

	//タイマーが指定した時間になったら
	if (sortieTimer >= sortieStartTime) {
		//次のフェーズへ
		sortieModePhase = SortieModePhase::Boost;
		//タイマーを初期化
		sortieTimer = 0;

		//飛行機雲の生成を開始する
		vaporEffect->VaporStart();
	}
}

void TitlePlayer::SortieBoost()
{
	//タイマー更新
	sortieTimer++;

	//回転速度
	const float rotSpeed = 0.2f;
	//x軸回転
	rotation.x -= rotSpeed;
	const float rotLimit = -45.0f;
	rotation.x = max(rotation.x, rotLimit);

	//自機が傾いている角度に移動させる
	Vector3 moveSpeed = { 0, 5, 5 };
	moveSpeed.y = moveSpeed.y * (rotation.x / rotLimit);
	position += moveSpeed;

	//ポストエフェクトにラジアルブラーをかける
	const float blurStrengthMax = 0.6f; //ブラー最大の広がる強さ
	if (!GamePostEffect::GetPostEffect()->GetIsRadialBlur()) {
		GamePostEffect::GetPostEffect()->SetRadialBlur(true);

		//開始時のブラーの強さをセット
		GamePostEffect::GetPostEffect()->SetRadialBlurStrength(blurStrengthMax);
	}
	else {
		//だんだんブラーを弱くする時間
		const int blurChangeTime = 300;
		//既に弱くしたら抜ける
		if (sortieTimer > blurChangeTime) { return; }

		//ブラー最小の広がる強さ
		const float blurStrengthMin = 0.05f;

		//イージングで強さを変更
		const float time = (float)sortieTimer / blurChangeTime;
		const float blurStrength = Easing::OutQuad(blurStrengthMax, blurStrengthMin, time);
		GamePostEffect::GetPostEffect()->SetRadialBlurStrength(blurStrength);
	}
}

void TitlePlayer::UpdateWingPos()
{
	//自機の中心座標からの距離
	const Vector3 leftDistancePos = { -2.0f, -0.2f, -0.9f };
	const Vector3 rightDistancePos = { 2.0f, -0.2f, -0.9f };
	//左翼座標を取得
	const Vector3 leftWingPos = LocalTranslation(leftDistancePos, matWorld);
	//右翼座標を取得
	const Vector3 rightWingPos = LocalTranslation(rightDistancePos, matWorld);

	vaporEffect->Update(leftWingPos, rightWingPos);
}
