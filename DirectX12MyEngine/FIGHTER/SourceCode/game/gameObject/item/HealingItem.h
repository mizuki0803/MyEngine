#pragma once
#include "ObjObject3d.h"

//���@�N���X��O���錾
class BasePlayer;

/// <summary>
/// �񕜃A�C�e��
/// </summary>
class HealingItem : public ObjObject3d
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <param name="model">���f��</param>
	/// <returns>�񕜃A�C�e��</returns>
	static HealingItem* Create(ObjModel* model, const Vector3& position, const float size);

	//setter
	static void SetPlayer(BasePlayer* player) { HealingItem::player = player; }

public: //�����o�֐�
	/// <summary>
	/// �X�V
	/// </summary>
	void Update() override;

	/// <summary>
	/// �Փˎ��R�[���o�b�N�֐�
	/// </summary>
	virtual void OnCollision();

	//getter
	bool GetIsTouched() const { return isTouched; }
	bool GetIsDelete() const { return isDelete; }

private: //�����o�֐�
	/// <summary>
	/// ���邮���]
	/// </summary>
	void Round();

	/// <summary>
	/// ��ʎ�O�܂ōs������폜���鏈��
	/// </summary>
	void FrontOfScreenDelete();

	/// <summary>
	/// �L���L�����o�Ǘ�
	/// </summary>
	void ShineEffect();

	/// <summary>
	/// �L���L�����o�T�C�Y�ύX
	/// </summary>
	void ShineEffectSizeChange();

	/// <summary>
	/// �L���L�����o�����Ԋu�ύX
	/// </summary>
	void ShineEffectIntervalChange();

	/// <summary>
	/// �ڐG��̓���
	/// </summary>
	void TouchedAction();

private: //�ÓI�����o�ϐ�
	//�v���C���[���@
	static BasePlayer* player;
	//�L���L�����o�̏����T�C�Y
	static const float shineEffectCreateSize;
	//�L���L�����o�̏��������Ԋu
	static const int shineEffectCreateInterval = 5;

private: //�����o�ϐ�
	//��]�̑���
	float rotSpeed = 2.0f;
	//�L���L�����o�p�^�C�}�[
	int32_t shineEffectTimer = 0;
	//�L���L�����o�T�C�Y
	float shineEffectSize = 0;
	//�L���L�����o�����Ԋu
	int shineEffectInterval = shineEffectCreateInterval;
	//�L���L�����o������
	int shineEffectNum = 5;
	//�ڐG�t���O
	bool isTouched = false;
	//�ڐG��^�C�}�[
	int32_t touchedTimer = 0;
	//�폜�t���O
	bool isDelete = false;
};
