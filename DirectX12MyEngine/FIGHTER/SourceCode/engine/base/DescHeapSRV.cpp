#include "DescHeapSRV.h"
#include <cassert>
#include <d3dx12.h>

using namespace Microsoft::WRL;

ID3D12Device* DescHeapSRV::dev = nullptr;
ID3D12GraphicsCommandList* DescHeapSRV::cmdList = nullptr;
ComPtr<ID3D12DescriptorHeap> DescHeapSRV::descHeapSRV;
UINT DescHeapSRV::texNumCount = 0;

void DescHeapSRV::Initialize(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList)
{
	//�f�o�C�X���Z�b�g
	DescHeapSRV::dev = dev;
	//�R�}���h���X�g���Z�b�g
	DescHeapSRV::cmdList = cmdList;

	HRESULT result;

	//SRV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = SRVCount;
	//SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = dev->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));
}

void DescHeapSRV::SetDescriptorHeaps()
{
	//�f�X�N���v�^�q�[�v�̃Z�b�g
	ID3D12DescriptorHeap* ppHeaps[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeaps), ppHeaps);
}

void DescHeapSRV::CreateShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC& srvDesc, Texture& texture)
{
	texture.texNumber = texNumCount;

	dev->CreateShaderResourceView(
		texture.texBuff.Get(),	//�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,	//�e�N�X�`���ݒ���
		CD3DX12_CPU_DESCRIPTOR_HANDLE(descHeapSRV->GetCPUDescriptorHandleForHeapStart(), texture.texNumber,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)
		)
	);
	
	texNumCount++;
}

void DescHeapSRV::SetGraphicsRootDescriptorTable(UINT rootParameterIndex, UINT texNumber)
{
	cmdList->SetGraphicsRootDescriptorTable(rootParameterIndex,
		CD3DX12_GPU_DESCRIPTOR_HANDLE(
			descHeapSRV->GetGPUDescriptorHandleForHeapStart(),
			texNumber,
			dev->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV)));
}
