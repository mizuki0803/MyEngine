#include "PostEffect.h"
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
	instance->SetPosition({ 10, 10 });
	instance->SetSize({ 100, 100 });
	instance->Update();

	return instance;
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
	ID3D12DescriptorHeap* ppHeap[] = { spriteCommon->GetDescHeap().Get()};
	cmdList->SetDescriptorHeaps(_countof(ppHeap), ppHeap);

	//���_�o�b�t�@���Z�b�g
	cmdList->IASetVertexBuffers(0, 1, &vbView);

	//���[�g�p�����[�^0�Ԃɒ萔�o�b�t�@���Z�b�g
	cmdList->SetGraphicsRootConstantBufferView(0, constBuff->GetGPUVirtualAddress());

	//���[�g�p�����[�^1�ԂɃV�F�[�_���\�[�X�r���[���Z�b�g
	spriteCommon->SetGraphicsRootDescriptorTable(1, texNumber);

	//�|���S���̕`��(4���_�Ŏl�p�`)
	cmdList->DrawInstanced(4, 1, 0, 0);
}
