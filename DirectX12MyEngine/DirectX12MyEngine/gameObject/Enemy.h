#pragma once
#include "ObjObject3d.h"
#include "EnemyBullet.h"
#include "Vector2.h"

//���@�N���X��O���錾
class Player;
//GameScene�̑O���錾
class GameScene;

/// <summary>
/// �G
/// </summary>
class Enemy : public ObjObject3d
{
public: //enum
	enum EnemyType {
		Demo,		//�f���p�G
		Cannon,		//��C
		Circular,	//�~�^��
		Fall,		//�~��
		UpDown,		//�㉺�ړ�
		ComeGo,		//�����o��
	};

public: //�ÓI�����o�֐�
	//setter
	static void SetPlayer(Player* player) { Enemy::player = player; }
	static void SetGameScene(GameScene* gameScene) { Enemy::gameScene = gameScene; }
	static void SetBulletModel(ObjModel* model) { Enemy::bulletModel = model; }

public: //�����o�֐�

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	virtual void OnCollision();

	//getter
	Vector3 GetWorldPos();
	Vector2 GetScreenPos();
	bool GetIsDead() const { return isDead; }

protected: //�����o�֐�
	/// <summary>
	/// �e����
	/// </summary>
	void Fire();

	/// <summary>
	/// ��ʎ�O�܂ōs������폜���鏈��
	/// </summary>
	void FrontOfScreenDelete();

protected: //�ÓI�����o�ϐ�
	//�v���C���[���@
	static Player* player;
	//�Q�[���V�[��
	static GameScene* gameScene;
	//�G�e�̃��f��
	static ObjModel* bulletModel;

protected: //�����o�ϐ�
	//���S�t���O
	bool isDead = false;
};
