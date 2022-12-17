#pragma once
#include "BaseGameScene.h"
#include "ObjObject3d.h"
#include "LightGroup.h"
#include "SortieCamera.h"
#include "LightCamera.h"
#include "SortiePlayer.h"
#include "Skydome.h"
#include "Ground.h"
#include "Mountain.h"

#include <sstream>

/// <summary>
/// �o�����o�V�[��
/// </summary>
class SortieScene :public BaseGameScene
{
private: // �G�C���A�X
	// Microsoft::WRL::���ȗ�
	template <class T> using ComPtr = Microsoft::WRL::ComPtr<T>;
	// DirectX::���ȗ�
	using XMFLOAT2 = DirectX::XMFLOAT2;
	using XMFLOAT3 = DirectX::XMFLOAT3;
	using XMFLOAT4 = DirectX::XMFLOAT4;
	using XMVECTOR = DirectX::XMVECTOR;
	using XMMATRIX = DirectX::XMMATRIX;

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �w�i�X�v���C�g�`��
	/// </summary>
	void DrawBackSprite() override;

	/// <summary>
	/// 3D�V�[���`��
	/// </summary>
	void Draw3D() override;

	/// <summary>
	/// 3D�V�[���`��(���C�g���猩�����_)
	/// </summary>
	void Draw3DLightView() override;

	/// <summary>
	/// 3D�V�[���`��(���ォ��̃��C�g���猩�����_)
	/// </summary>
	void Draw3DTopLightView() override;

	/// <summary>
	/// �O�i�X�v���C�g�`��
	/// </summary>
	void DrawFrontSprite() override;

private: //�����o�֐�
	/// <summary>
	/// �e�����p���C�g�J�����X�V
	/// </summary>
	void LightCameraUpdate();

	/// <summary>
	/// �o��
	/// </summary>
	void SortieAction();

	/// <summary>
	/// �X�L�b�v�@�\
	/// </summary>
	void MovieSkip();

private: //�����o�ϐ�
	//�J����
	std::unique_ptr<SortieCamera> sortieCamera;
	//�e�p�����J����
	std::unique_ptr<LightCamera> lightCamera;
	//���ォ��̉e�p�����J����
	std::unique_ptr<LightCamera> topLightCamera;

	//���C�g
	std::unique_ptr<LightGroup> lightGroup;
	float ambientColor0[3] = { 1,1,1 };
	// �������������l
	float lightDir0[3] = { 0,-1,0 };
	float lightColor0[3] = { 1,1,1 };

	//obj���f���f�[�^
	std::unique_ptr<ObjModel> modelSkydome;
	std::unique_ptr<ObjModel> modelGround;
	std::unique_ptr<ObjModel> modelMountain;
	std::unique_ptr<ObjModel> modelSphere;
	std::unique_ptr<ObjModel> modelFighter;

	//�o���V�[���p���@
	std::unique_ptr<SortiePlayer> player;
	//�V��
	std::unique_ptr<Skydome> skydome;
	//�n��
	std::unique_ptr<Ground> ground;
	//�w�i�p(�R)
	std::list<std::unique_ptr<Mountain>> mountains;
};