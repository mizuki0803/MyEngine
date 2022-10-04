#pragma once
#include "ObjObject3d.h"
#include "PlayerBullet.h"
#include "PlayerReticles.h"
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
	/// �`��
	/// </summary>
	void Draw() override;

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
	const bool GetIsCrash() { return isCrash; }
	const int GetCrashBoundCount() { return crashBoundCount; }
	const bool GetIsDead() { return isDead; }
	const bool GetIsRoll() { return isRoll; }
	const Vector3& GetKnockbackVel() { return knockbackVel; }
	PlayerReticles* GetReticles() { return reticles.get(); }
	const bool GetIsChargeShotMode() { return isChargeShotMode; }

private: //�����o�֐�
	/// <summary>
	/// �_���[�W����炤
	/// </summary>
	void Damage();

	/// <summary>
	/// �ė��J�n
	/// </summary>
	void CrashStart();

	/// <summary>
	/// �ė�
	/// </summary>
	void Crash();

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
	/// �ړ�
	/// </summary>
	void Roll();

	/// <summary>
	/// �U��
	/// </summary>
	void Attack();

	/// <summary>
	/// �e���ˍ��W���X�V
	/// </summary>
	void UpdateBulletShotPos();

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
	//�z�[�~���O�e�̑傫��
	static const float homingBulletSize;
	//���@�̉�]���E
	static const Vector2 rotLimit;
	//�ő�̗�
	static const int maxHP = 101;

private: //�����o�ϐ�
	//�̗�
	int HP = maxHP;
	//HP�o�[
	std::unique_ptr<PlayerHPBar> hpBar;
	//HP�o�[�t���[��
	std::unique_ptr<PlayerHPFrame> hpFrame;
	//�_���[�W�t���O
	bool isDamage = false;
	//�ė�����
	bool isCrash = false;
	//�ė��o�E���h��
	int crashBoundCount = 0;
	//�ė����x
	Vector3 crashVel = { 0, 0, 0.2f };
	//���S�t���O
	bool isDead = false;
	//�ً}��𒆂�
	bool isRoll = false;
	//�ً}���p�^�C�}�[
	int32_t rollTimer = 0;
	//�ً}����J�n����Z���p�x
	float rollStartRot = 0;
	//�ً}����I������Z���p�x
	float rollEndRot = 0;
	//�m�b�N�o�b�N�p�^�C�}�[
	int32_t knockbackTimer = 0;
	//�m�b�N�o�b�N����
	Vector3 knockbackVec;
	//�m�b�N�o�b�N���x
	Vector3 knockbackVel;
	//���e�B�N��
	std::unique_ptr<PlayerReticles> reticles;
	//�e���ˍ��W
	Vector3 bulletShotPos;
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
