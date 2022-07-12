#pragma once
#include "ObjObject3d.h"
#include "EnemyBullet.h"

//���@�N���X��O���錾
class Player;

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
	/// �`��
	/// </summary>
	void Draw() override;

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	void OnCollision();

	/// <summary>
	/// �O�i�t�F�[�Y������
	/// </summary>
	void PreviousPhaseInit();

	//setter
	void SetPlayer(Player* player) { this->player = player; }

	//getter
	Vector3 GetWorldPos();
	const std::list<std::unique_ptr<EnemyBullet>>& GetBullets() { return enemyBullets; };

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

private: //�����o�ϐ�
	//���x
	Vector3 velocity;
	//�s���t�F�[�Y
	Phase phase = Phase::Previous;
	//�G�e
	std::list<std::unique_ptr<EnemyBullet>> enemyBullets;
	//���˃^�C�}�[
	int32_t fireTimer = 0;
	//�v���C���[���@
	Player* player = nullptr;
};
