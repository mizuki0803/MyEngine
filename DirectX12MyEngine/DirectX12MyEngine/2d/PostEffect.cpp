#include "PostEffect.h"
#include "WindowApp.h"
#include <d3dx12.h>

PostEffect* PostEffect::Create(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	// 3D�I�u�W�F�N�g�̃C���X�^���X�𐶐�
	PostEffect* instance = new PostEffect();
	if (instance == nullptr) {
		return nullptr;
	}

	// ������
	if (!instance->Initialize(texNumber, anchorpoint, isFlipX, isFlipY)) {
		delete instance;
		assert(0);
		return nullptr;
	}
	instance->SetPosition({ 0, 0 });
	instance->SetSize({ 500, 500 });
	instance->Update();

	return instance;
}

bool PostEffect::Initialize(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY)
{
	HRESULT result;

	//���N���X�Ƃ��Ă̏�����
	if (!Sprite::Initialize(texNumber, anchorpoint, isFlipX, isFlipY)) {
		return false;
	}

	//�e�N�X�`�����\�[�X�ݒ�
	CD3DX12_RESOURCE_DESC texresDesc = CD3DX12_RESOURCE_DESC::Tex2D(
		DXGI_FORMAT_R8G8B8A8_UNORM,
		WindowApp::window_width,
		(UINT)WindowApp::window_height,
		1, 0, 1, 0, D3D12_RESOURCE_FLAG_ALLOW_RENDER_TARGET
	);

	//�e�N�X�`���o�b�t�@�̐���
	ID3D12Device* dev = SpriteCommon::GetInstance()->GetDevice();
	result = dev->CreateCommittedResource(
		&CD3DX12_HEAP_PROPERTIES(D3D12_CPU_PAGE_PROPERTY_WRITE_BACK,
			D3D12_MEMORY_POOL_L0),
		D3D12_HEAP_FLAG_NONE,
		&texresDesc,
		D3D12_RESOURCE_STATE_PIXEL_SHADER_RESOURCE,
		nullptr,
		IID_PPV_ARGS(&texBuff)
	);
	assert(SUCCEEDED(result));

	//�e�N�X�`����ԃN���A
	{
		//��f��(1280x720 = 921600�s�N�Z��)
		const UINT pixelCount = WindowApp::window_width * WindowApp::window_height;
		//�摜1�s���̃f�[�^�T�C�Y
		const UINT rowPitch = sizeof(UINT) * WindowApp::window_width;
		//�摜�S�̂̃f�[�^�T�C�Y
		const UINT depthPitch = rowPitch * WindowApp::window_height;
		//�摜�C���[�W
		UINT* img = new UINT[pixelCount];
		for (int i = 0; i < pixelCount; i++) {
			img[i] = 0xff0000ff;
		}

		//�e�N�X�`���o�b�t�@�Ƀf�[�^�]��
		result = texBuff->WriteToSubresource(0, nullptr,
			img, rowPitch, depthPitch);
		assert(SUCCEEDED(result));
		delete[] img;
	}

	//SRV�p�f�X�N���v�^�q�[�v�ݒ�
	D3D12_DESCRIPTOR_HEAP_DESC srvDescHeapDesc = {};
	srvDescHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
	srvDescHeapDesc.Flags = D3D12_DESCRIPTOR_HEAP_FLAG_SHADER_VISIBLE;
	srvDescHeapDesc.NumDescriptors = 1;
	//SRV�p�f�X�N���v�^�q�[�v�𐶐�
	result = dev->CreateDescriptorHeap(&srvDescHeapDesc, IID_PPV_ARGS(&descHeapSRV));
	assert(SUCCEEDED(result));

	//SRV�ݒ�
	D3D12_SHADER_RESOURCE_VIEW_DESC srvDesc{};	//�ݒ�\����
	srvDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
	srvDesc.Shader4ComponentMapping = D3D12_DEFAULT_SHADER_4_COMPONENT_MAPPING;
	srvDesc.ViewDimension = D3D12_SRV_DIMENSION_TEXTURE2D;	//2D�e�N�X�`��
	srvDesc.Texture2D.MipLevels = 1;

	//�f�X�N���v�^�q�[�v��SRV�쐬
	dev->CreateShaderResourceView(texBuff.Get(),	//�r���[�Ɗ֘A�t����o�b�t�@
		&srvDesc,
		descHeapSRV->GetCPUDescriptorHandleForHeapStart()
	);

	return true;
}

void PostEffect::Draw()
{
	SpriteCommon* spriteCommon = SpriteCommon::GetInstance();
	ID3D12GraphicsCommandList* cmdList = spriteCommon->GetCmdList();
	PipelineSet pipelineSet = spriteCommon->GetPipelineSet();

	//�p�C�v���C���X�e�[�g�̐ݒ�
	cmdList->SetPipelineState(pipelineSet.pipelinestate.Get());
	//���[�g�V�O�l�`���̐ݒ�
	cmdList->SetGraphicsRootSignature(pipelineSet.rootsignature.Get());
	//�v���~�e�B�u�`���ݒ�
	cmdList->IASetPrimitiveTopology(D3D_PRIMITIVE_TOPOLOGY_TRIANGLESTRIP);

	//�e�N�X�`���p�f�X�N���v�^�q�[�v�̐ݒ�
	//ID3D12DescriptorHeap* ppHeap[] = { spriteCommon->GetDescHeap().Get() };
	ID3D12DescriptorHeap* ppHeap[] = { descHeapSRV.Get() };
	cmdList->SetDescriptorHeaps(_countof(ppHeap), ppHeap);

	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//���[�g�p�����[�^0�Ԃɒ萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//���[�g�p�����[�^1�ԂɃV�F�[�_���\�[�X�r���[���Z�b�g
	//spriteCommon->SetGraphicsRootDescriptorTable(1, texNumber);
	cmdList->SetGraphicsRootDescriptorTable(1, descHeapSRV->GetGPUDescriptorHandleForHeapStart());

	//�|���S���̕`��(4���_�Ŏl�p�`)
	cmdList->DrawInstanced(4, 1, 0, 0);
}
