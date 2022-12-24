#pragma once
#include "BaseGameScene.h"
#include "LightGroup.h"
#include "TitleCamera.h"
#include "LightCamera.h"
#include "TitlePlayer.h"
#include "Skydome.h"
#include "TitleUI.h"
#include "LevelDataLoader.h"

#include <sstream>

/// <summary>
/// �^�C�g���V�[��
/// </summary>
class TitleScene :public BaseGameScene
{
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
	//���ォ��̉e�p�����J����
	std::unique_ptr<LightCamera> topLightCamera;

	//���C�g
	std::unique_ptr<LightGroup> lightGroup;
	float ambientColor0[3] = { 1,1,1 };
	// �������������l
	float lightDir0[3] = { 2,-1,-6 };
	float lightColor0[3] = { 1,1,1 };
	float lightDir1[3] = { -2,-1,6 };
	float lightColor1[3] = { 0.3f,0.3f,0.3f };

	//obj���f���f�[�^
	std::unique_ptr<ObjModel> modelSkydome;
	std::unique_ptr<ObjModel> modelFighter;
	std::unique_ptr<ObjModel> modelbaseField;
	std::unique_ptr<ObjModel> modelbaseField02;
	std::unique_ptr<ObjModel> modelRoad;
	std::unique_ptr<ObjModel> modelWarehouse01;
	std::unique_ptr<ObjModel> modelWarehouse02;
	std::unique_ptr<ObjModel> modelControlTower;
	std::unique_ptr<ObjModel> modelControlRoom;

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