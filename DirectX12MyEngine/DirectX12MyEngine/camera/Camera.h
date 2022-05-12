#pragma once
#include <DirectXMath.h>

using namespace DirectX;

/// <summary>
/// カメラ
/// </summary>
class Camera
{
private: // エイリアス
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	/// <summary>
	/// カメラ初期化
	/// </summary>
	void Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// カメラアングルを更新
	/// </summary>
	void UpdateAngle();

	/// <summary>
	/// ビュー行列を更新
	/// </summary>
	void UpdateMatView();

	/// <summary>
	/// 射影行列を更新
	/// </summary>
	void UpdateMatProjection();

	/// <summary>
	/// ベクトルによる移動
	/// </summary>
	/// <param name="move">移動量</param>
	void MoveVector(const XMFLOAT3& move);

	/// <summary>
	/// ベクトルによる視点移動
	/// </summary>
	/// <param name="move">移動量</param>
	void MoveEyeVector(const XMFLOAT3& move);


	//getter
	const XMFLOAT3& GetEye() { return eye; }
	const XMFLOAT3& GetTarget() { return target; }
	const XMMATRIX& GetMatView() { return matView; }
	const XMMATRIX& GetMatProjection() { return matProjection; }
	const XMMATRIX& GetMatBillboard() { return matBillboard; }
	const XMMATRIX& GetMatBillboardY() { return matBillboardY; }
	const XMFLOAT3& GetAngle() { return angle; }
	const float GetDistance() { return distance; }

	//setter
	void SetEye(const XMFLOAT3& eye) { this->eye = eye; dirtyView = true; }
	void SetTarget(const XMFLOAT3& target) { this->target = target; dirtyView = true; }
	void SetAngle(const XMFLOAT3& angle) { this->angle = angle; dirtyAngle = true; }
	void SetDistance(const float distance) { this->distance = distance; dirtyAngle = true; }

private:
	// ビュー行列
	XMMATRIX matView = XMMatrixIdentity();
	// 射影行列
	XMMATRIX matProjection = XMMatrixIdentity();
	//ビルボード行列
	XMMATRIX matBillboard = XMMatrixIdentity();
	//Y軸回りのビルボード行列
	XMMATRIX matBillboardY = XMMatrixIdentity();
	// 視点座標
	XMFLOAT3 eye = { 0, 1, -10.0f };
	// 注視点座標
	XMFLOAT3 target = { 0, 0, 0 };
	// 上方向ベクトル
	XMFLOAT3 up = { 0, 1, 0 };
	//カメラの回転角
	XMFLOAT3 angle = { 0.0f, 0.0f, 0.0f };
	//targetからカメラまでの距離
	float distance = 10.0f;
	//ダーティフラグ
	bool dirtyAngle = false;
	bool dirtyView = false;
	bool dirtyProjection = false;
};