#pragma once
#include "BaseGameScene.h"
#include "ObjObject3d.h"
#include "LightGroup.h"
#include "StageSelectCamera.h"
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
	/// �O�i�X�v���C�g�`��
	/// </summary>
	void DrawFrontSprite() override;

private: //�����o�֐�
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

	//���C�g
	std::unique_ptr<LightGroup> lightGroup;
	float ambientColor0[3] = { 1,1,1 };
	// �������������l
	float lightDir0[3] = { 0,0,1 };
	float lightColor0[3] = { 1,0,0 };

	float lightDir1[3] = { 0,1,0 };
	float lightColor1[3] = { 0,1,0 };

	float lightDir2[3] = { 1,0,0 };
	float lightColor2[3] = { 0,0,1 };

	float pointLightPos[3] = { 0,0,0 };
	float pointLightColor[3] = { 1,1,1 };
	float pointLightAtten[3] = { 0.3f, 0.1f, 0.1f };

	float spotLightDir[3] = { 0, -1, 0 };
	float spotLightPos[3] = { 0, 5, 0 };
	float spotLightColor[3] = { 1, 1, 1 };
	float spotLightAtten[3] = { 0.0f, 0.0f, 0.0f };
	float spotLightfactorAngleCos[2] = { 20.0f, 30.0f };

	float circleShadowDir[3] = { 0, -1, 0 };
	float circleShadowAtten[3] = { 0.5f, 0.6f, 0.0f };
	float circleShadowFactorAngle[2] = { 0.0f, 0.5f };

	//obj���f���f�[�^
	std::unique_ptr<ObjModel> modelSkydome;
	std::unique_ptr<ObjModel> modelGround;
	std::unique_ptr<ObjModel> modelMountain;
	std::unique_ptr<ObjModel> modelSphere;
	std::unique_ptr<ObjModel> modelFighter;
	std::unique_ptr<ObjModel> modelStageSelect;
	std::unique_ptr<ObjModel> modelPlanetBasis;
	std::unique_ptr<ObjModel> modelPlanetMautan;
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