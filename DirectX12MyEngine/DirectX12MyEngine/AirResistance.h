#pragma once
#include "Object3d.h"

class AirResistance 
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
	/// 空気抵抗球生成
	/// </summary>
	/// <param name="model">モデル</param>
	/// <returns>空気抵抗球</returns>
	static AirResistance* Create(Model* model = nullptr);

public:
	/// <summary>
	/// デストラクタ
	/// </summary>
	~AirResistance();

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
	/// 砲台
	/// </summary>
	void Turret();

	/// <summary>
	/// リセット
	/// </summary>
	void Reset();


private:
	//球オブジェクト
	Object3d* sphereObject = nullptr;
	//初期位置
	const XMFLOAT3 initPos = { -900, 0, 0 };
	//初期速度
	const XMFLOAT2 initVel = { 13, 13 };
	//速度
	XMFLOAT2 vel = {};
	//加速度
	XMFLOAT2 accel = {};
	//質量(kg)
	const float mass = 1;
	//地面についたか
	bool isOnGround = false;
};