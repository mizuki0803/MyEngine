#pragma once
#include "BaseGameScene.h"
#include "ObjObject3d.h"
#include "LightGroup.h"
#include "Stage01SortieCamera.h"
#include "LightCamera.h"
#include "Stage01SortiePlayer.h"
#include "Skydome.h"
#include "Ground.h"
#include "Building.h"

#include <array>

/// <summary>
/// �X�e�[�W01�o�����o�V�[��
/// </summary>
class Stage01SortieScene :public BaseGameScene
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
	std::unique_ptr<Stage01SortieCamera> sortieCamera;
	//�e�p�����J����
	std::unique_ptr<LightCamera> lightCamera;
	//���ォ��̉e�p�����J����
	std::unique_ptr<LightCamera> topLightCamera;

	//���C�g
	std::unique_ptr<LightGroup> lightGroup;
	float ambientColor0[3] = { 1,1,1 };
	// �������������l
	float lightDir0[3] = { 2,-1,1 };
	float lightColor0[3] = { 0.9f,0.9f,0.9f };
	float lightDir1[3] = { -2,-1,-6 };
	float lightColor1[3] = { 0.3f,0.3f,0.3f };

	//obj���f���f�[�^
	std::unique_ptr<ObjModel> modelSkydome;
	std::unique_ptr<ObjModel> modelGround;
	std::array<std::unique_ptr<ObjModel>, 2> modelBuilding;
	std::unique_ptr<ObjModel> modelFighter;

	//�o���V�[���p���@
	std::unique_ptr<Stage01SortiePlayer> player;
	//�V��
	std::unique_ptr<Skydome> skydome;
	//�n��
	std::unique_ptr<Ground> ground;
	//�w�i�p(�r��)
	std::list<std::unique_ptr<Building>> buildings;
};