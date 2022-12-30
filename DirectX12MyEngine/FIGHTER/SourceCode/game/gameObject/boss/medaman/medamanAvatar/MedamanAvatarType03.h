#pragma once
#include "MedamanAvatar.h"

/// <summary>
/// ���_�}�[��(���g�F�^�C�v03)
/// </summary>
class MedamanAvatarType03 : public MedamanAvatar
{
public: //�ÓI�����o�֐�
	/// <summary>
	/// ��������
	/// </summary>
	/// <returns>���_�}�[��(���g�F�^�C�v03)</returns>
	static MedamanAvatarType03* Create(ObjObject3d* parent, const Vector3& position);

public: //�����o�֐�
	/// <summary>
	/// ������
	/// </summary>
	bool Initialize() override;

	/// <summary>
	/// �U�����e:���g�̓�����̃��b�N�I������
	/// </summary>
	void AttackTypeAvatarBodyBlowLockon() override;

	/// <summary>
	/// �U�����e:���g�K�g�����O�C�̒��S�Ɉړ������鏈��
	/// </summary>
	void AttackTypeAvatarGatlingMoveCenter() override;

	/// <summary>
	/// �U�����e:���g����e�̑ҋ@����
	/// </summary>
	void AttackTypeAvatarGiantBulletWait() override;

	/// <summary>
	/// �U�����e:���g����e�̈ړ������鏈��
	/// </summary>
	void AttackTypeAvatarGiantBulletMove() override;

private: //�����o�֐�
	/// <summary>
	/// ���S�����Ƃ��̓���
	/// </summary>
	void DeadAction() override;
};
