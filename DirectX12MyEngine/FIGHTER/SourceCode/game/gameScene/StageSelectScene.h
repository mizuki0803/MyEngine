#pragma once
#include "BaseGameScene.h"
#include "ObjObject3d.h"
#include "LightGroup.h"
#include "StageSelectCamera.h"
#include "LightCamera.h"
#include "StageSelectPlayer.h"
#include "StageSelectPlanet.h"
#include "Skydome.h"
#include "Ground.h"
#include "StageSelectUI.h"
#include "StageDecisionUI.h"

#include <sstream>

/// <summary>
/// �X�e�[�W�I���V�[��
/// </summary>
class StageSelectScene :public BaseGameScene
{
public:
	//�I������X�e�[�W
	enum class SelectStage
	{
		Title,		//�^�C�g���V�[��
		Stage01,	//�X�e�[�W01�V�[��
		ComingSoon,	//�܂��Ȃ��X�e�[�W
	};

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
	/// �f������
	/// </summary>
	void CreatePlanets();

	/// <summary>
	/// �f������o�Ă���
	/// </summary>
	/// <returns></returns>
	void GooutPlanet();

	/// <summary>
	/// �X�e�[�W�I��
	/// </summary>
	void StageSelect();

	/// <summary>
	/// �I������X�e�[�W���E�ɕύX
	/// </summary>
	void StageSelectChangeRight();

	/// <summary>
	/// �I������X�e�[�W�����ɕύX
	/// </summary>
	void StageSelectChangeLeft();

	/// <summary>
	/// �X�e�[�W����
	/// </summary>
	void StageDicision();

	/// <summary>
	/// �X�e�[�W���肷�邩�I��
	/// </summary>
	void SelectStageDicision();

	/// <summary>
	/// �f���ɓ���s��
	/// </summary>
	void EnterPlanet();

	/// <summary>
	/// �I�������V�[���ɕύX
	/// </summary>
	void ChangeSelectScene();


private: //�ÓI�����o�ϐ�
	//�I�����Ă���X�e�[�W
	static SelectStage selectStage;

private: //�����o�ϐ�
	//�J����
	std::unique_ptr<StageSelectCamera> stageSelectCamera;
	//�e�p�����J����
	std::unique_ptr<LightCamera> lightCamera;
	//���ォ��̉e�p�����J����
	std::unique_ptr<LightCamera> topLightCamera;

	//���C�g
	std::unique_ptr<LightGroup> lightGroup;
	float ambientColor0[3] = { 1,1,1 };
	// �������������l
	float lightDir0[3] = { 0,-3,-4 };
	float lightColor0[3] = { 1,1,1 };
	float lightDir1[3] = { 1,-2,4 };
	float lightColor1[3] = { 1,1,1 };
	float lightDir2[3] = { -1,-2,4 };
	float lightColor2[3] = { 1,1,1 };

	//obj���f���f�[�^
	std::unique_ptr<ObjModel> modelSkydome;
	std::unique_ptr<ObjModel> modelFighter;
	std::unique_ptr<ObjModel> modelStageSelect;
	std::unique_ptr<ObjModel> modelPlanetBasis;
	std::unique_ptr<ObjModel> modelPlanetTrial;
	std::unique_ptr<ObjModel> modelPlanetSoon;

	//�X�e�[�W�I��p�f��
	std::vector<std::unique_ptr<StageSelectPlanet>> planets;
	//�X�e�[�W�I���t�B�[���h���W
	std::vector<Vector3> stageSelectFieldPos;
	//�X�e�[�W�I��p���@
	std::unique_ptr<StageSelectPlayer> player;
	//�V��
	std::unique_ptr<Skydome> skydome;
	//�n��
	std::unique_ptr<Ground> ground;
	//�X�e�[�W�I���t�B�[���h
	std::unique_ptr<ObjObject3d> stageSelectField;

	//�X�e�[�W�I���V�[��UI
	std::unique_ptr<StageSelectUI> stageSelectUI;
	//�X�e�[�W����m�FUI
	std::unique_ptr<StageDecisionUI> stageDecisionUI;

	//�X�e�[�W����m�F����
	bool isStageDecisionCheck = false;
};