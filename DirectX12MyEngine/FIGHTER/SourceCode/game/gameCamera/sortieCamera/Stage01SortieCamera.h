#pragma once
#include <DirectXMath.h>
#include "Camera.h"

//�X�e�[�W01�o���V�[���p���@�N���X��O���錾
class Stage01SortiePlayer;

/// <summary>
/// �X�e�[�W01�o���V�[���p�J����
/// </summary>
class Stage01SortieCamera : public Camera
{
public:
	//�X�e�[�W01�o���s���t�F�[�Y
	enum class Stage01SortieActionPhase {
		Stay,					//���@���������痈��̂�҂�
		RunningSideZoom,		//���@�����&�Y�[��
		RunningSideSpeedMove,	//���@��������Ă������x�œ���
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

	//getter
	bool GetIsZoomEnd() const { return isZoomEnd; }

	//setter
	void SetSortiePlayer(Stage01SortiePlayer* player) { this->player = player; }

private: //�����o�֐�
	/// <summary>
	/// ���@���������痈��̂�҂�
	/// </summary>
	void StayPlayer();

	/// <summary>
	/// ���@������Y�[��
	/// </summary>
	void RunningSideZoomPlayer();

	/// <summary>
	/// ���@�������Ă������x�œ�����
	/// </summary>
	void CameraRunningSideSpeedMove();

private: //�ÓI�����o�ϐ�
	//�o���s���J��
	static void (Stage01SortieCamera::* sortieActionFuncTable[])();
	//�������J�n���鎩�@�ƃJ�����̋���
	static const float runningSideStartDistance;
	//�������I�����鎩�@�ƃJ�����̋���
	static const float runningSideEndDistance;

private: //�����o�ϐ�
	//�v���C���[���@
	Stage01SortiePlayer* player = nullptr;
	//�J�������_�Ǝ��@���W�̋���
	Vector3 eyePlayerDistance;
	//�o���J��������
	Stage01SortieActionPhase sortieMovePhase = Stage01SortieActionPhase::Stay;
	//�J�����s���p�^�C�}�[
	int32_t cameraActionTimer = 0;
	//���@�Y�[�����I�����邩
	bool isZoomEnd = false;
};