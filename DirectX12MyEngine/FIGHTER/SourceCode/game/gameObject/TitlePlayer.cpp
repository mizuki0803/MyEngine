#include "TitlePlayer.h"
#include "ParticleEmitter.h"
#include "Easing.h"
#include "GamePostEffect.h"

TitlePlayer* TitlePlayer::Create(ObjModel* model, const Vector3& startPosition)
{
	//タイトルシーン用自機のインスタンスを生成
	TitlePlayer* titlePlayer = new TitlePlayer();
	if (titlePlayer == nullptr) {
		return nullptr;
	}

	//モデルをセット
	assert(model);
	titlePlayer->model = model;

	// 初期化
	if (!titlePlayer->Initialize()) {
		delete titlePlayer;
		assert(0);
		return nullptr;
	}

	//座標をセット
	titlePlayer->position = startPosition;

	return titlePlayer;
}

void TitlePlayer::Update()
{
	//出撃
	if (isSortie) {
		Sortie();
	}

	//オブジェクト更新
	ObjObject3d::Update();
}

void TitlePlayer::Sortie()
{
	//発射するまでの時間
	const float sortieStartTime = 180;
	sortieStartTimer++;

	//自機のジェット噴射演出用パーティクル生成
	ParticleEmitter::GetInstance()->PlayerJet(matWorld);

	if (sortieStartTimer <= sortieStartTime) { return; }

	//回転速度
	const float rotSpeed = 0.2f;
	//x軸回転
	rotation.x -= rotSpeed;
	const float rotLimit = -45.0f;
	rotation.x = max(rotation.x, rotLimit);

	//自機が傾いている角度に移動させる
	Vector3 move = { 0, 0, 2.5f };
	const float moveSpeed = 2.5f;
	move.y = moveSpeed * (rotation.x / rotLimit);
	position += move;

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
		if ((sortieStartTimer - sortieStartTime) > blurChangeTime) { return; }

		//ブラー最小の広がる強さ
		const float blurStrengthMin = 0.05f;

		//イージングで強さを変更
		const float time = (float)(sortieStartTimer - sortieStartTime) / blurChangeTime;
		const float blurStrength = Easing::OutQuad(blurStrengthMax, blurStrengthMin, time);
		GamePostEffect::GetPostEffect()->SetRadialBlurStrength(blurStrength);
	}
}
