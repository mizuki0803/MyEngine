#pragma once
#include "Object3d.h"

/// <summary>
/// 砲台
/// </summary>
class Turret
{
private: // エイリアス
// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	/// <summary>
	/// 砲台を作成
	/// </summary>
	/// <param name="model">使用するモデル</param>
	/// <returns>砲台</returns>
	static Turret* Create(Model* turretModel, Model* bulletModel);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~Turret();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">使用するモデル</param>
	/// <returns>成否</returns>
	bool Initialize(Model* turretModel, Model* bulletModel);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//getter
	XMFLOAT3 GetBulletPosition() { return bulletObject->GetPosition(); };

private:
	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

private:
	//砲台オブジェクト
	Object3d* turretObject = nullptr;
	//弾オブジェクト
	Object3d* bulletObject = nullptr;
	//発射地点
	const XMFLOAT3 startPos = { -1000, 10, 0 };
	//重力
	const float gravity = 9.8f / 60;
	//速度
	XMFLOAT2 vel = { 10, -10 };
	//地面に着いたか
	bool isOnGround = false;
};