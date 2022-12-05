#pragma once
#include "Texture.h"
#include <wrl.h>
#include <d3d12.h>

/// <summary>
/// SRV用デスクリプタヒープ
/// </summary>
class DescHeapSRV
{
private: //エイリアス
	//Microsoft::WRL::を省略
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

public:
	//テクスチャの最大枚数
	static const int SRVCount = 512;

public: //静的メンバ関数
	/// <summary>
	/// 共通初期化設定
	/// </summary>
	static void Initialize(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// SRV共通SetDescriptorHeaps
	/// </summary>
	/// <param name="cmdList">コマンドリスト</param>
	static void SetDescriptorHeaps();

	/// <summary>
	/// SRV共通CreateShaderResourceView
	/// </summary>
	static void CreateShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC& srvDesc, Texture& texture);

	/// <summary>
	/// SRV共通SetGraphicsRootDescriptorTable
	/// </summary>
	static void SetGraphicsRootDescriptorTable(UINT rootParameterIndex, UINT texNumber);

public: //静的メンバ変数  (たくさん使うので、使いやすいようにpublicにしておく)
	//デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeapSRV;

private: //静的メンバ変数
	//デバイス
	static ID3D12Device* dev;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//テクスチャ番号カウンター
	static UINT texNumCount;
};