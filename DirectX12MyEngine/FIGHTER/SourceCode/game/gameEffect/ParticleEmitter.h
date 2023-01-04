#pragma once
#include "ParticleManager.h"

/// <summary>
/// パーティクルエミッター
/// </summary>
class ParticleEmitter final
{
private: //シングルトン化
	//コンストラクタを隠蔽
	ParticleEmitter() = default;
	//デストラクタを隠蔽
	~ParticleEmitter() = default;
public:
	//コピーコンストラクタを無効化
	ParticleEmitter(const ParticleEmitter& particleEmitter) = delete;
	//代入演算子を無効化
	void operator = (const ParticleEmitter& particleEmitter) = delete;

public: //メンバ関数
	/// <summary>
	/// インスタンス取得
	/// </summary>
	/// <returns>デバッグテキスト</returns>
	static ParticleEmitter* GetInstance();

	/// <summary>
	/// 初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 一斉描画
	/// </summary>
	void DrawAll();

	/// <summary>
	/// お試し用演出
	/// </summary>
	void DemoEffect();

	/// <summary>
	/// お試し用演出
	/// </summary>
	void DemoEffect2();

	/// <summary>
	/// 自機のジェット噴射演出
	/// </summary>
	/// <param name="playerMatWorld">自機ワールド行列</param>
	/// <param name="playerSpeedPhase">自機移動速度フェーズ</param>
	void PlayerJet(const XMMATRIX& playerMatWorld, const int playerSpeedPhase = 0);

	/// <summary>
	/// 自機の死亡ジェット黒煙噴射演出
	/// </summary>
	/// <param name="playerMatWorld">自機ワールド行列</param>
	void PlayerBlackSmokeJet(const XMMATRIX& playerMatWorld);

	/// <summary>
	/// 自機の翼から出る飛行機雲(ヴェイパー)
	/// </summary>
	/// <param name="position">パーティクル生成座標</param>
	/// <param name="size">パーティクル大きさ</param>
	/// <param name="velocity">パーティクル移動速度</param>
	void PlayerVapor(const Vector3& position, const float size, const Vector3& velocity = {});

	/// <summary>
	/// ショット演出
	/// </summary>
	/// <param name="position">パーティクル生成座標</param>
	void Shot(const Vector3& position);

	/// <summary>
	/// ショット死亡演出
	/// </summary>
	/// <param name="position">パーティクル生成座標</param>
	void ShotDead(const Vector3& position, const float size);

	/// <summary>
	/// チャージショット演出
	/// </summary>
	/// <param name="position">パーティクル生成座標</param>
	/// <param name="size">大きさ</param>
	void ChargeShot(const Vector3& position, const float size);

	/// <summary>
	/// チャージショット死亡演出
	/// </summary>
	/// <param name="position">パーティクル生成座標</param>
	void ChargeShotDead(const Vector3& position, const float size);

	/// <summary>
	/// ボスチャージ演出
	/// </summary>
	/// <param name="position">パーティクル生成座標</param>
	/// <param name="size">大きさ</param>
	void BossCharge(const Vector3& position, const float size);

	/// <summary>
	/// ボスチャージショット演出
	/// </summary>
	/// <param name="position">パーティクル生成座標</param>
	/// <param name="size">大きさ</param>
	void BossChargeShot(const Vector3& position, const float size);

	/// <summary>
	/// ボスチャージショット死亡演出
	/// </summary>
	/// <param name="position">パーティクル生成座標</param>
	void BossChargeShotDead(const Vector3& position, const float size);

	/// <summary>
	/// 爆発演出
	/// </summary>
	/// <param name="position">パーティクル生成座標</param>
	void Explosion(const Vector3& position, const float size = 1, const int time = 10);

	/// <summary>
	/// ボス死亡専用爆発演出
	/// </summary>
	/// <param name="position">パーティクル生成座標</param>
	void BossDeadExplosion(const Vector3& position);

	/// <summary>
	/// 黒煙演出
	/// </summary>
	/// <param name="position">パーティクル生成座標</param>
	/// <param name="size">パーティクル大きさ</param>
	/// <param name="time">生存時間</param>
	void BlackSmoke(const Vector3& position, const float size, const int time = 30);

	/// <summary>
	/// アイテムのキラキラ
	/// </summary>
	/// <param name="position">パーティクル生成座標</param>
	/// <param name="size">パーティクル大きさ</param>
	/// <param name="size">一度に出す数</param>
	void ItemShine(const Vector3& position, const float size, const int num);

	/// <summary>
	/// 全削除
	/// </summary>
	void AllDelete();

private: //メンバ関数
	/// <summary>
	/// テクスチャ読み込み
	/// </summary>
	void LoadTexture();

private: //メンバ変数
	//パーティクルマネージャー(丸)
	std::unique_ptr<ParticleManager> circleParticle;
	//パーティクルマネージャー(爆発)
	std::unique_ptr<ParticleManager> explosionParticle;
	//パーティクルマネージャー(黒煙)
	std::unique_ptr<ParticleManager> blackSmokeParticle;
	//パーティクルマネージャー(キラキラ)
	std::unique_ptr<ParticleManager> shineParticle;
};
