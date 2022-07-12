#pragma once
#include <DirectXMath.h>
#include <wrl.h>
#include <d3d12.h>
#include "SpriteCommon.h"


/// <summary>
/// �X�v���C�g
/// </summary>
class Sprite
{
protected: // �G�C���A�X
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

public: //�����o�֐�
	/// <summary>
	/// �X�v���C�g����
	/// </summary>
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	/// <param name="anchorpoint">�A���J�[�|�C���g</param>
	/// <param name="isFlipX">���E���]���邩</param>
	/// <param name="isFlipY">�㉺���]���邩</param>
	/// <returns>Sprite</returns>
	static Sprite *Create(UINT texNumber, XMFLOAT2 anchorpoint = { 0.5f, 0.5f }, bool isFlipX = false, bool isFlipY = false);

	/// <summary>
	/// ������
	/// </summary>
	/// <param name="texNumber">�e�N�X�`���ԍ�</param>
	/// <param name="anchorpoint">�A���J�[�|�C���g</param>
	/// <param name="isFlipX">���E���]���邩</param>
	/// <param name="isFlipY">�㉺���]���邩</param>
	/// <returns>����</returns>
	bool Initialize(UINT texNumber, XMFLOAT2 anchorpoint, bool isFlipX, bool isFlipY);

	/// <summary>
	/// ���_�o�b�t�@�̓]��
	/// </summary>
	void TransferVertexBuffer();

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();


	//getter
	const XMFLOAT2 &GetPosition() { return position; }
	const float GetRotation() { return rotation; }
	const XMFLOAT2 &GetSize() { return size; }
	const XMFLOAT2 &GetTexSize() { return texSize; }
	const XMFLOAT2 &GetTexLeftTop() { return texLeftTop; }
	const XMFLOAT4 &GetColor() { return color; }
	const XMFLOAT2 &GetAnchorpoint() { return anchorpoint; }
	const bool GetIsFlipX() { return isFlipX; }
	const bool GetIsFlipY() { return isFlipY; }

	//setter
	void SetTexNumber(UINT texNumber) { this->texNumber = texNumber; }
	void SetPosition(const XMFLOAT2& position) { this->position = position; }
	void SetRotation(float rotation) { this->rotation = rotation; }
	void SetSize(const XMFLOAT2& size) { this->size = size; }
	void SetTexSize(const XMFLOAT2& texSize) { this->texSize = texSize; }
	void SetTexLeftTop(const XMFLOAT2& texLeftTop) { this->texLeftTop = texLeftTop; }
	void SetColor(const XMFLOAT4& color) { this->color = color; }
	void SetAnchorpoint(const XMFLOAT2& anchorpoint) { this->anchorpoint = anchorpoint; }
	void SetIsFlipX(bool isFlipX) { this->isFlipX = isFlipX; }
	void SetIsFlipY(bool isFlipY) { this->isFlipY = isFlipY; }


protected: //�����o�ϐ�
	//���_�o�b�t�@
	ComPtr<ID3D12Resource> vertBuff;
	//���_�o�b�t�@�r���[
	D3D12_VERTEX_BUFFER_VIEW vbView{};
	//�萔�o�b�t�@
	ComPtr<ID3D12Resource> constBuff;
	//Z���܂��̉�]����]�p
	float rotation = 0.0f;
	//���W
	XMFLOAT2 position = { 0, 0 };
	//���[���h�s��
	XMMATRIX matWorld;
	//�F(RGBA)
	XMFLOAT4 color = { 1, 1, 1, 1 };
	//�e�N�X�`���ԍ�
	UINT texNumber = 0;
	//�傫��
	XMFLOAT2 size = { 100, 100 };
	//�A���J�[�|�C���g
	XMFLOAT2 anchorpoint = { 0.5f, 0.5f };
	//���E���]
	bool isFlipX = false;
	//�㉺���]
	bool isFlipY = false;
	//�e�N�X�`��������W
	XMFLOAT2 texLeftTop = { 0, 0 };
	//�e�N�X�`���؂�o���T�C�Y
	XMFLOAT2 texSize = { 100, 100 };
};