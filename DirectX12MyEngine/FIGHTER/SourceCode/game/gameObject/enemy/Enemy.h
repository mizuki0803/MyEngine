#pragma once
#include "ObjObject3d.h"
#include "Vector2.h"
#include "EnemyBullet.h"
#include "EnemyBreakEffect.h"

//���@�N���X��O���錾
class BasePlayer;
//�X�e�[�W�V�[���̑O���錾
class BaseStageScene;

/// <summary>
/// �G
/// </summary>
class Enemy : public ObjObject3d
{
public: //enum
	enum EnemyType {
		None,		//���ݒ�
		Cannon,		//��C
		Circular,	//�~�^��
		Fall,		//�~��
		UpDown,		//�㉺�ړ�
		ComeGo,		//�����o��
		Meteorite,	//�j��\覐�
	};

public: //�ÓI�����o�֐�
	//setter
	static void SetPlayer(BasePlayer* player) { Enemy::player = player; }
	static void SetStageScene(BaseStageScene* stageScene) { Enemy::stageScene = stageScene; }
	static void SetBulletModel(ObjModel* model) { Enemy::bulletModel = model; }
	static void SetIsGroundMode(bool isGroundMode) { Enemy::isGroundMode = isGroundMode; }

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	/// <returns>����</returns>
	bool Initialize() override;

	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	virtual void OnCollision(const int damageNum);

	//getter
	Vector3 GetWorldPos();
	Vector2 GetScreenPos();
	bool GetIsCollisionFrame() const { return isCollisionFrame; }
	bool GetIsDead() const { return isDead; }
	bool GetIsDelete() const { return isDelete; }
	bool GetIsHitDelete() const { return isHitDelete; }

protected: //�����o�֐�
	/// <summary>
	/// �e����
	/// </summary>
	void Fire();

	/// <summary>
	/// �_���[�W��Ԃ̏���
	/// </summary>
	void DamageMode();

	/// <summary>
	/// �_���[�W��Ԃő傫�������T�C�Y��߂��Ă�������
	/// </summary>
	void DamageSizeReturn();

	/// <summary>
	/// �_���[�W����������Ԃ̐F�ɂ���
	/// </summary>
	void DamageColorMode();

	/// <summary>
	/// ���S��̍����G�t�F�N�g
	/// </summary>
	/// <param name="size">�G�t�F�N�g�̑傫��</param>
	/// <param name="smokeStartTime">���S���Ă��獕�����o���܂ł̎���</param>
	void DeadSmokeEffect(const float size, const int smokeStartTime = 30);

	/// <summary>
	/// �j��
	/// </summary>
	virtual void Break();

	/// <summary>
	/// �j��G�t�F�N�g�ǉ�
	/// </summary>
	/// <param name="model">���f��</param>
	/// <param name="velocity">���x</param>
	/// <param name="rotSpeed">��]�̑���</param>
	/// <param name="scale">�傫��</param>
	void BreakEffect(ObjModel* model, const Vector3& velocity, const Vector3& rotSpeed, const Vector3& scale);

	/// <summary>
	/// ��ʎ�O�܂ōs������폜���鏈��
	/// </summary>
	void FrontOfScreenDelete();

protected: //�ÓI�����o�ϐ�
	//�v���C���[���@
	static BasePlayer* player;
	//�X�e�[�W�V�[��
	static BaseStageScene* stageScene;
	//�G�e�̃��f��
	static ObjModel* bulletModel;
	//�n�ʂ���̍s�����s����
	static bool isGroundMode;
	//�_���[�W��Ԃ̐F
	static const XMFLOAT4 damageColor;

protected: //�����o�ϐ�
	//HP
	int HP = 0;
	//�ʏ�T�C�Y
	Vector3 normalSize;
	//�_���[�W��Ԃ̃T�C�Y
	Vector3 damageSize;
	//�_���[�W��Ԃ�
	bool isDamage = false;
	//�_���[�W�F��
	bool isDamageColor = false;
	//�_���[�W��ԃ^�C�}�[
	int32_t damageTimer;
	//�����蔻�肪��p�����t���[����
	bool isCollisionFrame = false;
	//���S�t���O
	bool isDead = false;
	//���S���Ă���̎��ԃ^�C�}�[
	int32_t deadTimer = 0;
	//�폜�t���O
	bool isDelete = false;
	//��e���č폜��
	bool isHitDelete = false;
};
