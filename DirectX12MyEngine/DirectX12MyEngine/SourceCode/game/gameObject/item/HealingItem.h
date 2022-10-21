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
	static HealingItem* Create(ObjModel* model, const Vector3& position);

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
	bool GetIsDead() const { return isDead; }

private: //�����o�֐�
	/// <summary>
	/// ��ʎ�O�܂ōs������폜���鏈��
	/// </summary>
	void FrontOfScreenDelete();

private: //�ÓI�����o�ϐ�
	//�v���C���[���@
	static Player* player;

private: //�����o�ϐ�
	//���S�t���O
	bool isDead = false;
};
