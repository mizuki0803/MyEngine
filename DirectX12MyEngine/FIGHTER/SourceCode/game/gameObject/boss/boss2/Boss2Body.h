#pragma once
#include "ObjObject3d.h"

//�X�e�[�W�V�[���̑O���錾
class BaseStageScene;

/// <summary>
/// �{�X2(����)
/// </summary>
class Boss2Body : public ObjObject3d
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>�{�X2(����)</returns>
	static Boss2Body* Create(const Vector3& basePos);

	//getter
	static const int GetMaxHP() { return Boss2Body::maxHP; }
	static const Vector3& GetNormalSize() { return Boss2Body::normalSize; }

	//setter
	static void SetStageScene(BaseStageScene* stageScene) { Boss2Body::stageScene = stageScene; }
	static void SetBodyModel(ObjModel* model) { Boss2Body::bodyModel = model; }

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Update() override;

	/// <summary>
	/// �_���[�W����炤
	/// </summary>
	/// <param name="attackPower">�U����</param>
	void Damage(int attackPower, const Vector3& collisionPos, const Vector3& subjectVel);

	/// <summary>
	/// �~��
	/// </summary>
	/// <param name="time">�C�[�W���O�p(0�`1)�̐��l</param>
	void Fall(const float time);

	//getter
	Vector3 GetWorldPos();
	const bool GetIsDead() { return isDead; }
	const bool GetIsDelete() { return isDelete; }
	const int GetDamageNum() { return damageNum; }

private: //�����o�֐�
	/// <summary>
	/// �ҋ@����
	/// </summary>
	void Wait();

	/// <summary>
	/// �_���[�W��Ԃ̏���
	/// </summary>
	void DamageMode();

	/// <summary>
	/// �_���[�W��Ԃ̃m�b�N�o�b�N�����Z�b�g
	/// </summary>
	/// <param name="subjectVel">�Ώۂ̑��x</param>
	void SetDamageKnockback(const Vector3& subjectVel);

	/// <summary>
	/// �_���[�W��Ԃ̃m�b�N�o�b�N
	/// </summary>
	void DamageKnockback();

	/// <summary>
	/// �_���[�W��Ԃő傫�������T�C�Y��߂��Ă�������
	/// </summary>
	void DamageSizeReturn();

	/// <summary>
	/// �_���[�W����
	/// </summary>
	void DamageExplosion(const Vector3& position);

	/// <summary>
	/// �_���[�W����������Ԃ̐F�ɂ���
	/// </summary>
	void DamageColorMode();

private: //�ÓI�����o�ϐ�
	//�X�e�[�W�V�[��
	static BaseStageScene* stageScene;
	//���̂̃��f��
	static ObjModel* bodyModel;
	//�ʏ�T�C�Y
	static const Vector3 normalSize;
	//�_���[�W��Ԃ̃T�C�Y
	static const Vector3 damageSize;
	//�̗�
	static const int maxHP = 30;
	//�_���[�W��Ԃ̐F
	static const XMFLOAT4 damageColor;

private: //�����o�ϐ�
	//��~�����̍��W
	Vector3 basePos;
	//�̗�
	int HP = maxHP;
	//���S�t���O
	bool isDead = false;
	//�폜�t���O
	bool isDelete = false;
	//��炤�_���[�W��
	int damageNum;
	//�_���[�W�t���O
	bool isDamage = false;
	//�_���[�W���������u�Ԃ�
	bool isDamageTrigger = false;
	//�_���[�W��ԃ^�C�}�[
	int32_t damageTimer = 0;
	//�_���[�W�F��
	bool isDamageColor = false;
	//�m�b�N�o�b�N����
	Vector3 knockbackVec;
	//�m�b�N�o�b�N���x
	Vector3 knockbackVel;
};
