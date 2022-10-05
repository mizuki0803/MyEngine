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
	/// <param name="matWorld">自機ワールド行列</param>
	void PlayerJet(const XMMATRIX& matWorld);

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
	void ChargeShotDead(const Vector3& position);

	/// <summary>
	/// 全削除
	/// </summary>
	void AllDelete();

private: //メンバ変数
	//パーティクルマネージャー
	std::unique_ptr<ParticleManager> particleManager;
	//パーティクルマネージャー
	std::unique_ptr<ParticleManager> particleA;
};
