#pragma once
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

private:
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
	static void CreateShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC& srvDesc, ID3D12Resource* texBuff);

	/// <summary>
	/// SRV共通SetGraphicsRootDescriptorTable
	/// </summary>
	static void SetGraphicsRootDescriptorTable(UINT rootParameterIndex, UINT texNumber);

	//ヒープの解放

public: //静的メンバ変数  (たくさん使うので、使いやすいようにpublicにしておく)
	//デバイス
	static ID3D12Device* dev;
	//コマンドリスト
	static ID3D12GraphicsCommandList* cmdList;
	//デスクリプタヒープ
	static ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	
	static UINT texNumber;
};