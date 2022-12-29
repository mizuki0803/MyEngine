#pragma once
#include <DirectXMath.h>
#include "Camera.h"

//�X�e�[�W02�o���V�[���p���@�N���X��O���錾
class Stage02SortiePlayer;

/// <summary>
/// �X�e�[�W02�o���V�[���p�J����
/// </summary>
class Stage02SortieCamera : public Camera
{
public:
	//�X�e�[�W02�o���s���t�F�[�Y
	enum class Stage02SortieActionPhase {
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
	void SetSortiePlayer(Stage02SortiePlayer* player) { this->player = player; }

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
	static void (Stage02SortieCamera::* sortieActionFuncTable[])();
	//�������J�n���鎩�@�ƃJ�����̋���
	static const float runningSideStartDistance;
	//�������I�����鎩�@�ƃJ�����̋���
	static const float runningSideEndDistance;

private: //�����o�ϐ�
	//�v���C���[���@
	Stage02SortiePlayer* player = nullptr;
	//�J�������_�Ǝ��@���W�̋���
	Vector3 eyePlayerDistance;
	//�o���J��������
	Stage02SortieActionPhase sortieMovePhase = Stage02SortieActionPhase::Stay;
	//�J�����s���p�^�C�}�[
	int32_t cameraActionTimer = 0;
	//���@�Y�[�����I�����邩
	bool isZoomEnd = false;
};