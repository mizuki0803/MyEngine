#pragma once
#include <DirectXMath.h>
#include "PipelineSet.h"

/// <summary>
/// シャドウマップ
/// </summary>
class ShadowMap
{
private: // エイリアス
	// Microsoft::WRL::を省略
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::を省略
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: //サブクラス
	struct VertexPosUv
	{
		XMFLOAT3 pos;	//xyz座標
		XMFLOAT2 uv;	//uv座標
	};

	//定数バッファ用データ構造体
	struct ConstBufferData
	{
		XMFLOAT4 color;	//色(RGBA)
		XMMATRIX mat;	//3変換行列
	};

public:
	/// <summary>
	/// シャドウマップ生成
	/// </summary>
	/// <returns>Sprite</returns>
	static ShadowMap* Create(const XMFLOAT2& size = { 1.0f, 1.0f }, const XMFLOAT2& center = { 0, 0 });

	/// <summary>
	/// シャドウマップ共通部分の初期化
	/// </summary>
	/// <param name="dev">デバイス</param>
	/// <param name="cmdList">コマンドリスト</param>
	static void ShadowMapCommon(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList);

public:
	/// <summary>
	/// 初期化
	/// </summary>
	bool Initialize(const XMFLOAT2& size, const XMFLOAT2& center);

	/// <summary>
	/// 描画
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void Draw();

	/// <summary>
	/// シーン描画前処理
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void DrawScenePrev();

	/// <summary>
	/// シーン描画後処理
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void DrawSceneRear();

	/// <summary>
	/// シーン読み取り前処理
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void ReadScenePrev();

	/// <summary>
	/// シーン読み取り後処理
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void ReadSceneRear();

	/// <summary>
	/// シーン読み取り前処理
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void DrawSceneReadPrev();

	/// <summary>
	/// シーン読み取り後処理
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	void DrawSceneReadRear();

	/// <summary>
	/// パイプライン生成
	/// </summary>
	void CreateGraphicsPipelineState();

	//getter
	ID3D12Resource* GetDepthBuff() { return depthBuff.Get(); }
	ID3D12DescriptorHeap* GetDescHeapDSV() { return descHeapDSV.Get(); }
	ID3D12DescriptorHeap* GetDescHeapSRV() { return descHeapSRV.Get(); }

private:
	//画面クリアカラー
	static const float clearColor[4];
	//デバイス
	static ID3D12Device* dev;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//パイプラインセット
	static PipelineSet pipelineSet;
private:
	//頂点バッファ
	ComPtr<ID3D12Resource> vertBuff;
	//頂点バッファビュー
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//定数バッファ
	ComPtr<ID3D12Resource> constBuff;
	//テクスチャバッファ
	ComPtr<ID3D12Resource> texBuff;
	//SRV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	//深度バッファ
	ComPtr<ID3D12Resource> depthBuff;
	//RTV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV用デスクリプタヒープ
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	//色(RGBA)
	XMFLOAT4 color = { 1, 1, 1, 1 };
};

