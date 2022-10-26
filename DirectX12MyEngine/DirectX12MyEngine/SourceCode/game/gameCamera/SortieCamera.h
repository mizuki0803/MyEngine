#pragma once
#include <DirectXMath.h>
#include "Camera.h"

//�o���V�[���p���@�N���X��O���錾
class SortiePlayer;

/// <summary>
/// �o���V�[���p�J����
/// </summary>
class SortieCamera : public Camera
{
public:
	//�o���s���t�F�[�Y
	enum class SortieActionPhase {
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
	void SetSortiePlayer(SortiePlayer* player) { this->player = player; }

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
	static void (SortieCamera::* sortieActionFuncTable[])();
	//�������J�n���鎩�@�ƃJ�����̋���
	static float runningSideStartDistance;

private: //�����o�ϐ�
	//�v���C���[���@
	SortiePlayer* player = nullptr;
	//�J�������_�Ǝ��@���W�̋���
	Vector3 eyePlayerDistance;
	//�o���J��������
	SortieActionPhase sortieMovePhase = SortieActionPhase::Stay;
	//�J�����s���p�^�C�}�[
	int32_t cameraActionTimer = 0;
	//���@�Y�[�����I�����邩
	bool isZoomEnd = false;
};