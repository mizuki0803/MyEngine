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
}

void ParticleEmitter::Update()
{
	//パーティクルマネージャー更新
	circleParticle->Update();
	explosionParticle->Update();
}

void ParticleEmitter::DrawAll()
{
	//パーティクルマネージャー描画
	circleParticle->Draw();
	explosionParticle->Draw();
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
		const float md_pos = 10.0f;
		Vector3 pos{};
		pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		//X,Y,Z全て[-0.05f, +0.05f]でランダムに分布
		const float md_vel = 0.1f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//重力に見立ててYのみ[-0.001f, 0]でランダムに分布
		Vector3 acc{};
		const float md_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * md_acc;

		//大きさ変更のイージング
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1 , std::placeholders::_2, std::placeholders::_3);

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
		const float md_pos = 10.0f;
		Vector3 pos{};
		pos.x = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.y = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.z = (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		//X,Y,Z全て[-0.05f, +0.05f]でランダムに分布
		const float md_vel = 0.1f;
		Vector3 vel{};
		vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
		//重力に見立ててYのみ[-0.001f, 0]でランダムに分布
		Vector3 acc{};
		const float md_acc = 0.001f;
		acc.y = -(float)rand() / RAND_MAX * md_acc;

		//大きさ変更のイージング
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//追加
		explosionParticle->Add(life, pos, vel, acc, 1.0f, 0.0f, lerp, red, green);
	}
}

void ParticleEmitter::PlayerJet(const XMMATRIX& playerMatWorld)
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
		vel.z = (float)rand() / RAND_MAX * -0.1f - 0.1f;
		Vector3 acc{};
		const float md_acc = 0.005f;
		acc.z = -(float)rand() / RAND_MAX * md_acc;
		float startScale = (float)rand() / RAND_MAX * 0.1f + 0.5f;
		float endScale = 0;
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
		vel.z = (float)rand() / RAND_MAX * -0.1f - 0.1f;
		Vector3 acc{};
		const float md_acc = 0.008f;
		acc.z = -(float)rand() / RAND_MAX * md_acc;
		float startScale = (float)rand() / RAND_MAX * 0.2f + 2.0f;
		float endScale = 0;
		//大きさ変更のイージング
		std::function<float(const float, const float, const float) > lerp =
			std::bind(&Easing::LerpFloat, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
		//追加
		circleParticle->Add(5, pos, vel, acc, startScale, endScale, lerp,startColor, endColor);
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
		const float md_pos = 0.5f;
		Vector3 pos = position;
		pos.x += (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.z += (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		//大きさをランダムに分布
		const float md_scale = 2.0f;
		const float startScale = (float)rand() / RAND_MAX * md_scale + (size * 2);
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
		const float md_scale = 2.8f;
		float startScale = (float)rand() / RAND_MAX * md_scale + (size * 2 / 7) - md_scale / 2;
		float endScale = (float)rand() / RAND_MAX * md_scale + (size * 2) - md_scale / 2;
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
		const float md_scale = 10.0f;
		const float scale = size * 3 / 5;
		float startScale = 0;
		float endScale = (float)rand() / RAND_MAX * md_scale + (size * 2);
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
		const float md_scale = 10.0f;
		float startScale = 0;
		float endScale = (float)rand() / RAND_MAX * md_scale + (size * 2);
		//大きさ変更のイージング
		std::function<float(const float, const float, const float) > outQuad =
			std::bind(&Easing::OutCubic, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);

		//追加
		circleParticle->Add(life, position, vel, acc, startScale, endScale, outQuad, startColor, endColor);
	}
}

void ParticleEmitter::Explosion(const Vector3& position)
{
	for (int j = 0; j < 5; j++) {
		//X,Y,Z全て[-5.0f, +5.0f]でランダムに分布
		const float md_pos = 2.0f;
		Vector3 pos = position;
		pos.x += (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.y += (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;
		pos.z += (float)rand() / RAND_MAX * md_pos - md_pos / 2.0f;

		for (int i = 0; i < 25; i++) {
			//生存時間
			int life = (rand() % 30) + 10;

			//X,Y,Z全て[-5.0f, +5.0f]でランダムに分布
			const float md_pos2 = 0.8f;
			pos.x += (float)rand() / RAND_MAX * md_pos2 - md_pos2 / 2.0f;
			pos.y += (float)rand() / RAND_MAX * md_pos2 - md_pos2 / 2.0f;
			pos.z += (float)rand() / RAND_MAX * md_pos2 - md_pos2 / 2.0f;

			//X,Y,Z全て[-0.05f, +0.05f]でランダムに分布
			const float md_vel = 0.05f;
			Vector3 vel{};
			vel.x = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			vel.y = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			vel.z = (float)rand() / RAND_MAX * md_vel - md_vel / 2.0f;
			Vector3 acc{};

			const float md_scale = 3.0f;
			float startScale = 0.3f;
			float endScale = 4.0f + (float)rand() / RAND_MAX * md_scale - md_scale / 2.0f;
			//大きさ変更のイージング
			std::function<float(const float, const float, const float) > outQuart =
				std::bind(&Easing::OutQuart, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3);
			//色
			const float md_green = 0.2f;
			const float green = 0.2f + (float)rand() / RAND_MAX * md_green;
			const XMFLOAT4 startColor = { 0.9f, green, 0.1f, 1.0f }; //濃い緑
			const XMFLOAT4 endColor = { 0, 0, 0, 1.0f }; //無色

			//追加
			explosionParticle->Add(life, pos, vel, acc, startScale, endScale, outQuart, startColor, endColor);
		}
	}
}

void ParticleEmitter::BlackSmoke(const Vector3& position)
{
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
}
