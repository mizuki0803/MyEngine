#pragma once
#include "Object3d.h"

/// <summary>
/// 落下球
/// </summary>
class FallSphere 
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
	/// 落下球を作成
	/// </summary>
	/// <param name="model">使用するモデル</param>
	/// <returns>落下球</returns>
	static FallSphere* Create(Model* model);

public:	
	/// <summary>
	/// デストラクタ
	/// </summary>
	~FallSphere();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">使用するモデル</param>
	/// <returns>成否</returns>
	bool Initialize(Model* model);

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();
	
	//getter
	XMFLOAT3 GetPosition() { return fallSphereObject->GetPosition(); };

private:
	/// <summary>
	/// リセット
	/// </summary>
	void Reset();

private:
	//落下球オブジェクト
	Object3d* fallSphereObject = nullptr;
	//初期地点
	const XMFLOAT3 startPos = { 0, 1900, 0 };
	//重力
	const float gravity = 9.8f / 60;
	//速度
	float velY = 0;
	//地面に着いたか
	bool isOnGround = false;
};