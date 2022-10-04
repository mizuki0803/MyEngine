#include "ParticleEmitter.h"

ParticleEmitter* ParticleEmitter::GetInstance()
{
	static ParticleEmitter instance;

	return &instance;
}

void ParticleEmitter::Initialize()
{
	//パーティクルマネージャー生成
	particleManager.reset(ParticleManager::Create(1));
}

void ParticleEmitter::Update()
{
	//パーティクルマネージャー更新
	particleManager->Update();
}

void ParticleEmitter::DrawAll()
{
	//パーティクルマネージャー描画
	particleManager->Draw();
}

void ParticleEmitter::DemoEffect()
{
	//色
	const XMFLOAT4 purple = { 1.0f, 0.1f, 1.0f, 1.0f }; //紫
	const XMFLOAT4 lightBlue = { 0.1f, 1.0f, 1.0f, 1.0f }; //水色

	for (int i = 0; i < 10; i++) {
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

		//追加
		particleManager->Add(life, pos, vel, acc, 1.0f, 0.0f, purple, lightBlue);
	}
}

void ParticleEmitter::DemoEffect2()
{
	//色
	const XMFLOAT4 red = { 1.0f, 0.1f, 0.1f, 1.0f }; //紫
	const XMFLOAT4 green = { 0.1f, 1.0f, 0.1f, 1.0f }; //水色

	for (int i = 0; i < 10; i++) {
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

		//追加
		particleManager->Add(life, pos, vel, acc, 1.0f, 0.0f, red, green);
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
		//追加
		particleManager->Add(5, pos, vel, acc, startScale, endScale, startColor, endColor);
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
		float endScale2 = 0;
		//追加
		particleManager->Add(5, pos, vel, acc, startScale, endScale2, startColor, endColor);
	}
}

void ParticleEmitter::ChargeShot(const Vector3& position, const float size)
{
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
		//追加
		particleManager->Add(2, position, vel, acc, startScale, endScale, startColor, endColor);
	}
}
