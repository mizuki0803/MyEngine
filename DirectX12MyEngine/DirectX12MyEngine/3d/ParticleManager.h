#pragma once
#include <Windows.h>
#include "DirectXBase.h"
#include <DirectXMath.h>
#include <forward_list>
#include "Camera.h"
#include "PipelineSet.h"

/// <summary>
/// パーティクルマネージャー
/// </summary>
class ParticleManager
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
	//頂点データ構造体
	struct VertexPos
	{
		XMFLOAT3 pos;	//xyz座標
		float scale;	//スケール
		XMFLOAT4 color;	//色
	};

	//定数バッファ用データ構造体B0
	struct ConstBufferData
	{
		XMMATRIX mat;	//ビュープロジェクション行列
		XMMATRIX matBillboad;	//ビルボード行列
	};

	//パーティクル1粒
	struct Particle
	{
		//DirectX::を省略
		using XMFLOAT3 = DirectX::XMFLOAT3;

		//座標
		XMFLOAT3 position = {};
		//速度
		XMFLOAT3 velocity = {};
		//加速度
		XMFLOAT3 accel = {};
		//現在フレーム
		int frame = 0;
		//終了フレーム
		int num_frame = 0;
		//スケール
		float scale = 1.0f;
		//初期値
		float s_scale = 1.0f;
		//最終値
		float e_scale = 0.0f;
		//色
		XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		//色初期値
		XMFLOAT4 s_color = { 1.0f, 1.0f, 1.0f, 1.0f };
		//色終了値
		XMFLOAT4 e_color = { 0.0f, 0.0f, 0.0f, 0.0f };
	};

public:
	/// <summary>
	/// パーティクル共通部分の初期化
	/// </summary>
	/// <param name="dev">デバイス</param>
	/// <param name="cmdList">コマンドリスト</param>
	static void ParticleManagerCommon(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// パイプライン生成
	/// </summary>
	static void CreatePipeline();

	/// <summary>
	/// デスクリプタヒープ生成
	/// </summary>
	static void CreateDescHeap();

	/// <summary>
	/// 描画前処理
	/// </summary>
	static void DrawPrev();

	/// <summary>
	/// 生成処理
	/// </summary>
	/// <returns>ParticleManager</returns>
	static ParticleManager* Create();

	/// <summary>
	/// パーティクルの形生成
	/// </summary>
	static void CreateModel();

	/// <summary>
	/// 画像読み込み
	/// </summary>
	/// <returns>成否</returns>
	static bool LoadTexture();

	/// <summary>
	/// パーティクルの情報をセット
	/// </summary>
	/// <param name="life">寿命</param>
	/// <param name="position">座標</param>
	/// <param name="velocity">速度</param>
	/// <param name="accel">加速度</param>
	/// <param name="start_scale">スケール初期値</param>
	/// <param name="end_scale">スケール終了値</param>
	/// <param name="start_color">色初期値</param>
	/// <param name="end_color">色終了値</param>
	void Add(const int life, const XMFLOAT3& position, const XMFLOAT3& velocity, const XMFLOAT3& accel,
		const float start_scale, const float end_scale, const XMFLOAT4& start_color, const XMFLOAT4& end_color);

	/// <summary>
	/// 初期化
	/// </summary>
	/// <returns>成否</returns>
	bool Initialize();

	/// <summary>
	/// 更新
	/// </summary>
	void Update();

	/// <summary>
	/// 描画
	/// </summary>
	void Draw();

	//setter
	static void SetCamera(Camera* camera) { ParticleManager::camera = camera; }

private:
	//デバイス
	static ID3D12Device* dev;
	//デスクリプタ1つ分のサイズ
	static UINT descHandleIncrementSize;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//パイプラインセット
	static PipelineSet pipelineSet;
	//テクスチャ用デスクリプタヒープの生成
	static ComPtr<ID3D12DescriptorHeap> descHeap;
	//テクスチャリソース(テクスチャバッファ)の配列
	static ComPtr<ID3D12Resource> texBuff;
	//デスクリプタヒープの先頭ハンドルを取得
	static CD3DX12_CPU_DESCRIPTOR_HANDLE cpuDescHandleSRV;
	static CD3DX12_GPU_DESCRIPTOR_HANDLE gpuDescHandleSRV;
	//頂点数
	static const int vertexCount = 1024;
	//頂点データ配列
	static VertexPos vertices[vertexCount];
	//カメラ
	static Camera* camera;

private:
	////頂点バッファの生成
	static ComPtr<ID3D12Resource> vertBuff;
	////頂点バッファビューの作成
	static D3D12_VERTEX_BUFFER_VIEW vbView;
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//パーティクル配列
	std::forward_list<Particle> particles;
};