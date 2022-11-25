#include "TitlePlayer.h"
#include "ParticleEmitter.h"

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
	const float sortieStartTime = 180;
	if (sortieStartTimer <= sortieStartTime) {
		sortieStartTimer++;
	}
	else {
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

	}

	//自機のジェット噴射演出用パーティクル生成
	ParticleEmitter::GetInstance()->PlayerJet(matWorld);
}
