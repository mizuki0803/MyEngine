#pragma once
#include "ground.h"
#include <array>

//���@
class Player;
//�Q�[���J����
class GameCamera;

/// <summary>
/// �Q�[���Ŏg���n�ʊǗ�
/// </summary>
class GameGroundManager
{
public:  //�ÓI�����o�ϐ�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�Q�[���Ŏg���n�ʊǗ�</returns>
	static GameGroundManager* Create(ObjModel* model);

	//setter
	static void SetPlayer(Player* player) { GameGroundManager::player = player; }
	static void SetGameCamera(GameCamera* gameCamera) { GameGroundManager::gameCamera = gameCamera; }

public: //�����o�ϐ�
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize(ObjModel* model);

	/// <summary>
	/// �X�V
	/// </summary>
	void Update();

	/// <summary>
	/// �`��
	/// </summary>
	void Draw();

	//setter
	void SetIsScroll(bool isScroll) { this->isScrollMode = isScroll; }

private: //�����o�֐�
	/// <summary>
	/// ���ɍs�����n�ʂ�O�Ɏ����Ă���
	/// </summary>
	void SwapGroundPos();

	/// <summary>
	/// �X�N���[�����
	/// </summary>
	void ScrollMode();

private: //�ÓI�����o�ϐ�
	//���@
	static Player* player;
	//�Q�[���J����
	static GameCamera* gameCamera;

private: //�����o�ϐ�
	//�n�ʃI�u�W�F�N�g
	std::array<std::unique_ptr<Ground>, 2> grounds;
	//�n�ʃ��f��
	ObjModel* model = nullptr;
	//�n�ʂ��X�N���[����Ԃ�
	bool isScrollMode = false;
};