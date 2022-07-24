#pragma once
#include "ObjObject3d.h"
#include "PlayerBullet.h"
#include "Reticle.h"
#include <memory>
#include <list>

//�Q�[���V�[���̑O���錾
class GameScene;

class Enemy;

/// <summary>
/// ���@
/// </summary>
class Player : public ObjObject3d
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>���@</returns>
	static Player* Create(ObjModel* model);

	//setter
	static void SetGameScene(GameScene* gameScene) { Player::gameScene = gameScene; }
	static void SetBulletModel(ObjModel* model) { Player::bulletModel = model; }

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
	/// UI�`��
	/// </summary>
	void DrawUI();

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	void OnCollision();

	//getter
	Vector3 GetWorldPos();
	Enemy* GetEnemy() { return enemy; }

	void SetEnemy(Enemy* enemy) { this->enemy = enemy; }

private: //�����o�֐�
	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// ��]
	/// </summary>
	void Rotate();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

private: //�ÓI�����o�ϐ�
	//�Q�[���V�[��
	static GameScene* gameScene;
	//���@�e�̃��f��
	static ObjModel* bulletModel;

private: //�����o�ϐ�
	//���e�B�N��
	std::unique_ptr<Reticle> reticle;
	std::unique_ptr<Reticle> reticle2;
	Enemy* enemy = nullptr;
};
