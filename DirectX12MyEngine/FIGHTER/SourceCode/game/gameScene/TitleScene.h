#pragma once
#include "BaseGameScene.h"
#include "Sprite.h"
#include "ObjObject3d.h"
#include "LightGroup.h"
#include "TitleCamera.h"
#include "LightCamera.h"
#include "CollisionShape.h"
#include "Collision.h"
#include "TitlePlayer.h"
#include "Skydome.h"
#include "Ground.h"
#include "Mountain.h"
#include "TitleUI.h"
#include "LevelDataLoader.h"

#include <sstream>

/// <summary>
/// �^�C�g���V�[��
/// </summary>
class TitleScene :public BaseGameScene
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
	/// �O�i�X�v���C�g�`��
	/// </summary>
	void DrawFrontSprite() override;

private: //�����o�֐�
	/// <summary>
	/// �o���J�n
	/// </summary>
	void SortieStart();

	/// <summary>
	/// ���[�r�[�X�L�b�v�@�\
	/// </summary>
	void MovieSkip();

private: //�����o�ϐ�
	//�J����
	std::unique_ptr<TitleCamera> titleCamera;
	//�e�p�����J����
	std::unique_ptr<LightCamera> lightCamera;

	//���C�g
	std::unique_ptr<LightGroup> lightGroup;
	float ambientColor0[3] = { 1,1,1 };
	// �������������l
	float lightDir0[3] = { 1,-1,-12 };
	float lightColor0[3] = { 1,1,1 };

	//obj���f���f�[�^
	std::unique_ptr<ObjModel> modelSkydome;
	std::unique_ptr<ObjModel> modelFighter;
	std::unique_ptr<ObjModel> modelbaseField;
	std::unique_ptr<ObjModel> modelRoad;
	std::unique_ptr<ObjModel> modelWarehouse01;
	std::unique_ptr<ObjModel> modelWarehouse02;
	std::unique_ptr<ObjModel> modelControlTower;

	//�w�i�}�b�v���x���f�[�^
	std::unique_ptr<LevelDataLoader> backgroundMapData;

	//�^�C�g���V�[���p���@
	std::unique_ptr<TitlePlayer> player;
	//�V��
	std::unique_ptr<Skydome> skydome;

	//�^�C�g��UI
	std::unique_ptr<TitleUI> titleUI;

	//���̑��ϐ�
	float soundVol = 0.1f;
	char str[100];
};