#pragma once
#include "Building.h"
#include <array>

//���@
class BasePlayer;
//�Q�[���J����
class BaseGameCamera;

/// <summary>
/// �Q�[���Ŏg���r���Ǘ�
/// </summary>
class GameBuildingManager
{
public:  //�ÓI�����o�ϐ�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="centerDistance">���S����̋���</param>
	/// <param name="objectDistance">�I�u�W�F�N�g���m�̋���</param>
	/// <param name="startNum">�J�n���ɃZ�b�g���鐔</param>
	/// <param name="basePos">����W</param>
	/// <returns>�Q�[���Ŏg���r���Ǘ�</returns>
	static GameBuildingManager* Create(float centerDistance, float objectDistance, int startNum, const Vector3& basePos);

	//setter
	static void SetBuidingModel(int modelNum, ObjModel* model);
	static void SetPlayer(BasePlayer* player) { GameBuildingManager::player = player; }
	static void SetGameCamera(BaseGameCamera* gameCamera) { GameBuildingManager::gameCamera = gameCamera; }

public: //�����o�ϐ�
	/// <summary>
	/// ������
	/// </summary>
	/// <param name="centerDistance">���S����̋���</param>
	/// <param name="objectDistance">�I�u�W�F�N�g���m�̋���</param>
	/// <param name="startNum">�J�n���ɃZ�b�g���鐔</param>
	/// <param name="basePos">����W</param>
	/// <returns>����</returns>
	bool Initialize(float centerDistance, float objectDistance, int startNum, const Vector3& basePos);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �e�p�������C�g���猩�����_�ł̕`��
	/// </summary>
	void DrawLightCameraView();

	/// <summary>
	/// �Q�[���N���A��Ɍ��������̂ŃI�u�W�F�N�g���ʂɒǉ�
	/// </summary>
	void CreateBehindObjects();

	//setter
	void SetIsScroll(bool isScroll) { this->isScrollMode = isScroll; }

private: //�����o�֐�
	/// <summary>
	/// �V���ȃr������
	/// </summary>
	void CreateNewBuilding();

	/// <summary>
	/// ���ɃZ�b�g����r���̃��f���ԍ����擾
	/// </summary>
	/// <param name="lastSetModelNum">1�O�ɃZ�b�g�������f���ԍ�</param>
	/// <returns>���ɃZ�b�g����r���̃��f���ԍ�</returns>
	int NextCreateModelNum(int& lastSetModelNum);

	/// <summary>
	/// �X�N���[�����
	/// </summary>
	void ScrollMode();

private: //�ÓI�����o�ϐ�
	//�r�����f��
	static std::array<ObjModel*, 2> buildingModels;
	//���@
	static BasePlayer* player;
	//�Q�[���J����
	static BaseGameCamera* gameCamera;

private: //�����o�ϐ�
	//�r���I�u�W�F�N�g
	std::list<std::unique_ptr<Building>> buildings;
	//��̍��W
	Vector3 basePos;
	//�����ɍŌ�ɐݒu�����r�����f���ԍ�
	int lastLeftSetModelNum = 0;
	//�E���ɍŌ�ɐݒu�����r�����f���ԍ�
	int lastRightSetModelNum = 1;
	//���S����̋���
	float centerDistance = 0;
	//�I�u�W�F�N�g���m�̋���
	float objectDistance = 0;
	//��ʒu����̃r���̔ԍ�
	int buildingNum = 0;
	//�V���ȃr����ݒu�\��
	bool isCanCreate = true;
	//�Ō�ɐ��������r���̍��W
	Vector3 lastCreateBuildingPos;
	//�r�����X�N���[����Ԃ�
	bool isScrollMode = false;
	//���@�̌��܂ōs������폜���邩
	bool isPlayerBehindDelete = true;
};