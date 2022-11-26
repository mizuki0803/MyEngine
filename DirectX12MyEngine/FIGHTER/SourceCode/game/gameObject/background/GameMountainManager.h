#pragma once
#include "Mountain.h"

/// <summary>
/// �Q�[���Ŏg���R�Ǘ�
/// </summary>
class GameMountainManager
{
public:  //�ÓI�����o�ϐ�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�Q�[���Ŏg���R�Ǘ�</returns>
	static GameMountainManager* Create(ObjModel* model, float distance, int startNum);

public: //�����o�ϐ�
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize(ObjModel* model, float distance, int startNum);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update(const Vector3& targetPos);

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	/// <summary>
	/// �e�p�������C�g���猩�����_�ł̕`��
	/// </summary>
	void DrawLightCameraView();

private: //�����o�֐�
	/// <summary>
	/// �V���ȎR����
	/// </summary>
	void CreateNewMountain();

private: //�����o�ϐ�
	//�R�I�u�W�F�N�g
	std::list<std::unique_ptr<Mountain>> mountains;
	//�R���f��
	ObjModel* model = nullptr;
	//���S����̋���
	float distance = 0;
	//��ʒu����̎R�̔ԍ�
	int mountainNum = 0;
	//�V���ȎR��ݒu�\��
	bool isCanCreate = true;
};