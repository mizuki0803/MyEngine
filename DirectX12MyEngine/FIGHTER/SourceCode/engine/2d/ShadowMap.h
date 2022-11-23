#pragma once
#include <DirectXMath.h>
#include "PipelineSet.h"

/// <summary>
/// �V���h�E�}�b�v
/// </summary>
class ShadowMap
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public: //�T�u�N���X
	struct VertexPosUv
	{
		XMFLOAT3 pos;	//xyz���W
		XMFLOAT2 uv;	//uv���W
	};

	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		XMFLOAT4 color;	//�F(RGBA)
		XMMATRIX mat;	//3�ϊ��s��
	};

public:
	/// <summary>
	/// �V���h�E�}�b�v����
	/// </summary>
	/// <returns>Sprite</returns>
	static ShadowMap* Create(const XMFLOAT2& size = { 1.0f, 1.0f }, const XMFLOAT2& center = { 0, 0 });

	/// <summary>
	/// �V���h�E�}�b�v���ʕ����̏�����
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	/// <param name="cmdList">�R�}���h���X�g</param>
	static void ShadowMapCommon(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList);

public:
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize(const XMFLOAT2& size, const XMFLOAT2& center);

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void Draw();

	/// <summary>
	/// �V�[���`��O����
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void DrawScenePrev();

	/// <summary>
	/// �V�[���`��㏈��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	void DrawSceneRear();

	/// <summary>
	/// �p�C�v���C������
	/// </summary>
	void CreateGraphicsPipelineState();

	//getter
	ID3D12Resource* GetDepthBuff() { return depthBuff.Get(); }
	ID3D12DescriptorHeap* GetDescHeapDSV() { return descHeapDSV.Get(); }
	ID3D12DescriptorHeap* GetDescHeapSRV() { return descHeapSRV.Get(); }

private:
	//��ʃN���A�J���[
	static const float clearColor[4];
	//�f�o�C�X
	static ID3D12Device* dev;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//�p�C�v���C���Z�b�g
	static PipelineSet pipelineSet;
private:
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//�e�N�X�`���o�b�t�@
	ComPtr<ID3D12Resource> texBuff;
	//SRV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	//�[�x�o�b�t�@
	ComPtr<ID3D12Resource> depthBuff;
	//RTV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapRTV;
	//DSV�p�f�X�N���v�^�q�[�v
	ComPtr<ID3D12DescriptorHeap> descHeapDSV;
	//�F(RGBA)
	XMFLOAT4 color = { 1, 1, 1, 1 };
};

