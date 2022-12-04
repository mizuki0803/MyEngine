#include "DescHeapSRV.h"
#include <cassert>
#include <d3dx12.h>

using namespace Microsoft::WRL;

ID3D12Device* DescHeapSRV::dev = nullptr;
ID3D12GraphicsCommandList* DescHeapSRV::cmdList = nullptr;
ComPtr<ID3D12DescriptorHeap> DescHeapSRV::descHeapSRV;
UINT DescHeapSRV::texNumber;

void DescHeapSRV::Initialize(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList)
{
	//デバイスをセット
	DescHeapSRV::dev = dev;
	//コマンドリストをセット
	DescHeapSRV::cmdList = cmdList;

	HRESULT result;

	//SRV用デスクリプタヒープ設定
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = SRVCount;
	//SRV用デスクリプタヒープを生成
	result = dev->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));
}

void DescHeapSRV::SetDescriptorHeaps()
{
	//デスクリプタヒープのセット
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void DescHeapSRV::CreateShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC& srvDesc, ID3D12Resource* texBuff)
{
	dev->CreateShaderResourceView(
		texBuff,	//ビューと関連付けるバッファ
		&srvDesc,	//テクスチャ設定情報
		CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapSRV->GetCPUDescriptorHandleForHeapStart(), texNumber,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);
}

void DescHeapSRV::SetGraphicsRootDescriptorTable(UINT rootParameterIndex, UINT texNumber)
{
	cmdList->SetGraphicsRootDescriptorTable(rootParameterIndex,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeapSRV->GetGPUDescriptorHandleForHeapStart(),
			texNumber,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
}
