#pragma once
#include "Object3d.h"

class FrictionSphere
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
	/// 摩擦球生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>摩擦球</returns>
	static FrictionSphere* Create(Model* model = nullptr);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~FrictionSphere();

	/// <summary>
	/// 初期化
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>成否</returns>
	bool Initialize(Model* model);

	/// <summary>
	/// 毎フレーム処理
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//getter
	XMFLOAT3 GetPosition() { return sphereObject->GetPosition(); }

private:
	/// <summary>
	/// 摩擦をかける
	/// </summary>
	void Friction();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();


private:
	//球オブジェクト
	Object3d* sphereObject = nullptr;
	//初期位置
	const XMFLOAT3 initPos = { -800, 0, 0 };
	//初期速度
	const float initVel = 10;
	//速度
	float vel = 0;
	//質量(kg)
	const float mass = 100;
	//摩擦係数
	const float frictionCoefficient = 0.5f;
};