#include "ParticleEmitter.h"
#include "Easing.h"
#include <functional>
#include <d3dx12.h>

ParticleEmitter* ParticleEmitter::GetInstance()
{
	static ParticleEmitter instance;

	return &instance;
}

void ParticleEmitter::Initialize()
{
	//テクスチャ全読み込み
	LoadTexture();

	//パーティクルマネージャー生成
	circleParticle.reset(ParticleManager::Create(1));
	explosionParticle.reset(ParticleManager::Create(2));
	blackSmokeParticle.reset(ParticleManager::Create(2));
	shineParticle.reset(ParticleManager::Create(3));
}

void ParticleEmitter::Update()
{
	//パーティクルマネージャー更新
	circleParticle->Update();
	explosionParticle->Update();
	blackSmokeParticle->Update();
	shineParticle->Update();
}

void ParticleEmitter::DrawAll()
{
	//加算合成描画前処理
	ParticleManager::DrawPrevAddBlend();

	//パーティクルマネージャー描画
	circleParticle->Draw();
	explosionParticle->Draw();
	shineParticle->Draw();

	//減算合成描画前処理
	ParticleManager::DrawPrevSubBlend();

	//パーティクルマネージャー描画
	blackSmokeParticle->Draw();
}

void ParticleEmitter::DemoEffect()
{
	//色
	const XMFLOAT4 purple = { 1.0f, 0.1f, 1.0f, 1.0f }; //紫
	const XMFLOAT4 lightBlue = { 0.1f, 1.0f, 1.0f, 1.0f }; //水色

	for (int i = 0; i < 100; i++) {
		int life = 0;
		life = (rand() % 200) + 10;
		//X,Y,Z全て[-5.0f, +5.0f]でランダムに分布
		const float mdPos = 10.0f;
		Vector3 pos{};
		pos.x = (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		//X,Y,Z全て[-0.05f, +0.05f]でランダムに分布
		const float mdVel = 0.1f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * mdVel - mdVel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * mdVel - mdVel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * mdVel - mdVel / 2.0f;
		//重力に見立ててYのみ[-0.001f, 0]でランダムに分布
		Vector3 acc{};
		const float mdAcc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * mdAcc;

		//大きさ変更のイージング
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//追加
		circleParticle->Add(life, pos, vel, acc, 1.0f, 0.0f, lerp, purple, lightBlue);
	}
}

void ParticleEmitter::DemoEffect2()
{
	//色
	const XMFLOAT4 red = { 1.0f, 0.1f, 0.1f, 1.0f }; //紫
	const XMFLOAT4 green = { 0.1f, 1.0f, 0.1f, 1.0f }; //水色

	for (int i = 0; i < 100; i++) {
		int life = 0;
		life = (rand() % 200) + 10;
		//X,Y,Z全て[-5.0f, +5.0f]でランダムに分布
		const float mdPos = 10.0f;
		Vector3 pos{};
		pos.x = (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		//X,Y,Z全て[-0.05f, +0.05f]でランダムに分布
		const float mdVel = 0.1f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * mdVel - mdVel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * mdVel - mdVel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * mdVel - mdVel / 2.0f;
		//重力に見立ててYのみ[-0.001f, 0]でランダムに分布
		Vector3 acc{};
		const float mdAcc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * mdAcc;

		//大きさ変更のイージング
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//追加
		explosionParticle->Add(life, pos, vel, acc, 1.0f, 0.0f, lerp, red, green);
	}
}

void ParticleEmitter::PlayerJet(const XMMATRIX& playerMatWorld, const int playerSpeedPhase)
{
	//自機の中心座標からの距離
	const Vector3 distancePos = { 0, -0.25f, -1.2f };
	//平行移動行列の計算
	XMMATRIX matTrans = XMMatrixTranslation(distancePos.x, distancePos.y, distancePos.z);
	//ワールド行列の合成
	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();	//変形をリセット
	matWorld *= matTrans;	//ワールド行列に平行移動を反映
	//自機オブジェクトのワールド行列をかける
	matWorld *= playerMatWorld;
	//パーティクル生成座標を取得
	Vector3 pos = { matWorld.r[3].m128_f32[0], matWorld.r[3].m128_f32[1], matWorld.r[3].m128_f32[2] };

	//パーティクル(小)生成
	{
		//色
		const XMFLOAT4 startColor = { 0.6f, 0.6f, 0.6f, 1.0f }; //濃い白
		const XMFLOAT4 endColor = { 0.4f, 0.4f, 0.4f, 1.0f }; //薄い白
		Vector3 vel{};
		Vector3 acc{};
		float startScale = 0;
		float endScale = 0;

		//移動速度フェーズごとに調整
		//通常移動状態
		if (playerSpeedPhase == 0) {
			vel.z = (float)rand() / RAND_MAX * -0.1f - 0.1f;
			const float mdAcc = 0.005f;
			acc.z = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.1f + 0.5f;
		}
		//加速移動状態
		else if (playerSpeedPhase == 1) {
			vel.z = (float)rand() / RAND_MAX * -0.2f - 0.2f;
			const float mdAcc = 0.01f;
			acc.z = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.1f + 1.0f;
		}
		//減速移動状態
		else if (playerSpeedPhase == 2) {
			vel.z = (float)rand() / RAND_MAX * -0.001f - 0.001f;
			const float mdAcc = 0.001f;
			acc.z = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.05f + 0.25f;
		}
		//通常移動に戻す状態
		else if (playerSpeedPhase == 3) {
			vel.z = (float)rand() / RAND_MAX * -0.002f - 0.002f;
			const float mdAcc = 0.0025f;
			acc.z = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.05f + 0.3f;
		}

		//大きさ変更のイージング
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		//追加
		circleParticle->Add(5, pos, vel, acc, startScale, endScale, lerp, startColor, endColor);
	}

	//パーティクル(大)生成
	{
		//色
		const XMFLOAT4 startColor = { 1.0f, 0.21f, 0.21f, 1.0f }; //薄い赤
		const XMFLOAT4 endColor = { 0.3f, 0.01f, 0.01f, 1.0f }; //濃い赤
		Vector3 vel{};
		Vector3 acc{};
		float startScale = 0;
		float endScale = 0;

		//移動速度フェーズごとに調整
		//通常移動状態
		if (playerSpeedPhase == 0) {
			vel.z = (float)rand() / RAND_MAX * -0.1f - 0.1f;
			const float mdAcc = 0.008f;
			acc.z = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.2f + 2.0f;
		}
		//加速移動状態
		else if (playerSpeedPhase == 1) {
			vel.z = (float)rand() / RAND_MAX * -0.2f - 0.2f;
			const float mdAcc = 0.0016f;
			acc.z = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.2f + 4.0f;
		}
		//減速移動状態
		else if (playerSpeedPhase == 2) {
			vel.z = (float)rand() / RAND_MAX * -0.04f - 0.04f;
			const float mdAcc = 0.004f;
			acc.z = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.1f + 1.0f;
		}
		//通常移動に戻す状態
		else if (playerSpeedPhase == 3) {
			vel.z = (float)rand() / RAND_MAX * -0.06f - 0.06f;
			const float mdAcc = 0.005f;
			acc.z = -(float)rand() / RAND_MAX * mdAcc;
			startScale = (float)rand() / RAND_MAX * 0.15f + 1.2f;
		}

		//大きさ変更のイージング
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		//追加
		circleParticle->Add(5, pos, vel, acc, startScale, endScale, lerp, startColor, endColor);
	}
}

void ParticleEmitter::PlayerBlackSmokeJet(const XMMATRIX& playerMatWorld)
{
	//自機の中心座標からの距離
	const Vector3 distancePos = { 0, -0.25f, -1.2f };
	//平行移動行列の計算
	XMMATRIX matTrans = XMMatrixTranslation(distancePos.x, distancePos.y, distancePos.z);
	//ワールド行列の合成
	XMMATRIX matWorld;
	matWorld = XMMatrixIdentity();	//変形をリセット
	matWorld *= matTrans;	//ワールド行列に平行移動を反映
	//自機オブジェクトのワールド行列をかける
	matWorld *= playerMatWorld;
	//パーティクル生成座標を取得
	Vector3 pos = { matWorld.r[3].m128_f32[0], matWorld.r[3].m128_f32[1], matWorld.r[3].m128_f32[2] };

	//座標を元に黒煙エフェクトを作成
	for (int i = 0; i < 2; i++) {
		//生存時間
		int life = (rand() % 30) + 60;

		//X,Y,Zランダムに分布
		const Vector3 mdVel = { 0.01f, 0.05f, 0.01f };
		Vector3 vel{};
		vel.x = ((float)rand() / RAND_MAX * mdVel.x - mdVel.x / 2.0f);
		vel.y = ((float)rand() / RAND_MAX * mdVel.y);
		vel.z = ((float)rand() / RAND_MAX * mdVel.z - mdVel.z / 2.0f);
		Vector3 acc{};
		const float mdAcc = 0.001f;
		acc.y = (float)rand() / RAND_MAX * mdAcc;

		//大きさをセット
		const float mdScale = 2.5f;
		const float randScale = ((float)rand() / RAND_MAX * mdScale);
		const float startScale = randScale;
		const float endScale = randScale * 5.0f;
		//大きさ変更のイージング
		std::function<float(const float, const float, const float) > lerpFloat =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//色
		const XMFLOAT4 startColor = { 0.15f, 0.15f, 0.15f, 1 }; //薄い黒
		const XMFLOAT4 endColor = { 0.01f, 0.01f, 0.01f, 1 }; //うっすい黒

		//追加
		blackSmokeParticle->Add(life, pos, vel, acc, startScale, endScale, lerpFloat, startColor, endColor);
	}
}

void ParticleEmitter::Shot(const Vector3& position)
{
	//生存時間
	const int life = 3;
	//速度、加速度は0
	const Vector3 vel{};
	const Vector3 acc{};
	//大きさ変更のイージング
	std::function<float(const float, const float, const float) > lerp =
		std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
	//色
	const XMFLOAT4 green = { 0.1f, 0.3f, 0.1f, 1.0f }; //薄い緑
	for (int i = 0; i < 3; i++) {
		//大きさ
		const float scale = 2.5f;

		//追加
		circleParticle->Add(life, position, vel, acc, scale, scale, lerp, green, green);
	}

	//色
	const XMFLOAT4 white = { 0.1f, 0.1f, 0.1f, 1.0f }; //薄い白
	for (int i = 0; i < 3; i++) {
		//大きさ
		const float scale = 1.25f;

		//追加
		circleParticle->Add(life, position, vel, acc, scale, scale, lerp, white, white);
	}
}

void ParticleEmitter::ShotDead(const Vector3& position, const float size)
{
	//色
	const XMFLOAT4 startColor = { 0.2f, 0.3f, 0.4f, 1.0f }; //薄い青
	const XMFLOAT4 endColor = { 0, 0, 0, 1.0f }; //無色

	//速度、加速度は0
	Vector3 vel{};
	Vector3 acc{};

	for (int i = 0; i < 3; i++) {
		//生存時間
		const int life = (rand() % 10) + 15;
		//X,Y,Z全て[-5.0f, +5.0f]でランダムに分布
		const float mdPos = 0.5f;
		Vector3 pos = position;
		pos.x += (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		pos.z += (float)rand() / RAND_MAX * mdPos - mdPos / 2.0f;
		//大きさをランダムに分布
		const float mdScale = 2.0f;
		const float startScale = (float)rand() / RAND_MAX * mdScale + (size * 2);
		const float endScale = startScale;
		//大きさ変更のイージング
		std::function<float(const float, const float, const float) > outQuad =
			std::bind(&Easing::OutCubic, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//追加
		circleParticle->Add(life, pos, vel, acc, startScale, endScale, outQuad, startColor, endColor);
	}
}

void ParticleEmitter::ChargeShot(const Vector3& position, const float size)
{
	//生存時間
	const int life = 2;
	//色
	const XMFLOAT4 startColor = { 0.1f, 1.0f, 0.05f, 1.0f }; //濃い緑
	const XMFLOAT4 endColor = { 0.1f, 0.4f, 0.05f, 1.0f }; //薄い緑
	//速度、加速度は0
	Vector3 vel{};
	Vector3 acc{};

	for (int i = 0; i < 10; i++) {
		//大きさをランダムに分布
		const float mdScale = 2.8f;
		float startScale = (float)rand() / RAND_MAX * mdScale + (size * 2 / 7) - mdScale / 2;
		float endScale = (float)rand() / RAND_MAX * mdScale + (size * 2) - mdScale / 2;
		//大きさ変更のイージング
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//追加
		circleParticle->Add(life, position, vel, acc, startScale, endScale, lerp, startColor, endColor);
	}
}

void ParticleEmitter::ChargeShotDead(const Vector3& position, const float size)
{
	//色
	const XMFLOAT4 startColor = { 0.04f, 0.1f, 0.04f, 1.0f }; //濃い緑
	const XMFLOAT4 endColor = { 0, 0, 0, 1.0f }; //無色

	//速度、加速度は0
	Vector3 vel{};
	Vector3 acc{};

	for (int i = 0; i < 20; i++) {
		//生存時間
		int life = (rand() % 20) + 30;
		//大きさをランダムに分布
		const float mdScale = 10.0f;
		const float scale = size * 3 / 5;
		float startScale = 0;
		float endScale = (float)rand() / RAND_MAX * mdScale + (size * 2);
		//大きさ変更のイージング
		std::function<float(const float, const float, const float) > outQuad =
			std::bind(&Easing::OutCubic, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//追加
		circleParticle->Add(life, position, vel, acc, startScale, endScale, outQuad, startColor, endColor);
	}

	for (int i = 0; i < 10; i++) {
		//生存時間
		int life = (rand() % 20) + 60;
		//大きさをランダムに分布
		const float mdScale = 10.0f;
		float startScale = 0;
		float endScale = (float)rand() / RAND_MAX * mdScale + (size * 2);
		//大きさ変更のイージング
		std::function<float(const float, const float, const float) > outQuad =
			std::bind(&Easing::OutCubic, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//追加
		circleParticle->Add(life, position, vel, acc, startScale, endScale, outQuad, startColor, endColor);
	}
}

void ParticleEmitter::BossCharge(const Vector3& position, const float size)
{
	//生存時間
	const int life = 2;
	//色
	const XMFLOAT4 startColor = { 1.0f, 0.1f, 0.1f, 1.0f }; //濃い赤
	const XMFLOAT4 endColor = { 0.4f, 0.01f, 0.01f, 1.0f }; //薄い赤
	//速度、加速度は0
	Vector3 vel{};
	Vector3 acc{};

	for (int i = 0; i < 20; i++) {
		//大きさをランダムに分布
		const float mdScale = 1.5f;
		float startScale = (float)rand() / RAND_MAX * mdScale + (size * 2 / 7) - mdScale / 2;
		float endScale = (float)rand() / RAND_MAX * mdScale + (size * 2) - mdScale / 2;
		//大きさ変更のイージング
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//追加
		circleParticle->Add(life, position, vel, acc, startScale, endScale, lerp, startColor, endColor);
	}
}

void ParticleEmitter::BossChargeShot(const Vector3& position, const float size)
{
	//生存時間
	const int life = 2;
	//色
	const XMFLOAT4 startColor = { 1.0f, 0.1f, 0.1f, 1.0f }; //濃い赤
	const XMFLOAT4 endColor = { 0.4f, 0.01f, 0.01f, 1.0f }; //薄い赤
	//速度、加速度は0
	Vector3 vel{};
	Vector3 acc{};

	for (int i = 0; i < 20; i++) {
		//大きさをランダムに分布
		const float mdScale = 1.0f;
		float startScale = (float)rand() / RAND_MAX * mdScale + (size * 2 / 4) - mdScale / 2;
		float endScale = (float)rand() / RAND_MAX * mdScale + (size * 2) - mdScale / 2;
		//大きさ変更のイージング
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//追加
		circleParticle->Add(life, position, vel, acc, startScale, endScale, lerp, startColor, endColor);
	}
}

void ParticleEmitter::BossChargeShotDead(const Vector3& position, const float size)
{
	//色
	const XMFLOAT4 startColor = { 0.1f, 0.04f, 0.04f, 1.0f }; //濃い赤
	const XMFLOAT4 endColor = { 0, 0, 0, 1.0f }; //無色

	//速度、加速度は0
	Vector3 vel{};
	Vector3 acc{};

	for (int i = 0; i < 20; i++) {
		//生存時間
		int life = (rand() % 10) + 15;
		//大きさをランダムに分布
		const float mdScale = 10.0f;
		const float scale = size * 3 / 5;
		float startScale = 0;
		float endScale = (float)rand() / RAND_MAX * mdScale + (size * 2);
		//大きさ変更のイージング
		std::function<float(const float, const float, const float) > outQuad =
			std::bind(&Easing::OutCubic, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//追加
		circleParticle->Add(life, position, vel, acc, startScale, endScale, outQuad, startColor, endColor);
	}

	for (int i = 0; i < 10; i++) {
		//生存時間
		int life = (rand() % 20) + 60;
		//大きさをランダムに分布
		const float mdScale = 10.0f;
		float startScale = 0;
		float endScale = (float)rand() / RAND_MAX * mdScale + (size * 2);
		//大きさ変更のイージング
		std::function<float(const float, const float, const float) > outQuad =
			std::bind(&Easing::OutCubic, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//追加
		circleParticle->Add(life, position, vel, acc, startScale, endScale, outQuad, startColor, endColor);
	}
}

void ParticleEmitter::Explosion(const Vector3& position, const float size, const int time)
{
	for (int j = 0; j < 5; j++) {
		//X,Y,Z全て[-5.0f, +5.0f]でランダムに分布
		const float mdPos = 2.0f;
		Vector3 pos = position;
		pos.x += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f) * size;
		pos.y += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f) * size;
		pos.z += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f) * size;

		for (int i = 0; i < 25; i++) {
			//生存時間
			int life = (rand() % 30) + time;

			//X,Y,Z全て[-5.0f, +5.0f]でランダムに分布
			const float mdPos2 = 0.8f;
			pos.x += ((float)rand() / RAND_MAX * mdPos2 - mdPos2 / 2.0f) * size;
			pos.y += ((float)rand() / RAND_MAX * mdPos2 - mdPos2 / 2.0f) * size;
			pos.z += ((float)rand() / RAND_MAX * mdPos2 - mdPos2 / 2.0f) * size;

			//X,Y,Z全て[-0.05f, +0.05f]でランダムに分布
			const float mdVel = 0.05f;
			Vector3 vel{};
			vel.x = ((float)rand() / RAND_MAX * mdVel - mdVel / 2.0f) * size;
			vel.y = ((float)rand() / RAND_MAX * mdVel - mdVel / 2.0f) * size;
			vel.z = ((float)rand() / RAND_MAX * mdVel - mdVel / 2.0f) * size;
			Vector3 acc{};

			const float mdScale = 3.0f;
			float startScale = 0.3f * size;
			float endScale = (4.0f + (float)rand() / RAND_MAX * mdScale - mdScale / 2.0f) * size;
			//大きさ変更のイージング
			std::function<float(const float, const float, const float) > outQuart =
				std::bind(&Easing::OutQuart, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
			//色
			const float mdGreen = 0.2f;
			const float green = 0.2f + (float)rand() / RAND_MAX * mdGreen;
			const XMFLOAT4 startColor = { 0.9f, green, 0.1f, 1.0f }; //濃い緑
			const XMFLOAT4 endColor = { 0, 0, 0, 1.0f }; //無色

			//追加
			explosionParticle->Add(life, pos, vel, acc, startScale, endScale, outQuart, startColor, endColor);
		}
	}
}

void ParticleEmitter::BossDeadExplosion(const Vector3& position)
{
	for (int j = 0; j < 20; j++) {
		//X,Y,Z全て[-5.0f, +5.0f]でランダムに分布
		const float mdPos = 2.5f;
		Vector3 pos = position;
		pos.x += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f);
		pos.y += ((float)rand() / RAND_MAX * mdPos);
		pos.z += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f);

		for (int i = 0; i < 30; i++) {
			//生存時間
			int life = (rand() % 30) + 40;

			//X,Y,Z全て[-5.0f, +5.0f]でランダムに分布
			const float mdPos2 = 2.5f;
			pos.x += ((float)rand() / RAND_MAX * mdPos2 - mdPos2 / 2.0f);
			pos.z += ((float)rand() / RAND_MAX * mdPos2 - mdPos2 / 2.0f);

			//X,Y,Z全て[-0.25f, +0.25f]でランダムに分布
			const float mdVel = 0.75f;
			Vector3 vel{};
			vel.x = ((float)rand() / RAND_MAX * mdVel - mdVel / 2.0f);
			vel.y = ((float)rand() / RAND_MAX * mdVel);
			vel.z = ((float)rand() / RAND_MAX * mdVel - mdVel / 2.0f);
			Vector3 acc{};
			const float mdAcc = 0.01f;
			acc.y = -(float)rand() / RAND_MAX * mdAcc;

			const float mdScale = 5.0f;
			float startScale = 3.0f;
			float endScale = (20.0f + (float)rand() / RAND_MAX * mdScale - mdScale / 2.0f);
			//大きさ変更のイージング
			std::function<float(const float, const float, const float) > outQuart =
				std::bind(&Easing::OutQuart, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
			//色
			const float mdGreen = 0.2f;
			const float green = 0.2f + (float)rand() / RAND_MAX * mdGreen;
			const XMFLOAT4 startColor = { 0.9f, green, 0.1f, 1.0f }; //濃い緑
			const XMFLOAT4 endColor = { 0, 0, 0, 1.0f }; //無色

			//追加
			explosionParticle->Add(life, pos, vel, acc, startScale, endScale, outQuart, startColor, endColor);
		}
	}
}

void ParticleEmitter::BlackSmoke(const Vector3& position, const float size)
{
	for (int i = 0; i < 2; i++) {
		//生存時間
		int life = (rand() % 30) + 30;

		//X,Y,Zごとにでランダムに分布
		const float mdPos = 0.1f;
		Vector3 pos = position;
		pos.x += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f);
		pos.y += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f) + 1.0f;
		pos.z += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f);

		//X,Y,Z全て[-0.25f, +0.25f]でランダムに分布
		const Vector3 mdVel = { 0.01f, 0.05f, 0.01f };
		Vector3 vel{};
		vel.x = ((float)rand() / RAND_MAX * mdVel.x - mdVel.x / 2.0f);
		vel.y = ((float)rand() / RAND_MAX * mdVel.y);
		vel.z = ((float)rand() / RAND_MAX * mdVel.z - mdVel.z / 2.0f);
		Vector3 acc{};
		const float mdAcc = 0.003f;
		acc.y = (float)rand() / RAND_MAX * mdAcc;

		const float mdScale = 1.3f;
		const float randScale = ((float)rand() / RAND_MAX * mdScale);
		const float startScale = randScale * size;
		const float endScale = randScale * size * 2.0f;
		//大きさ変更のイージング
		std::function<float(const float, const float, const float) > lerpFloat =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//色
		const XMFLOAT4 startColor = { 0.15f, 0.15f, 0.15f, 1 }; //薄い黒
		const XMFLOAT4 endColor = { 0.01f, 0.01f, 0.01f, 1 }; //うっすい黒

		//追加
		blackSmokeParticle->Add(life, pos, vel, acc, startScale, endScale, lerpFloat, startColor, endColor);
	}
}

void ParticleEmitter::ItemShine(const Vector3& position, const float size, const int num)
{
	//生存時間
	for (int i = 0; i < num; i++) {
		int life = (rand() % 30) + 30;

		//X,Y,Zにでランダムに分布
		const float mdPos = 4.0f;
		Vector3 pos = position;
		pos.x += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f);
		pos.y += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f);
		pos.z += ((float)rand() / RAND_MAX * mdPos - mdPos / 2.0f);

		//移動しないので速度は変更なし
		Vector3 vel{};
		Vector3 acc{};
		const float mdScale = 2.0f * size;
		const float randScale = ((float)rand() / RAND_MAX * mdScale) + (1.0f * size);
		const float scale = randScale;
		//大きさ変更のイージング
		std::function<float(const float, const float, const float) > lerpFloat =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//色
		const XMFLOAT4 startColor = { 0.8f, 0.8f, 0.6f, 1 }; //黄色
		const XMFLOAT4 endColor = { 0.4f, 0.4f, 0.3f, 1 }; //薄い黄色

		//回転速度
		const float mdRotSpeed = 5.0f;
		const float baseSpeed = 10.0f * ((float)num / 2);
		const float rotSpeed = ((float)rand() / RAND_MAX * mdRotSpeed) + baseSpeed;

		//追加
		shineParticle->Add(life, pos, vel, acc, scale, scale, lerpFloat, startColor, endColor, rotSpeed);
	}
}

void ParticleEmitter::AllDelete()
{
	//全パーティクルの削除
	circleParticle->AllDelete();
	explosionParticle->AllDelete();
}

void ParticleEmitter::LoadTexture()
{
	//テクスチャ全読み込み
	ParticleManager::LoadTexture(1, "effect1.png");
	ParticleManager::LoadTexture(2, "effect2.png");
	ParticleManager::LoadTexture(3, "effect3.png");
}
