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
		LookPlayer,		//���@�̕���������
		ZoomPlayer,		//���@���Y�[��
		RunningSide,	//���@�����
		Stay,			//�J������~
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
	/// ���@�̕���������
	/// </summary>
	void LookPlayer();

	/// <summary>
	/// ���@�̕���������
	/// </summary>
	void ZoomPlayer();

	/// <summary>
	/// ���@�����
	/// </summary>
	void RunningSidePlayer();

	/// <summary>
	/// ��~
	/// </summary>
	void Stay();

private: //�ÓI�����o�ϐ�
	//�o���s���J��
	static void (Stage02SortieCamera::* sortieActionFuncTable[])();
	//�������̎��_�Ǝ��@�̋���
	static const Vector3 playerDistance;

private: //�����o�ϐ�
	//�v���C���[���@
	Stage02SortiePlayer* player = nullptr;
	//�o���J��������
	Stage02SortieActionPhase sortieMovePhase = Stage02SortieActionPhase::LookPlayer;
	//�J�����s���p�^�C�}�[
	int32_t cameraActionTimer = 0;
	//�C�[�W���O�p�J�����ړ��O���_
	Vector3 moveBeforeEye = {};
	//���@�Y�[�����I�����邩
	bool isZoomEnd = false;
};