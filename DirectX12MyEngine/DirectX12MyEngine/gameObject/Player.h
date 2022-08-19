#pragma once
#include "ObjObject3d.h"
#include "PlayerBullet.h"
#include "Reticle.h"
#include "PlayerHPBar.h"
#include "PlayerHPFrame.h"
#include <memory>
#include <list>

//�Q�[���V�[���̑O���錾
class GameScene;

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

	//getter
	static const Vector2& GetRotLimit() { return rotLimit; }

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
	/// �Փˎ��R�[���o�b�N�֐�(�G�Ȃǃ_���[�W����炤)
	/// </summary>
	void OnCollisionDamage(const Vector3& subjectPos);

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�(�A�C�e���Ȃǉ񕜂���)
	/// </summary>
	void OnCollisionHeal();

	//getter
	Vector3 GetWorldPos();
	const int GetHP() { return HP; }
	const bool GetIsDamage() { return isDamage; }
	const bool GetIsDead() { return isDead; }
	const Vector3& GetKnockbackVel() { return knockbackVel; }
	Reticle* GetReticle() { return reticle2.get(); }
	const bool GetIsChargeShotMode() { return isChargeShotMode; }

private: //�����o�֐�
	/// <summary>
	/// �_���[�W����炤
	/// </summary>
	void Damage();

	/// <summary>
	/// ��
	/// </summary>
	void Heal();

	/// <summary>
	/// ��]
	/// </summary>
	void Rotate();

	/// <summary>
	/// �ړ�
	/// </summary>
	void Move();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// ���i�e����
	/// </summary>
	void ShotStraightBullet();

	/// <summary>
	/// �z�[�~���O�e����
	/// </summary>
	void ShotHomingBullet();

	/// <summary>
	/// �m�b�N�o�b�N�����Z�b�g
	/// </summary>
	void SetKnockback(const Vector3& subjectPos);

	/// <summary>
	/// �m�b�N�o�b�N
	/// </summary>
	void Knockback();

private: //�ÓI�����o�ϐ�
	//�Q�[���V�[��
	static GameScene* gameScene;
	//���@�e�̃��f��
	static ObjModel* bulletModel;
	//���@�̉�]���E
	static const Vector2 rotLimit;
	//�ő�̗�
	static const int maxHP = 100;

private: //�����o�ϐ�
	//�̗�
	int HP = maxHP;
	//HP�o�[
	std::unique_ptr<PlayerHPBar> hpBar;
	//HP�o�[�t���[��
	std::unique_ptr<PlayerHPFrame> hpFrame;
	//�_���[�W�t���O
	bool isDamage = false;
	//�m�b�N�o�b�N�p�^�C�}�[
	int32_t knockbackTimer = 0;
	//�m�b�N�o�b�N����
	Vector3 knockbackVec;
	//�m�b�N�o�b�N���x
	Vector3 knockbackVel;
	//���S�t���O
	bool isDead = false;
	//���e�B�N��
	std::unique_ptr<Reticle> reticle;
	std::unique_ptr<Reticle> reticle2;
	//���i�e�̔��ˑҋ@����
	bool isStraightShotWait = false;
	//���i�e�̔��ˑҋ@�^�C�}�[
	int32_t straightShotWaitTimer = 0;
	//�`���[�W�V���b�g����
	bool isChargeShotMode = false;
	//�`���[�W��������
	int32_t chargeTimer = 0;
	//z��������]���E��]��
	bool isRotZRight = true;
	//z��������]�p
	float swayZ = 0.0f;
};
