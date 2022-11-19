#pragma once
#include "ObjObject3d.h"

//���@�N���X��O���錾
class Player;

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
	static void SetPlayer(Player* player) { HealingItem::player = player; }

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
	Vector3 GetWorldPos();
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
	/// �ڐG��̓���
	/// </summary>
	void TouchedAction();

private: //�ÓI�����o�ϐ�
	//�v���C���[���@
	static Player* player;

private: //�����o�ϐ�
	//��]�̑���
	float rotSpeed = 2.0f;
	//�ڐG�t���O
	bool isTouched = false;
	//�ڐG��^�C�}�[
	int32_t touchedTimer = 0;
	//�폜�t���O
	bool isDelete = false;
};
