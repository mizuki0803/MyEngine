#include "PlayerReticles.h"
#include "SpriteTexture.h"

PlayerReticles* PlayerReticles::Create()
{
	//自機付属の2つのレティクル制御クラスのインスタンスを生成
	PlayerReticles* playerReticles = new PlayerReticles();
	if (playerReticles == nullptr) {
		return nullptr;
	}

	// 初期化
	if (!playerReticles->Initialize()) {
		delete playerReticles;
		assert(0);
		return nullptr;
	}

	return playerReticles;
}

bool PlayerReticles::Initialize()
{
	//レティクルを生成
	nearReticle.reset(PlayerNearReticle::Create(SpriteTexture::Reticle, 15.0f, { 100, 100 }));
	farReticle.reset(PlayerFarReticle::Create(SpriteTexture::Reticle, 25.0f, { 50, 50 }, { 100, 100 }));

	return true;
}

void PlayerReticles::Update(const XMMATRIX& matWorld, const XMMATRIX& matView, const XMMATRIX& matProjection)
{
	//レティクル更新
	nearReticle->Update(matWorld, matView, matProjection);
	farReticle->Update(matWorld, matView, matProjection);
}

void PlayerReticles::Draw()
{
	//レティクル描画
	nearReticle->Draw();
	farReticle->Draw();
}

void PlayerReticles::ChargeModeStart()
{
	//レティクルをチャージ状態にする
	nearReticle->GetReticle2D()->ChargeModeStart();
	farReticle->GetReticle2D()->ChargeModeStart();
}

void PlayerReticles::ChargeModeEnd()
{
	nearReticle->GetReticle2D()->ChargeModeEnd();
	farReticle->GetReticle2D()->ChargeModeEnd();
}

void PlayerReticles::LockonEnemy(Enemy* enemy)
{
	//レティクルのチャージ状態を終了させる
	ChargeModeEnd();

	//敵をロックオンする
	farReticle->GetReticle2D()->LockonEnemy(enemy);
}

void PlayerReticles::UnlockonEnemy()
{
	//ロックオンを解除
	farReticle->GetReticle2D()->UnlockonEnemy();
}
