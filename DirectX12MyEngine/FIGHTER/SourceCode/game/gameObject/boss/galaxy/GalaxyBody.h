#pragma once
#include "ObjObject3d.h"

//�X�e�[�W�V�[���̑O���錾
class BaseStageScene;

/// <summary>
/// �M�����N�V�[(����)
/// </summary>
class GalaxyBody : public ObjObject3d
{
public: //enum
	//�o��̍s���t�F�[�Y
	enum class AppearPhase {
		Advance,	//�O�i
		Wait,		//�ҋ@
		Rotation,	//��]
	};

	//�U������p�[�c�ύX�̍s���t�F�[�Y
	enum class AttackPartChangePhase {
		Wait,		//�ҋ@
		Rotation,	//��]
	};


public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="bornPos">�������W</param>
	/// <param name="basePos">����W</param>
	/// <returns>�M�����N�V�[(����)</returns>
	static GalaxyBody* Create(const Vector3& bornPos, const Vector3& basePos);

	//getter
	static const Vector3& GetNormalSize() { return GalaxyBody::normalSize; }

	//setter
	static void SetStageScene(BaseStageScene* stageScene) { GalaxyBody::stageScene = stageScene; }
	static void SetBodyModel(ObjModel* model) { GalaxyBody::bodyModel = model; }

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	void Update() override;

	/// <summary>
	/// �_���[�W����炤
	/// </summary>
	void Damage();

	/// <summary>
	/// �㉺�����
	/// </summary>
	void Sway();

	/// <summary>
	/// �o��
	/// </summary>
	void Appear();

	/// <summary>
	/// �U������p�[�c��ύX�����]���J�n
	/// </summary>
	/// <param name="rotSpeed">��]���x</param>
	/// <param name="changeRota">�ύX��̊p�x</param>
	void AttackPartChangeRotaStart(const float rotSpeed, const float changeRota);

	/// <summary>
	/// �U������p�[�c��ύX�s��
	/// </summary>
	void AttackPartChange();

	/// <summary>
	/// ���S
	/// </summary>
	void Dead();

	//getter
	const bool GetIsAppear() { return isAppear; }
	const bool GetIsAttackPartChangeRota() { return isAttackPartChangeRota; }

private: //�����o�֐�
	/// <summary>
	/// �_���[�W��Ԃ̏���
	/// </summary>
	void DamageMode();

	/// <summary>
	/// �_���[�W��Ԃő傫�������T�C�Y��߂��Ă�������
	/// </summary>
	void DamageSizeReturn();

	/// <summary>
	/// �_���[�W����
	/// </summary>
	void DamageExplosion();

	/// <summary>
	/// �_���[�W����������Ԃ̐F�ɂ���
	/// </summary>
	void DamageColorMode();

	/// <summary>
	/// �o��̑O�i�s��
	/// </summary>
	void AppaerAdvance();

	/// <summary>
	/// �o��̑ҋ@�s��
	/// </summary>
	void AppaerWait();

	/// <summary>
	/// �o��̉�]�s��
	/// </summary>
	void AppaerRotation();

	/// <summary>
	/// �U������p�[�c�ύX�̑ҋ@�s��
	/// </summary>
	void AttackPartChangeWait();

	/// <summary>
	/// �U������p�[�c�ύX�̉�]�s��
	/// </summary>
	void AttackPartChangeRotation();

private: //�ÓI�����o�ϐ�
	//�X�e�[�W�V�[��
	static BaseStageScene* stageScene;
	//���̂̃��f��
	static ObjModel* bodyModel;
	//�ʏ�T�C�Y
	static const Vector3 normalSize;
	//�_���[�W��Ԃ̃T�C�Y
	static const Vector3 damageSize;
	//�_���[�W��Ԃ̐F
	static const XMFLOAT4 damageColor;
	//�o���Ԃ̍s���J��
	static void (GalaxyBody::* appearPhaseFuncTable[])();
	//�U������p�[�c�ύX��Ԃ̍s���J��
	static void (GalaxyBody::* attackPartChangePhaseFuncTable[])();

private: //�����o�ϐ�
	//�������W
	Vector3 bornPos;
	//��~�����̍��W
	Vector3 basePos;
	//�_���[�W�t���O
	bool isDamage = false;
	//�_���[�W���������u�Ԃ�
	bool isDamageTrigger = false;
	//�_���[�W��ԃ^�C�}�[
	int32_t damageTimer = 0;
	//�_���[�W�F��
	bool isDamageColor = false;
	//����炪��ړ���
	bool isSwayUp = false;
	//�����̑��x
	Vector3 swayVel = {};
	//�����̑��x���ő傩
	bool isSwaySpeedMax = false;
	//�s���Ɏg�p����^�C�}�[
	int32_t actionTimer = 0;
	//�o�ꒆ��
	bool isAppear = true;
	//�o��̍s���t�F�[�Y
	AppearPhase appearPhase = AppearPhase::Advance;
	//�U������p�[�c�ύX�������]�����邩
	bool isAttackPartChangeRota = false;
	//�U������p�[�c�ύX�̍s���t�F�[�Y
	AttackPartChangePhase attackPartChangePhase = AttackPartChangePhase::Wait;
	//�U������p�[�c�ύX�������]���x
	float attackPartChangeRotSpeed = 0;
	//�U������p�[�c�ύX�������]��̊p�x
	float  attackPartChangeRota = 0;
	//�U������p�[�c�ύX�������]���E������
	bool isAttackPartChangeRotaRight = true;
};
