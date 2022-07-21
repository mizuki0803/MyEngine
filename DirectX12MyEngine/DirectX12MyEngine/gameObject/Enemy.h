#pragma once
#include "ObjObject3d.h"
#include "EnemyBullet.h"

//���@�N���X��O���錾
class Player;
//GameScene�̑O���錾
class GameScene;

/// <summary>
/// �G
/// </summary>
class Enemy : public ObjObject3d
{
public:
	//�s���t�F�[�Y
	enum class Phase {
		Previous,	//�O�i
		Back,		//���
	};

public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�G</returns>
	static Enemy* Create(ObjModel* model, const Vector3& position, const Vector3& velocity);

	//setter
	static void SetPlayer(Player* player) { Enemy::player = player; }
	static void SetGameScene(GameScene* gameScene) { Enemy::gameScene = gameScene; }
	static void SetBulletModel(ObjModel* model) { Enemy::bulletModel = model; }

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	void OnCollision();

	/// <summary>
	/// �O�i�t�F�[�Y������
	/// </summary>
	void PreviousPhaseInit();

	//getter
	Vector3 GetWorldPos();

private: //�����o�֐�
	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// �e����
	/// </summary>
	void Fire();

private: //�ÓI�����o�ϐ�
	//���ˊԊu
	static const int fireInterval = 60;
	//�v���C���[���@
	static Player* player;
	//�Q�[���V�[��
	static GameScene* gameScene;
	//�G�e�̃��f��
	static ObjModel* bulletModel;

private: //�����o�ϐ�
	//���x
	Vector3 velocity;
	//�s���t�F�[�Y
	Phase phase = Phase::Previous;
	//���˃^�C�}�[
	int32_t fireTimer = 0;
};
