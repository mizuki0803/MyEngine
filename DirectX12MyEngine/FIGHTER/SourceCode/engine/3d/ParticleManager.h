#pragma once
#include <Windows.h>
#include "DirectXBase.h"
#include <DirectXMath.h>
#include <forward_list>
#include <functional>
#include "Camera.h"
#include "PipelineSet.h"
#include "Texture.h"

/// <summary>
/// �p�[�e�B�N���}�l�[�W���[
/// </summary>
class ParticleManager
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMMATRIX = DirectX::XMMATRIX;

public:
	//�e�N�X�`���̍ő喇��
	static const int SRVCount = 512;

public:
	//���_�f�[�^�\����
	struct VertexPos
	{
		Vector3 pos;	//xyz���W
		float scale;	//�X�P�[��
		float rot;	//��]�p
		XMFLOAT4 color;	//�F
	};

	//�萔�o�b�t�@�p�f�[�^�\����B0
	struct ConstBufferData
	{
		XMMATRIX mat;	//�r���[�v���W�F�N�V�����s��
		XMMATRIX matBillboad;	//�r���{�[�h�s��
	};

	//�p�[�e�B�N��1��
	struct Particle
	{
		//���W
		Vector3 position = {};
		//���x
		Vector3 velocity = {};
		//�����x
		Vector3 accel = {};
		//���݃t���[��
		int frame = 0;
		//�I���t���[��
		int numFrame = 0;
		//�X�P�[��
		float scale = 1.0f;
		//�����l
		float startScale = 1.0f;
		//�ŏI�l
		float endScale = 0.0f;
		//�X�P�[���ύX����
		std::function<float(const float, const float, const float)> easingScale;
		//��]�p
		float rotation = 0;
		//��]���x
		float rotSpeed = 0;
		//�F
		XMFLOAT4 color = { 1.0f, 1.0f, 1.0f, 1.0f };
		//�F�����l
		XMFLOAT4 startColor = { 1.0f, 1.0f, 1.0f, 1.0f };
		//�F�I���l
		XMFLOAT4 endColor = { 0.0f, 0.0f, 0.0f, 0.0f };
	};

public:
	/// <summary>
	/// �p�[�e�B�N�����ʕ����̏�����
	/// </summary>
	/// <param name="dev">�f�o�C�X</param>
	/// <param name="cmdList">�R�}���h���X�g</param>
	static void ParticleManagerCommon(ID3D12Device* dev, ID3D12GraphicsCommandList* cmdList, const std::string& directoryPath = "Resources/particleTexture/");

	/// <summary>
	/// ���Z�����p�C�v���C������
	/// </summary>
	static void CreateAddBlendPipeline();

	/// <summary>
	/// ���Z�����p�C�v���C������
	/// </summary>
	static void CreateSubBlendPipeline();

	/// <summary>
	/// ���Z�����`��O����
	/// </summary>
	static void DrawPrevAddBlend();

	/// <summary>
	/// ���Z�����`��O����
	/// </summary>
	static void DrawPrevSubBlend();

	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>ParticleManager</returns>
	static ParticleManager* Create(UINT texNumber);

	/// <summary>
	/// �摜�ǂݍ���
	/// </summary>
	/// <returns>����</returns>
	static bool LoadTexture(UINT texNumber, const std::string& filename);

	/// <summary>
	/// �p�[�e�B�N���̏����Z�b�g
	/// </summary>
	/// <param name="life">����</param>
	/// <param name="position">���W</param>
	/// <param name="velocity">���x</param>
	/// <param name="accel">�����x</param>
	/// <param name="startScale">�X�P�[�������l</param>
	/// <param name="endScale">�X�P�[���I���l</param>
	/// <param name="easingScale">�C�[�W���O�X�P�[���ύX</param>
	/// <param name="startColor">�F�����l</param>
	/// <param name="endColor">�F�I���l</param>
	/// <param name="rotSpeed">��]���x</param>
	void Add(const int life, const Vector3& position, const Vector3& velocity, const Vector3& accel,
		const float startScale, const float endScale, std::function<float(const float, const float, const float)> easingScale,
		const XMFLOAT4& startColor, const XMFLOAT4& endColor, const float rotSpeed = 0);

	/// <summary>
	/// �p�[�e�B�N���̌`����
	/// </summary>
	bool CreateModel(UINT texNumber);

	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �S�폜
	/// </summary>
	void AllDelete();

	//setter
	static void SetCamera(Camera* camera) { ParticleManager::camera = camera; }

private:
	//�f�o�C�X
	static ID3D12Device* dev;
	//�R�}���h���X�g
	static ID3D12GraphicsCommandList* cmdList;
	//���Z�����p�C�v���C���Z�b�g
	static PipelineSet addBlendPipelineSet;
	//���Z�����p�C�v���C���Z�b�g
	static PipelineSet subBlendPipelineSet;
	//�e�N�X�`�����\�[�X(�e�N�X�`���o�b�t�@)�̔z��
	static Texture texture[SRVCount];
	//�e�N�X�`���i�[�f�B���N�g��
	static std::string directoryPath;
	//���_��
	static const int vertexCount = 2048;
	//���_�f�[�^�z��
	VertexPos vertices[vertexCount];
	//�J����
	static Camera* camera;

private:
	//���_�o�b�t�@�̐���
	ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[�̍쐬
	D3D12_VERTEX_BUFFER_VIEW vbView;
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//�e�N�X�`���ԍ�
	UINT texNumber = 0;
	//�p�[�e�B�N���z��
	std::forward_list<Particle> particles;
};