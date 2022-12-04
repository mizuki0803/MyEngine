#pragma once
#include <wrl.h>
#include <d3d12.h>

/// <summary>
/// SRV�p�f�X�N���v�^�q�[�v
/// </summary>
class DescHeapSRV
{
private: //�G�C���A�X
	//Microsoft::WRL::���ȗ�
	template<class T> using ComPtr = Microsoft::WRL::ComPtr<T>;

private:
	//�e�N�X�`���̍ő喇��
	static const int SRVCount = 512;

public: //�ÓI�����o�֐�
	/// <summary>
	/// ���ʏ������ݒ�
	/// </summary>
	static void Initialize(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList);

	/// <summary>
	/// SRV����SetDescriptorHeaps
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	static void SetDescriptorHeaps();

	/// <summary>
	/// SRV����CreateShaderResourceView
	/// </summary>
	static void CreateShaderResourceView(const D3D12_SHADER_RESOURCE_VIEW_DESC& srvDesc, ID3D12Resource* texBuff);

	/// <summary>
	/// SRV����SetGraphicsRootDescriptorTable
	/// </summary>
	static void SetGraphicsRootDescriptorTable(UINT rootParameterIndex, UINT texNumber);

	//�q�[�v�̉��

public: //�ÓI�����o�ϐ�  (��������g���̂ŁA�g���₷���悤��public�ɂ��Ă���)
	//�f�o�C�X
	static ID3D12Device* dev;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//�f�X�N���v�^�q�[�v
	static ComPtr<ID3D12DescriptorHeap> descHeapSRV;
	
	static UINT texNumber;
};