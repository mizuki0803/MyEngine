#pragma once
#include <DirectXTex.h>
#include <d3d12.h>
#include <wrl.h>
#include <d3dx12.h>
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"
#include "CircleShadow.h"
#include "Vector3.h"

/// <summary>
/// ���C�g
/// </summary>
class LightGroup
{
private://�G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public://�萔
	//���s�����̐�
	static const int DirLightNum = 3;
	//�_�����̐�
	static const int PointLightNum = 3;
	//�X�|�b�g���C�g�̐�
	static const int SpotLightNum = 3;
	//�ۉe�̐�
	static const int CircleShadowNum = 1;

public://�T�u�N���X
	//�萔�o�b�t�@�p�f�[�^�\����
	struct ConstBufferData
	{
		//�����̐F
		XMFLOAT3 ambientColor;
		float pad1;
		//���s�����p
		DirectionalLight::ConstBufferData dirLights[DirLightNum];
		//�_�����p
		PointLight::ConstBufferData pointLights[PointLightNum];
		//�X�|�b�g���C�g�p
		SpotLight::ConstBufferData spotLights[SpotLightNum];
		//�ۉe�p
		CircleShadow::ConstBufferData circleShadows[CircleShadowNum];
	};


public: // �ÓI�����o�֐�

	/// <summary>
	/// �ÓI������
	/// </summary>
	/// <param name="device">�f�o�C�X</param>
	static void StaticInitialize(ID3D12Device* device);

	/// <summary>
	/// �C���X�^���X����
	/// </summary>
	/// <returns>�C���X�^���X</returns>
	static LightGroup* Create();


private: // �ÓI�����o�ϐ�
	//�f�o�C�X
	static ID3D12Device* device;


public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize();

	/// <summary>
	/// �萔�o�b�t�@�]��
	/// </summary>
	void TransferConstBuffer();

	/// <summary>
	/// �W���̃��C�g�ݒ�
	/// </summary>
	void DefaultLightSetting();

	/// <summary>
	/// �����̃��C�g�F���Z�b�g
	/// </summary>
	/// <param name="color">���C�g�F</param>
	void SetAmbientColor(const XMFLOAT3& color);

	/// <summary>
	/// ���s�����̗L���t���O���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="active">�L���t���O</param>
	void SetDirLightActive(int index, bool active);

	/// <summary>
	/// ���s�����̃��C�g�������Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightdir">���C�g����</param>
	void SetDirLightDir(int index, const XMVECTOR& lightdir);

	/// <summary>
	/// ���s�����̃��C�g�F���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightcolor">���C�g�F</param>
	void SetDirLightColor(int index, const XMFLOAT3& lightcolor);

	/// <summary>
	/// �_�����̗L���t���O���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="active">�L���t���O</param>
	void SetPointLightActive(int index, bool active);

	/// <summary>
	/// �_�����̃��C�g���W���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightpos">���C�g���W</param>
	void SetPointLightPos(int index, const Vector3& lightpos);

	/// <summary>
	/// �_�����̃��C�g�F���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightcolor">���C�g�F</param>
	void SetPointLightColor(int index, const XMFLOAT3& lightcolor);

	/// <summary>
	/// �_�����̃��C�g���������W�����Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightAtten">���C�g���������W��</param>
	void SetPointLightAtten(int index, const XMFLOAT3& lightAtten);

	/// <summary>
	/// �X�|�b�g���C�g�̗L���t���O���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="active">�L���t���O</param>
	void SetSpotLightActive(int index, bool active);

	/// <summary>
	/// �X�|�b�g���C�g�̃��C�g�������Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightdir">���C�g����</param>
	void SetSpotLightDir(int index, const XMVECTOR& lightdir);

	/// <summary>
	/// �X�|�b�g���C�g�̃��C�g���W���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightpos">���C�g���W</param>
	void SetSpotLightPos(int index, const Vector3& lightpos);

	/// <summary>
	/// �X�|�b�g���C�g�̃��C�g�F���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightcolor">���C�g�F</param>
	void SetSpotLightColor(int index, const XMFLOAT3& lightcolor);

	/// <summary>
	/// �X�|�b�g���C�g�̃��C�g���������W�����Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightAtten">���C�g���������W��</param>
	void SetSpotLightAtten(int index, const XMFLOAT3& lightAtten);

	/// <summary>
	/// �X�|�b�g���C�g�̃��C�g�����p�x���Z�b�g
	/// </summary>
	/// <param name="index">���C�g�ԍ�</param>
	/// <param name="lightFactorAngleCos">���C�g�����p�x</param>
	void SetSpotLightFactorAngleCos(int index, const XMFLOAT2& lightFactorAngleCos);

	/// <summary>
	/// �ۉe�̗L���t���O���Z�b�g
	/// </summary>
	/// <param name="index">�e�ԍ�</param>
	/// <param name="active">�L���t���O</param>
	void SetCircleShadowActive(int index, bool active);

	/// <summary>
	/// �ۉe�̃L���X�^�[���W���Z�b�g
	/// </summary>
	/// <param name="index">�e�ԍ�</param>
	/// <param name="casterPos">�L���X�^�[���W</param>
	void SetCircleShadowCasterPos(int index, const Vector3& casterPos);

	/// <summary>
	/// �ۉe�̉e�������Z�b�g
	/// </summary>
	/// <param name="index">�e�ԍ�</param>
	/// <param name="shadowDir">�e����</param>
	void SetCircleShadowDir(int index, const XMVECTOR& shadowDir);

	/// <summary>
	/// �ۉe�̃L���X�^�[�ƃ��C�g�̋������Z�b�g
	/// </summary>
	/// <param name="index">�e�ԍ�</param>
	/// <param name="distanceCasterLight">�L���X�^�[�ƃ��C�g�̋���</param>
	void SetCircleShadowDistanceCasterLight(int index, float distanceCasterLight);

	/// <summary>
	/// �ۉe�̉e���������W�����Z�b�g
	/// </summary>
	/// <param name="index">�e�ԍ�</param>
	/// <param name="shadowAtten">�e���������W��</param>
	void SetCircleShadowAtten(int index, const XMFLOAT3& shadowAtten);

	/// <summary>
	/// �ۉe�̉e�����p�x���Z�b�g
	/// </summary>
	/// <param name="index">�e�ԍ�</param>
	/// <param name="shadowFactorAngleCos">�e�����p�x</param>
	void SetCircleShadowFactorAngleCos(int index, const XMFLOAT2& shadowFactorAngleCos);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	/// <param name="cmdList">�R�}���h���X�g</param>
	/// <param name="rootParameterIndex"></param>
	void Draw(ID3D12GraphicsCommandList* cmdList, UINT rootParameterIndex);

private: // �����o�ϐ�
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//�����̐F
	XMFLOAT3 ambientColor = { 1 ,1 ,1 };
	//���s�����̔z��
	DirectionalLight dirLights[DirLightNum];
	//�_�����̔z��
	PointLight pointLights[PointLightNum];
	//�X�|�b�g���C�g�̔z��
	SpotLight spotLights[SpotLightNum];
	//�ۉe�̔z��
	CircleShadow circleShadows[CircleShadowNum];
	//�_�[�e�B�t���O
	bool dirty = false;
};